#ifndef LIBFACEBOOK_CLIENTPRIVATE_H
#define LIBFACEBOOK_CLIENTPRIVATE_H

#include <QObject>

#include <libfacebook/auth/credentials.hpp>
#include <libfacebook/auth/token.hpp>

#include <QVariantMap>
#include <QWebFrame>
#include <QWebPage>

namespace facebook {

namespace app {
struct Settings;
}

namespace auth {
struct Credentials;
struct Token;
}

class ClientPrivate : public QObject {
    Q_OBJECT
public:
    ClientPrivate();
    virtual ~ClientPrivate();

    void login(const app::Settings & settings, const auth::Credentials & credentials);

    QVariantMap get(const QString & object);
    // void put()

    const auth::Token & token() const;

private:
    enum State {
        EXPECT_NONE = 0,
        EXPECT_FORM = 1,
        EXPECT_TOKEN = 2,
    } state;

    void changeState(State newState);

    Q_SLOT void onFormLoaded(bool result);
    Q_SLOT void onLoginComplete(bool result);

    QWebPage page;
    QWebFrame & frame;

    auth::Credentials remoteKey;
    auth::Token access_token;
};

inline const auth::Token & ClientPrivate::token() const
{
    return access_token;
}

} /* namespace facebook */

#endif // LIBFACEBOOK_CLIENTPRIVATE_H
