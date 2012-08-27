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
#include <QtNetwork/QNetworkAccessManager>

#include <qjson/parser.h>

namespace facebook {

ClientPrivate::ClientPrivate() :
        frame(*page.mainFrame()),
        state(EXPECT_NONE)
{

}

ClientPrivate::~ClientPrivate()
{

}

void ClientPrivate::login(const app::Settings & settings, const auth::Credentials & credentials) {

    access_token.value.clear();
    remoteKey = credentials;
    
    QUrl target(QString("https://www.facebook.com/login.php"));

    target.addQueryItem(QString("api_key"), settings.apiKey);
    target.addQueryItem(QString("cancel_url"), settings.baseUrl);

    target.addQueryItem(QString("display"), QString("page"));
    target.addQueryItem(QString("fbconnect"), QString("1"));

    target.addQueryItem(QString("next"), settings.baseUrl);

    target.addQueryItem(QString("return_session"), QString("1"));
    target.addQueryItem(QString("session_version"), QString("3"));
    target.addQueryItem(QString("v"), QString("1.0"));

    // TODO: Replace with app::Settings fields.
    target.addQueryItem(QString("req_params"), QString("user_birthday"));

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loading ";
    qDebug() << target;
#endif

    changeState(EXPECT_FORM);
    frame.load(target);

    do {
        QCoreApplication::processEvents();
    } while(state != EXPECT_NONE);
    
    remoteKey = auth::Credentials();
}

void ClientPrivate::changeState(State newState) {

    static const Qt::ConnectionType type = Qt::DirectConnection;

    // Unbind the old pagehandler...
    switch(state) {
    case EXPECT_FORM:
        QObject::disconnect(&page, SIGNAL(loadFinished(bool)), this, SLOT(onFormLoaded(bool)));
        break;
    case EXPECT_TOKEN:
        QObject::disconnect(&page, SIGNAL(loadFinished(bool)), this, SLOT(onLoginComplete(bool)));
        break;
    case EXPECT_NONE:
    default:
        break;
    }

    state = newState;

    // Bind a new pagehandler...
    switch(newState) {
    case EXPECT_FORM:
        QObject::connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(onFormLoaded(bool)), type);
        break;
    case EXPECT_TOKEN:
        QObject::connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(onLoginComplete(bool)), type);
        break;
    case EXPECT_NONE:
    default:
        break;
    }
}

void ClientPrivate::onFormLoaded(bool result) {

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loaded Form";
    qDebug() << frame.url();
#endif
    
    if (!result) {
        changeState(EXPECT_NONE); // Send a null token;
        return;
    }

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
        changeState(EXPECT_NONE); // Send a null token
        return;
    }

    email.setAttribute("value", remoteKey.login);
    pass.setAttribute("value", remoteKey.pass);

    changeState(EXPECT_TOKEN);
    submit.evaluateJavaScript("this.click()");
}

void ClientPrivate::onLoginComplete(bool result) {

#ifdef VERBOSE_OUTPUT
    qDebug() << "Loaded Result";
    qDebug() << frame.url();
#endif
        
    access_token.value = QJson::Parser().parse(frame.url().queryItemValue("session").toUtf8()).toMap().value("access_token").toString();
    changeState(EXPECT_NONE);
}

QVariantMap ClientPrivate::get(const QString & object) {
    QUrl url;
    url.setScheme("https");
    url.setHost("graph.facebook.com");
    url.setPath(object);
    url.setQueryItems(QList<QPair<QString, QString> >() << qMakePair<QString, QString>(QString("access_token"), access_token.value));
#ifdef VERBOSE_OUTPUT
    qDebug() << "Fetching";
    qDebug() << url;
#endif

    QNetworkRequest request;
    QNetworkAccessManager manager;
    
    request.setUrl(url);
    QNetworkReply * const graphData = manager.get(request);
    
    while(graphData->isRunning()) {
        QCoreApplication::processEvents();
    }
    const QVariantMap jsonMap(QJson::Parser().parse(graphData->readAll()).toMap());
    
    graphData->deleteLater();

    return jsonMap;
}

}