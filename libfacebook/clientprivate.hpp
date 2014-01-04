#ifndef LIBFACEBOOK_CLIENTPRIVATE_H
#define LIBFACEBOOK_CLIENTPRIVATE_H

#include <libfacebook/app.hpp>
#include <libfacebook/token.hpp>

#include <libfacebook/facebookpage.hpp>

#include <libtwirl/model/login.hpp>
#include <libtwirl/model/session.hpp>
#include <libtwirl/util/delay.hpp>
#include <libtwirl/web/domloader.hpp>
#include <libtwirl/web/pageloader.hpp>
#include <libtwirl/web/requestloader.hpp>

#include <QByteArray>
#include <QDateTime>
#include <QStringList>
#include <QJsonDocument>

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

    QJsonDocument get(const QString & object, const QMap<QString,QString> & params);
    QJsonDocument post(const QString & object, const QMap<QString,QString> & params, const QByteArray & data);
    QJsonDocument del(const QString & object, const QMap<QString,QString> & params);

    bool load(twirl::Session & session);
    bool save(twirl::Session & session);

    Token token;

private:
    bool load();
    bool load(const QUrl & url);

    bool requestToken(const App & app, const QStringList & permissions);

    bool grantAuth(const App & app, const QStringList & permissions);

    // Quick -- ______ !!! =)
    void actHuman();

    QJsonDocument read(QNetworkReply * const reply);

    QJsonDocument decode(const QByteArray & data);
    QByteArray encode(const QJsonDocument & data);

    QUrl objectUrl(const QString & object, const QMap<QString,QString> & params);

    FacebookPage page;
    QWebFrame & frame;
    QNetworkAccessManager & manager;

    twirl::PageLoader pageLoader;
    twirl::DomLoader domLoader;
    twirl::RequestLoader requestLoader;
    twirl::Delay delay;

    QDateTime lastRequest;
};

} /* namespace facebook */

#endif // LIBFACEBOOK_CLIENTPRIVATE_H
