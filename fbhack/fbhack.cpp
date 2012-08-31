#include "fbhack.hpp"

#include <QDebug>
#include <QApplication>
#include <QRegExp>

#include <libfacebook/include/config.h>

#include <libfacebook/app/settings.hpp>
#include <libfacebook/auth/credentials.hpp>
#include <libfacebook/auth/token.hpp>
#include <libfacebook/client.hpp>

using namespace facebook;

FBHack::FBHack() :
    qtout(stdout),
    qtin(stdin) {
}

FBHack::~FBHack() {

}

void FBHack::help(const QStringList & args) {
    qDebug() << "Facebook OpenGraph Shell Script";
    qDebug() << "Usage:" << args.value(0);
    qDebug() << "\t-u username" << "\t" << "The user account that you wish to authenticate";
    qDebug() << "\t-p password" << "\t" << "The password for the above user account";
    qDebug() << "\t-b baseUrl" << "\t" << "Your Facebook App's Base Domain Url";
    qDebug() << "\t-k apiKey" << "\t" << "Your Facebook App's Api Key";
    qDebug() << "\t-r permissions" << "\t" << "A comma-delimited list of opengraph permissions";
    qDebug() << "\t-o graphObj" << "\t" << "The opengraph object to return";
    qDebug() << "\t-h" << "\t\t" << "Show this help message";
    qDebug();
    qDebug() << "Example:";
    qDebug() << "\t" << args.value(0) << "-u me@example.com -p 12345pass -b localhost.localdomain -k 123456789012345 -r user_birthday,user_photos -o me";
}

void FBHack::main() {
    app::Settings settings;
    auth::Credentials credentials;
    QString object;

    const QStringList args = QApplication::arguments();

    for(int argi = 0; argi < args.size(); argi++) {
        const QString arg = args.value(argi);
        if (arg == "-u") {
            credentials.login = args.value(++argi);
        } else if (arg == "-p") {
            credentials.pass = args.value(++argi);
        } else if (arg == "-b") {
            settings.baseUrl = args.value(++argi);
        } else if (arg == "-k") {
            settings.apiKey = args.value(++argi);
        } else if (arg == "-r") {
            settings.permissions = args.value(++argi).split(QRegExp("[\\s,]+"), QString::SkipEmptyParts);
        } else if (arg == "-o") {
            object = args.value(++argi);
        } else if (arg == "-h" || arg == "-help" || arg == "--help" || arg == "/?") {
            help(args);
            return;
        }
    }

    force("Username: ", credentials.login);
    force("Password: ", credentials.pass);
    force("BaseUrl: ", settings.baseUrl);
    force("ApiKey: ", settings.apiKey);
    force("GraphObj: ", object);

    Client session;
    session.login(settings, credentials);

#ifdef VERBOSE_OUTPUT
    qDebug() << "Auth Token: " << session.token().value;
#endif

    qDebug() << session.get(object);
}

void FBHack::force(const QString & param, QString & target) {
    while (target.isEmpty()) {
        qtout << param;
        qtout.flush();
        target = qtin.readLine();
    }
}
