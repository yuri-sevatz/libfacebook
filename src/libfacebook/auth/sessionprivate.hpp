#ifndef SESSIONPRIVATE_HPP
#define SESSIONPRIVATE_HPP

#include <QObject>

#include "credentials.hpp"
#include "token.hpp"

#include <QSemaphore>
#include <QWebPage>
#include <QWebFrame>

namespace facebook {

namespace app {
    class Settings;
}

namespace auth {

class Credentials;

class SessionPrivate : private QObject {
    Q_OBJECT
public:
    SessionPrivate();
    virtual ~SessionPrivate();

    void login();

    const Token & token() const;

    const Credentials * credentials;
    const app::Settings * settings;

private:
    Q_DISABLE_COPY(SessionPrivate);
    Q_SLOT void onLoadStarted();
    Q_SLOT void onFormLoaded(bool result);
    Q_SLOT void onLoginComplete(bool result);

    enum State {
        EXPECT_NONE = 0,
        EXPECT_FORM = 1,
        EXPECT_TOKEN = 2,
    } state;

    void changeState(State newState);
    void sendResult(const Token & token);

private:

    QWebPage page;
    QWebFrame & frame;
    QSemaphore blocker;
    Token lastToken;
};

} /* namespace auth */
} /* namespace facebook */

#endif // SESSIONPRIVATE_HPP
