#include "fbhack.hpp"

#include <libfacebook/auth/token.hpp>

#include <QDebug>
#include <QApplication>
#include <QStringList>

#include <libfacebook/include/config.h>
#include <libfacebook/app/settings.hpp>
#include <libfacebook/auth/credentials.hpp>
#include <libfacebook/client.hpp>

using namespace facebook;

FBHack::FBHack()
{
}

FBHack::~FBHack()
{

}

void FBHack::help()
{
    qDebug() << "Facebook OpenGraph Shell Script";
    qDebug() << "Usage:" << QApplication::arguments().at(0) << "[apiKey]" << "[baseUrl]" << "[username]" << "[password]" << "[graphobj]";
    qDebug() << "\tapiKey" << "\t\t-\t" << "Your Facebook App's Api Key";
    qDebug() << "\tbaseUrl" << "\t-\t" << "Your Facebook App's Base Domain Url";
    qDebug() << "\tusername" << "\t-\t" << "The user account that you wish to authenticate";
    qDebug() << "\tpassword" << "\t-\t" << "The password for the above user account";
    qDebug() << "\tgraphobj" << "\t-\t" << "The opengraph object to return";
}

void FBHack::main()
{
    if (QApplication::arguments().count() < 6) {
        help();
        return;
    }

    app::Settings settings;
    settings.apiKey = QApplication::arguments().at(1);
    settings.baseUrl = QApplication::arguments().at(2);

    auth::Credentials credentials;
    credentials.login = QApplication::arguments().at(3);
    credentials.pass = QApplication::arguments().at(4);

    const QString object(QApplication::arguments().at(5));

    Client session;
    session.login(settings, credentials);
#ifdef VERBOSE_OUTPUT
    qDebug() << "Auth Token: " << session.token().value;
#endif

    qDebug() << session.get(object);
}
