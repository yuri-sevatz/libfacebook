#ifndef LIBFACEBOOK_CLIENT_HPP
#define LIBFACEBOOK_CLIENT_HPP

#include <QObject>

#include <libfacebook/app.hpp>
#include <libfacebook/token.hpp>

#include <libtwirl/model/login.hpp>
#include <libtwirl/model/session.hpp>

#include <QByteArray>
#include <QScopedPointer>
#include <QString>
#include <QStringList>
#include <QVariantMap>

namespace facebook {

class ClientPrivate;

class Client : private QObject {
    Q_OBJECT
public:
    Client();
    virtual ~Client();

    bool login(const twirl::Login & login);
    bool logout();

    Token acquireToken(const App & app, const QStringList & permissions = QStringList());

    QVariantMap get(const QString & object);
    QVariantMap post(const QString & object, const QVariantMap & data);
    QVariantMap post(const QString & object, const QByteArray & data);
    QVariantMap del(const QString & object);

    bool load(twirl::Session & session);
    bool save(twirl::Session & session);

    const Token & token() const;
    void setToken(const Token & token);

private:
    Q_DISABLE_COPY(Client);
    Q_DECLARE_PRIVATE(Client);

    const QScopedPointer<ClientPrivate> d_ptr;
};

} /* namespace facebook */

#endif // LIBFACEBOOK_CLIENT_HPP
