#include "clientprivate.hpp"

#include <libfacebook/include/config.h>

#ifdef VERBOSE_OUTPUT
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QUrl>
#include <QUrlQuery>

#include <QWebFrame>
#include <QWebElement>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace facebook {

ClientPrivate::ClientPrivate() :
    frame(*page.mainFrame()),
    manager(*page.networkAccessManager()),
    pageLoader(page),
    domLoader(frame)
{

}

ClientPrivate::~ClientPrivate() {}

inline bool ClientPrivate::load() {
#ifdef VERBOSE_OUTPUT
    qDebug() << "***Click***";
#endif
    const bool result = pageLoader.load();
#ifdef VERBOSE_OUTPUT
    qDebug() << "***Done***";
#endif
    return result;
}

inline bool ClientPrivate::load(const QUrl & url) {
#ifdef VERBOSE_OUTPUT
    qDebug() << "***Link***";
#endif

    const bool result = pageLoader.load(url);
#ifdef VERBOSE_OUTPUT
    if (result) {
        qDebug() << "***Done***";
    }
#endif
    return result;
}

bool ClientPrivate::login(const twirl::Login & login) {

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading Form...";
#endif

    if (!load(FacebookPage::loginUrl()) || !page.viewing(FacebookPage::loginUrl())) {
        return false;
    }

#ifdef VERBOSE_OUTPUT
    qDebug() << "Logging In...";
#endif

    QWebElement email =
        frame.findFirstElement("input[id=\"email\"]");
    QWebElement pass =
        frame.findFirstElement("input[id=\"pass\"]");
    QWebElement persist =
        frame.findFirstElement("input[id=\"persist_box\"]");
    QWebElement submit =
        frame.findFirstElement("input[name=\"login\"]");

#ifdef VERBOSE_OUTPUT
    qDebug() << "Email" << (email.isNull() ? "NOT" : ":") << "Found";
    qDebug() << "Password" << (pass.isNull() ? "NOT" : ":") << "Found";
    qDebug() << "Persist" << (persist.isNull() ? "NOT" : ":") << "Found";
    qDebug() << "Submit" << (submit.isNull() ? "NOT" : ":") << "Found";
#endif

    if (email.isNull() || pass.isNull() || persist.isNull() || submit.isNull()) {
        return false;
    }

    email.setAttribute("value", login.username());
    pass.setAttribute("value", login.password());
    persist.evaluateJavaScript("this.click()");
    submit.evaluateJavaScript("this.click()");

    return load() && page.viewing(FacebookPage::mainUrl());
}

bool ClientPrivate::logout() {
    return load(FacebookPage::logoutUrl());
}

bool ClientPrivate::requestToken(const App & app, const QStringList & permissions) {

#ifdef VERBOSE_OUTPUT
    qDebug() << "Requesting Token...";
#endif

    actHuman();

    QUrl next(FacebookPage::authUrl());
    QUrlQuery query;
    query.addQueryItem("client_id", app.apiKey());
    query.addQueryItem("display", "page");
    query.addQueryItem("response_type", "token");
    if (!permissions.isEmpty()) {
        query.addQueryItem("scope", permissions.join(","));
    }
    query.addQueryItem("redirect_uri", app.baseUrl());
    next.setQuery(query);

    page.setTarget(app.baseUrl());

    load(next);
    const bool ret = page.viewing(app);

#ifdef VERBOSE_OUTPUT
    qDebug() << "Token Request" << (ret ? "Submitted" : "Rejected");
#endif
    return ret;
}

Token ClientPrivate::acquireToken(const App & app, const QStringList & permissions) {
    Token ret;

    if (requestToken(app, permissions) || (page.viewing(FacebookPage::authUrl()) && grantAuth(app, permissions))) {
#ifdef VERBOSE_OUTPUT
        qDebug() << "Extracting Token...";
#endif

        const QUrlQuery fragment(page.address().fragment());

        ret.setValue(fragment.queryItemValue("access_token"));

        if (const int expiry = fragment.queryItemValue("expires_in").toInt()) {
            ret.setExpiry(QDateTime::currentDateTime().addSecs(expiry));
        }
    }

    return ret;
}

bool ClientPrivate::grantAuth(const App & app, const QStringList & permissions) {
#ifdef VERBOSE_OUTPUT
    qDebug() << "Authorizing...";
#endif
        
    QWebElement okay;
    do {
        // The page is loaded, the content is not...
        okay = frame.findFirstElement("button[name=\"__CONFIRM__\"]");
            
        if (!okay.isNull()) {
            break;
        }

#ifdef VERBOSE_OUTPUT
        qDebug() << "Loading...";
#endif
        
        domLoader.load();
    } while (true);
    
#ifdef VERBOSE_OUTPUT
    qDebug() << "Confirm" << (okay.isNull() ? "NOT" : ":") << "Found";
#endif

    if (okay.isNull()) {
        return false;
    }

#ifdef VERBOSE_OUTPUT
    qDebug() << "Granting Permissions";
#endif

    okay.evaluateJavaScript("this.click()");
    load();

    // Check for redirect to app, otherwise resubmit request
    return page.viewing(app) || requestToken(app, permissions);
}

QUrl ClientPrivate::objectUrl(const QString & object, const QMap<QString,QString> & params) {
    QUrl url(FacebookPage::graphUrl());
    // Note: SetPath blanks out the whole QUrl if setting a non-'/'-prefixed path (WTF?!?)
    url.setPath(object.startsWith('/') ? object : QString(object).prepend('/'));

    QUrlQuery query;
    query.addQueryItem("access_token", token.value());
    for(QMap<QString,QString>::const_iterator iter = params.begin(); iter != params.end(); ++iter) {
        query.addQueryItem(iter.key(), iter.value());
    }
    url.setQuery(query);

    return url;
}

inline QJsonDocument ClientPrivate::read(QNetworkReply * const reply) {
    return reply ? decode(QScopedPointer<QNetworkReply>(requestLoader.load(reply))->readAll()) : QJsonDocument();
}

inline QJsonDocument ClientPrivate::decode(const QByteArray & json) {
    return QJsonDocument::fromJson(json);
}

inline QByteArray ClientPrivate::encode(const QJsonDocument & document) {
    return document.toJson();
}

QJsonDocument ClientPrivate::get(const QString & object, const QMap<QString,QString> & params) {

    QNetworkRequest request(objectUrl(object, params));

#ifdef VERBOSE_OUTPUT
    qDebug() << "GET" << request.url();
#endif

    return read(manager.get(request));
}

QJsonDocument ClientPrivate::post(const QString& object, const QMap<QString,QString> & params, const QByteArray & data) {
    QNetworkRequest request(objectUrl(object, params));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

#ifdef VERBOSE_OUTPUT
    qDebug() << "POST" << request.url();
    qDebug() << "DATA" << data;
#endif

    return read(manager.post(request, data));
}

QJsonDocument ClientPrivate::del(const QString & object, const QMap<QString,QString> & params) {
    QNetworkRequest request(objectUrl(object, params));

#ifdef VERBOSE_OUTPUT
    qDebug() << "DELETE" << request.url();
#endif

    return read(manager.deleteResource(request));
}

bool ClientPrivate::load(twirl::Session & session) {
    QList<QNetworkCookie> cookies;
    if (session.load(cookies)) {
        manager.cookieJar()->setCookiesFromUrl(cookies, QUrl("https://facebook.com"));
        return true;
    } else {
        return false;
    }
}

bool ClientPrivate::save(twirl::Session & session) {
    return session.save(manager.cookieJar()->cookiesForUrl(QUrl("https://facebook.com")));
}

void ClientPrivate::actHuman() {
    if (!lastRequest.isNull()) {
        const int lag = lastRequest.msecsTo(QDateTime::currentDateTime());

        // Dear security: Don't make me put rand() here.  (It won't end well for you! xD)
        if (lag < 5000) {
            delay.sleep(lag);
        }
    }
    lastRequest = QDateTime::currentDateTime();
}

}
