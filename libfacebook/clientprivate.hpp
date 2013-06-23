#ifndef LIBFACEBOOK_CLIENTPRIVATE_H
#define LIBFACEBOOK_CLIENTPRIVATE_H

#include <libfacebook/app.hpp>
#include <libfacebook/token.hpp>

#include <libtwirl/model/login.hpp>
#include <libtwirl/model/session.hpp>
#include <libtwirl/web/pageloader.hpp>
#include <libtwirl/web/requestloader.hpp>

#include <QByteArray>
#include <QStringList>
#include <QVariantMap>
#include <QWebPage>

class QNetworkAccessManager;
class QNetworkReply;
class QWebFrame;

namespace facebook {

class ClientPrivate {
public:
    ClientPrivate();
    virtual ~ClientPrivate();

    bool login(const twirl::Login & login);
    bool logout();

    Token acquireToken(const App & app, const QStringList & permissions);

    QVariantMap get(const QString & object);
    QVariantMap post(const QString & object, const QVariantMap & data);
    QVariantMap post(const QString & object, const QByteArray & data);
    QVariantMap del(const QString & object);

    bool load(twirl::Session & session);
    bool save(twirl::Session & session);

    Token token;

private:
    bool load();
    bool load(const QUrl & url);

    bool requestToken(const App & app, const QStringList & permissions);

    bool grantAuth(const App & app, const QStringList & permissions);

    bool viewingBase(const App & app) const;
    bool viewingAuth() const;
    bool viewingLogin() const;

    QVariantMap decode(QNetworkReply * const reply);
    QByteArray encode(const QVariantMap & data);
    QUrl objectUrl(const QString & object);

    static const QUrl authUrl;
    static const QUrl loginUrl;
    static const QUrl logoutUrl;

    QWebPage page;
    QWebFrame & frame;
    QNetworkAccessManager & manager;

    twirl::PageLoader pageLoader;
    twirl::RequestLoader requestLoader;
};

} /* namespace facebook */

#endif // LIBFACEBOOK_CLIENTPRIVATE_H
