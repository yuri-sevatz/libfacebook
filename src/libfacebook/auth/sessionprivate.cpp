#include "sessionprivate.hpp"

#include <QUrl>
#include <QWebFrame>
#include <QWebElement>

#include <iostream>

#include <libfacebook/app/settings.hpp>

namespace facebook {
namespace auth {

SessionPrivate::SessionPrivate() :
        credentials(0),
        settings(0),
        frame(*page.mainFrame()),
        blocker(1),
        state(EXPECT_NONE)
{
    // Invert the meaning of the semaphore
    // So that it can function as a "blocker"
    blocker.acquire(1);

    QObject::connect(&page, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()), Qt::DirectConnection);
}

SessionPrivate::~SessionPrivate() {

}

void SessionPrivate::login() {

    QUrl target(QString("https://www.facebook.com/login.php"));

    if (settings) {
        target.addQueryItem(QString("api_key"), QString::fromStdString(settings->apiKey));
        target.addQueryItem(QString("cancel_url"), QString::fromStdString(settings->baseUrl));
    }

    target.addQueryItem(QString("display"), QString("page"));
    target.addQueryItem(QString("fbconnect"), QString("1"));

    if (settings) {
        target.addQueryItem(QString("next"), QString::fromStdString(settings->baseUrl));
    }

    target.addQueryItem(QString("return_session"), QString("1"));
    target.addQueryItem(QString("session_version"), QString("3"));
    target.addQueryItem(QString("v"), QString("1.0"));

    if (settings) {
        // TODO: Replace with app::Settings fields.
        target.addQueryItem(QString("req_params"), QString("user_birthday"));
    }

    changeState(EXPECT_FORM);
    frame.load(target);

    std::cout << "Caller Blocked... Awaiting Response..." << std::endl;

    // Attempt to unblock
    // XXX: Not blocking this causes the caller NOT to block!
    // Unfortunately this causes the QWebkit code to get deallocated before the app is finished with it!
    blocker.acquire(1);
}

void SessionPrivate::changeState(State newState) {

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

void SessionPrivate::onFormLoaded(bool result) {

    std::cout << "Loaded Form" << std::endl;

    if (!result) {
        sendResult(Token()); // Send a null token;
        return;
    }

    QWebElement email =
        frame.findFirstElement("input[id=\"email\"]");
    QWebElement pass =
        frame.findFirstElement("input[id=\"pass\"]");
    QWebElement submit =
        frame.findFirstElement("input[name=\"login\"]");

    std::cout << "Email:" << std::string(email.isNull() ? "NOT" : "") << "Found" << std::endl;
    std::cout << "Password:" << std::string(pass.isNull() ? "NOT" : "") << "Found" << std::endl;
    std::cout << "Submit:" << std::string(submit.isNull() ? "NOT" : "") << "Found" << std::endl;

    if (email.isNull() || pass.isNull() || submit.isNull()) {
        sendResult(Token()); // Send a null token
        return;
    }

    if (settings) {
        email.setAttribute("value", QString::fromStdString(credentials->username()));
        pass.setAttribute("value", QString::fromStdString(credentials->password()));
    }

    changeState(EXPECT_TOKEN);
    submit.evaluateJavaScript("this.click()");
}

void SessionPrivate::onLoginComplete(bool result) {

    std::cout << "Loaded Result" << std::endl;

    const QUrl request(frame.requestedUrl());
    static const QString key("access_token");

    Token accessToken;

    if (request.hasQueryItem(key)) {
        accessToken.setValue(request.queryItemValue(key).toStdString());
    }

    std::cout << "Loaded: " << request.toString().toStdString() << std::endl;
    std::cout << accessToken.value() << std::endl;

    sendResult(accessToken);
}

inline void SessionPrivate::sendResult(const Token & token) {

    changeState(EXPECT_NONE);
    lastToken = token;
    blocker.release(1);
}

const Token & SessionPrivate::token() const {
    return lastToken;
}

void SessionPrivate::onLoadStarted() {
    std::cout << "Load Started" << std::endl;
}

} /* namespace auth */
} /* namespace facebook */
