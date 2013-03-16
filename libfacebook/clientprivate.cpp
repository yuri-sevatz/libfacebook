#include "clientprivate.hpp"

#include <libfacebook/include/config.h>

#include <libfacebook/app/settings.hpp>
#include <libfacebook/auth/credentials.hpp>

#ifdef VERBOSE_OUTPUT
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace facebook {

ClientPrivate::ClientPrivate() :
    frame(*page.mainFrame()),
    pageLoader(page)
{

}

ClientPrivate::~ClientPrivate() {

}

bool ClientPrivate::load() {
#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading AJAX...";
#endif

    return pageLoader.load();
}

bool ClientPrivate::load(const QUrl & url) {
#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading Page...";
    qDebug() << url;
#endif

    return pageLoader.load(url);
}


bool ClientPrivate::login(const app::Settings & settings, const auth::Credentials & credentials) {

    access_token.value.clear();

    QUrl target(QString("https://www.facebook.com/login.php"));

    target.addQueryItem(QString("api_key"), settings.apiKey);
    target.addQueryItem(QString("cancel_url"), settings.baseUrl);

    target.addQueryItem(QString("display"), QString("page"));
    target.addQueryItem(QString("fbconnect"), QString("1"));

    target.addQueryItem(QString("next"), settings.baseUrl);

    target.addQueryItem(QString("return_session"), QString("1"));
    target.addQueryItem(QString("session_version"), QString("3"));
    target.addQueryItem(QString("v"), QString("1.0"));

    target.addQueryItem(QString("req_params"), settings.permissions.join(","));

    if (!load(target)) {
        return false;
    }

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loaded Form";
    qDebug() << frame.url();
#endif

    QWebElement email =
        frame.findFirstElement("input[id=\"email\"]");
    QWebElement pass =
        frame.findFirstElement("input[id=\"pass\"]");
    QWebElement submit =
        frame.findFirstElement("input[name=\"login\"]");

#ifdef VERBOSE_OUTPUT
    qDebug() << "Email:" << QString(email.isNull() ? "NOT" : "") << "Found";
    qDebug() << "Password:" << QString(pass.isNull() ? "NOT" : "") << "Found";
    qDebug() << "Submit:" << QString(submit.isNull() ? "NOT" : "") << "Found";
#endif

    if (email.isNull() || pass.isNull() || submit.isNull()) {
        return false;
    }

    email.setAttribute("value", credentials.login);
    pass.setAttribute("value", credentials.pass);

    submit.evaluateJavaScript("this.click()");
    if (!load()) {
        return false;
    }

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loaded Result";
    qDebug() << frame.url();
#endif

    access_token.value = QJson::Parser().parse(frame.url().queryItemValue("session").toUtf8()).toMap().value("access_token").toString();
    return !access_token.value.isEmpty();
}

bool ClientPrivate::logout(const app::Settings & settings) {
    QUrl target(QString("https://www.facebook.com/logout.php"));

    target.addQueryItem(QString("next"), settings.baseUrl);
    target.addQueryItem(QString("access_token"), access_token.value);

    if (load(target)) {
#ifdef VERBOSE_OUTPUT
        qDebug() << "Loaded Result";
        qDebug() << frame.url();
#endif
        access_token.value.clear();
        return true;
    } else {
        return false;
    }
}

QUrl ClientPrivate::objectUrl(const QString & object) {
    QUrl url;
    url.setScheme("https");
    url.setHost("graph.facebook.com");
    url.setPath(object);
    url.setQueryItems(QList<QPair<QString, QString> >() << qMakePair<QString, QString>(QString("access_token"), access_token.value));
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

}
