#ifndef LIBFACEBOOK_CLIENT_HPP
#define LIBFACEBOOK_CLIENT_HPP

#include <QObject>

#include <QScopedPointer>
#include <QString>
#include <QVariantMap>

namespace facebook {

namespace app {
struct Settings;
}

namespace auth {
struct Credentials;
struct Token;
}

class ClientPrivate;

class Client : private QObject {
    Q_OBJECT
public:
    Client();
    virtual ~Client();

    void login(const app::Settings & settings, const auth::Credentials & credentials);
    QVariantMap get(const QString & object);
    const auth::Token & token() const;

private:
    Q_DISABLE_COPY(Client);
    Q_DECLARE_PRIVATE(Client);

    const QScopedPointer<ClientPrivate> d_ptr;
};

} /* namespace facebook */

#endif // LIBFACEBOOK_CLIENT_HPP
