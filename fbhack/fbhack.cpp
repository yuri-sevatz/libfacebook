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

enum Method {
    LOGIN,
    GET,
    POST,
    DELETE
};

FBHack::FBHack() :
    qtout(stdout),
    qtin(stdin) {
}

FBHack::~FBHack() {

}

void FBHack::help(const QStringList & args) {
    qDebug() << "Facebook OpenGraph Shell Script";
    qDebug() << "Usage:" << args.value(0);
    qDebug() << "\t-m method" << "\t" << "One of {login, get, post, delete} - (Default is login)";
    qDebug() << "";
    qDebug() << "\t-u username" << "\t" << "Account  - (Required if no accessToken)";
    qDebug() << "\t-p password" << "\t" << "Password - (Required if no accessToken; omit = stdin)";
    qDebug() << "\t-b baseUrl" << "\t" << "App Domain Base Url - (Required if no accessToken)";
    qDebug() << "\t-k apiKey" << "\t" << "App Api Key - (Required if no accessToken)";
    qDebug() << "";
    qDebug() << "\t-t accessToken" << "\t" << "An Access Token from a previous session - (Optional)";
    qDebug() << "";
    qDebug() << "\t-o graphObj" << "\t" << "Graph Object to return - (Required if method != LOGIN)";
    qDebug() << "\t-r permissions" << "\t" << "A comma-delimited list of Permissions - (Optional)";
    qDebug() << "\t-d postData" << "\t" << "Data to bundle with a POST request - (Optional)";
    qDebug() << "";
    qDebug() << "\t-h" << "\t\t" << "Show this help message";
    qDebug() << "";
    qDebug() << "Examples:";
    qDebug() << "\t" << args.value(0) << "-m get -u me@example.com -p 12345pass -b localhost.localdomain -k 123456789012345 -r user_birthday,user_photos -o me";
    qDebug() << "\t" << args.value(0) << "-m get -t 7NadXi8PGb3t7ESkq1YkSV6HIXNGWxCQyQfwO8PTaI21RrLRE6EAgmYOXdspz8TKvzNt7HDuTRvncDFDYmuDvwYFv3ctvSyOZu9pjlnaYlSJyVns -o me";
}

void FBHack::main() {
    app::Settings settings;
    auth::Credentials credentials;
    QString object;
    Client session;
    Method method = LOGIN;
    QByteArray data;

    const QStringList args = QApplication::arguments();

    if (args.size() < 2) {
        help(args);
        return;
    }

    for(int argi = 0; argi < args.size(); argi++) {
        const QString arg = args.value(argi);
        if (arg == "-m") {
            const QString methodStr = args.value(++argi);
            if (methodStr.compare("login", Qt::CaseInsensitive) == 0) {
                method = LOGIN;
            } else if (methodStr.compare("get", Qt::CaseInsensitive) == 0) {
                method = GET;
            } else if (methodStr.compare("post", Qt::CaseInsensitive) == 0) {
                method = POST;
            } else if (methodStr.compare("delete", Qt::CaseInsensitive) == 0) {
                method = DELETE;
            }
        } else if (arg == "-u") {
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
        } else if (arg == "-d") {
            data = args.value(++argi).toUtf8();
        } else if (arg == "-t") {
            session.token().value = args.value(++argi);
        } else if (arg == "-h" || arg == "-help" || arg == "--help" || arg == "/?") {
            help(args);
            return;
        }
    }

    if (session.token().value.isEmpty()) {
        if (credentials.login.isEmpty()) {
            qDebug() << "Please supply a username (-u)";
            return;
        } else if (settings.baseUrl.isEmpty()) {
            qDebug() << "Please supply a baseUrl (-b)";
            return;
        } else if (settings.apiKey.isEmpty()) {
            qDebug() << "Please supply an apiKey (-k)";
            return;
        }
    }

    if (method != LOGIN) {
        if (object.isEmpty()) {
            qDebug() << "Please supply a graphObj (-o)";
            return;
        }
    }

    if (session.token().value.isEmpty()) {
        force("Password: ", credentials.pass);
        session.login(settings, credentials);
    }

    switch(method) {
    case LOGIN:
        qDebug() << "Token:" << session.token().value;
        break;
    case GET:
        qDebug() << session.get(object);
        break;
    case POST:
        qDebug() << session.post(object, data);
        break;
    case DELETE:
        qDebug() << session.del(object);
        break;
    }
}

void FBHack::force(const QString & param, QString & target) {
    while (target.isEmpty()) {
        qtout << param;
        qtout.flush();
        target = qtin.readLine();
    }
}
