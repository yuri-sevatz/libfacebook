#ifndef LIBFACEBOOK_CLIENTPRIVATE_H
#define LIBFACEBOOK_CLIENTPRIVATE_H

#include <QObject>

#include <QByteArray>

#include <libfacebook/auth/token.hpp>
#include <libfacebook/web/pageloader.hpp>
#include <libfacebook/web/requestloader.hpp>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QVariantMap>
#include <QWebFrame>
#include <QWebPage>

namespace facebook {

namespace app {
struct Settings;
}

namespace auth {
struct Credentials;
}

class ClientPrivate : public QObject {
    Q_OBJECT
public:
    ClientPrivate();
    virtual ~ClientPrivate();

    bool login(const app::Settings & settings, const auth::Credentials & credentials);
    bool logout(const app::Settings & settings);

    QVariantMap get(const QString & object);
    QVariantMap post(const QString & object, const QVariantMap & data);
    QVariantMap post(const QString & object, const QByteArray & data);
    QVariantMap del(const QString & object);

    auth::Token & token();
    const auth::Token & token() const;

private:
    bool load();
    bool load(const QUrl & url);

    QVariantMap decode(QNetworkReply * const reply);
    QByteArray encode(const QVariantMap & data);
    QUrl objectUrl(const QString & object);

    QWebPage page;
    QWebFrame & frame;

    web::PageLoader pageLoader;
    web::RequestLoader requestLoader;

    QNetworkAccessManager manager;

    auth::Token access_token;
};

inline auth::Token & ClientPrivate::token() {
    return access_token;
}

inline const auth::Token & ClientPrivate::token() const {
    return access_token;
}

} /* namespace facebook */

#endif // LIBFACEBOOK_CLIENTPRIVATE_H
