#include "clientprivate.hpp"

#include <libfacebook/include/config.h>

#ifdef VERBOSE_OUTPUT
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QUrl>
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

const QUrl ClientPrivate::authUrl("https://www.facebook.com/dialog/oauth");
const QUrl ClientPrivate::loginUrl("https://www.facebook.com/login.php");
const QUrl ClientPrivate::logoutUrl("https://www.facebook.com/logout.php");

ClientPrivate::ClientPrivate() :
    frame(*page.mainFrame()),
    manager(*page.networkAccessManager()),
    pageLoader(page) {

}

ClientPrivate::~ClientPrivate() {}

inline bool ClientPrivate::load() {
#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading AJAX...";
#endif

    const bool result = pageLoader.load();
#ifdef VERBOSE_OUTPUT
    if (result) {
        qDebug() << "Done";
        qDebug() << frame.url().toEncoded();
    }
#endif
    return result;
}

inline bool ClientPrivate::load(const QUrl & url) {
#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading Page...";
    qDebug() << url.toEncoded();
#endif

    const bool result = pageLoader.load(url);
#ifdef VERBOSE_OUTPUT
    if (result) {
        qDebug() << "Done";
        qDebug() << frame.url().toEncoded();
    }
#endif
    return result;
}

bool ClientPrivate::login(const twirl::Login & login) {

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading Form...";
#endif

    if (!load(loginUrl) || !viewingLogin()) {
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
    qDebug() << "Email:" << (email.isNull() ? "NOT" : "") << "Found";
    qDebug() << "Password:" << (pass.isNull() ? "NOT" : "") << "Found";
    qDebug() << "Persist:" << (persist.isNull() ? "NOT" : "") << "Found";
    qDebug() << "Submit:" << (submit.isNull() ? "NOT" : "") << "Found";
#endif

    if (email.isNull() || pass.isNull() || persist.isNull() || submit.isNull()) {
        return false;
    }

    email.setAttribute("value", login.username());
    pass.setAttribute("value", login.password());
    persist.evaluateJavaScript("this.click()");
    submit.evaluateJavaScript("this.click()");

    // TODO: Add session validity detection
    return load();
}

bool ClientPrivate::logout() {
    return load(logoutUrl);
}

bool ClientPrivate::requestToken(const App & app, const QStringList & permissions) {

#ifdef VERBOSE_OUTPUT
    qDebug() << "Requesting Token...";
#endif

    QUrl next(authUrl);
    next.addEncodedQueryItem("redirect_uri", QUrl::toPercentEncoding(app.baseUrl()));
    next.addEncodedQueryItem("scope", QUrl::toPercentEncoding(permissions.join(",+"), "+"));
    next.addEncodedQueryItem("response_type", "token");
    next.addEncodedQueryItem("client_id", QUrl::toPercentEncoding(app.apiKey()));
    next.addEncodedQueryItem("ret", "login");

    QUrl cancelUri(app.baseUrl());
    cancelUri.addEncodedQueryItem("error", "access_denied");
    cancelUri.addEncodedQueryItem("error_code", "200");
    cancelUri.addEncodedQueryItem("error_description", "Permissions+error");
    cancelUri.addEncodedQueryItem("error_reason", "user_denied#_=_");

    QUrl target(loginUrl);
    target.addEncodedQueryItem("skip_api_login", "1");
    target.addEncodedQueryItem("api_key", QUrl::toPercentEncoding(app.apiKey()));
    target.addEncodedQueryItem("signed_next", "1");
    target.addEncodedQueryItem("next", QUrl::toPercentEncoding(next.toEncoded()));
    target.addEncodedQueryItem("cancel_uri", QUrl::toPercentEncoding(cancelUri.toEncoded()));
    target.addEncodedQueryItem("display", "page");

    return load(target);
}

Token ClientPrivate::acquireToken(const App & app, const QStringList & permissions) {
    Token ret;

    if (requestToken(app, permissions) && (viewingBase(app) || (viewingAuth() && grantAuth(app, permissions)))) {
#ifdef VERBOSE_OUTPUT
        qDebug() << "Extracting Token...";
#endif

        const QUrl fragment(QString("?").append(frame.url().fragment()));

        ret.setValue(fragment.queryItemValue("access_token"));
        ret.setExpiry(QDateTime::currentDateTime().addSecs(
                          fragment.queryItemValue("expires_in").toInt()
        ));
    }

    return ret;
}

bool ClientPrivate::grantAuth(const App & app, const QStringList & permissions) {
#ifdef VERBOSE_OUTPUT
    qDebug() << "Authorizing...";
#endif

    QWebElement okay =
        frame.findFirstElement("button[name=\"__CONFIRM__\"]");

#ifdef VERBOSE_OUTPUT
    qDebug() << "Confirm:" << (okay.isNull() ? "NOT" : "") << "Found";
#endif

    if (okay.isNull()) {
        return false;
    }

#ifdef VERBOSE_OUTPUT
    qDebug() << "Granting Permissions";
#endif

    okay.evaluateJavaScript("this.click()");

    // (1) Load and (2) Check for redirect to baseUrl, otherwise (3) Resubmit request and (4) recheck redirect
    return load() && (viewingBase(app) || (requestToken(app, permissions) && viewingBase(app)));
}

QUrl ClientPrivate::objectUrl(const QString & object) {
    QUrl url;
    url.setScheme("https");
    url.setHost("graph.facebook.com");
    url.setPath(object);
    url.addEncodedQueryItem("access_token", QUrl::toPercentEncoding(token.value()));
    return url;
}

inline QVariantMap ClientPrivate::decode(QNetworkReply * const reply) {
    return QJson::Parser().parse(QScopedPointer<QNetworkReply>(requestLoader.load(reply))->readAll()).toMap();
}

inline QByteArray ClientPrivate::encode(const QVariantMap & data) {
    return QJson::Serializer().serialize(data);
}

QVariantMap ClientPrivate::get(const QString & object) {

    QNetworkRequest request;
    request.setUrl(objectUrl(object));

#ifdef VERBOSE_OUTPUT
    qDebug() << "GET" << request.url();
#endif

    return decode(manager.get(request));
}

QVariantMap ClientPrivate::post(const QString & object, const QVariantMap & data) {
    return post(object, encode(data));
}

QVariantMap ClientPrivate::post(const QString & object, const QByteArray & data) {
    QNetworkRequest request;
    request.setUrl(objectUrl(object));

#ifdef VERBOSE_OUTPUT
    qDebug() << "POST" << request.url();
#endif

    return decode(manager.post(request, data));
}

QVariantMap ClientPrivate::del(const QString & object) {
    QNetworkRequest request;
    request.setUrl(objectUrl(object));

#ifdef VERBOSE_OUTPUT
    qDebug() << "DELETE" << request.url();
#endif

    return decode(manager.deleteResource(request));
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

bool ClientPrivate::viewingBase(const App & app) const {
    return frame.url().toString(uniqueUrl()) == QUrl(app.baseUrl()).toString(uniqueUrl());
}

bool ClientPrivate::viewingAuth() const {
    return frame.url().toString(uniqueUrl()) == authUrl.toString();
}

bool ClientPrivate::viewingLogin() const {
    return frame.url().toString(uniqueUrl()) == loginUrl.toString();
}

inline QUrl::FormattingOptions ClientPrivate::uniqueUrl() {
    return QUrl::RemoveQuery | QUrl::RemoveFragment | QUrl::StripTrailingSlash;
}

}
