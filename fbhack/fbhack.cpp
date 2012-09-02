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
    LOGOUT,
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
    qDebug() << "\t-m method" << "\t" << "{login, logout, get, post, delete} - (Default: login)";
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
            } else if (methodStr.compare("logout", Qt::CaseInsensitive) == 0) {
                method = LOGOUT;
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

    switch(method) {
    case GET:
    case POST:
    case DELETE:
        if (object.isEmpty()) {
            qDebug() << "Please supply a graphObj (-o)";
            return;
        }
    case LOGIN:
    case LOGOUT:
        if (session.token().value.isEmpty()) {
            force("Password: ", credentials.pass);
            session.login(settings, credentials);
        }
    }

    if (method == LOGOUT) {
        session.logout(settings);
    }

    switch(method) {
    case LOGIN:
        qtout << "Token: " << session.token().value << "\n";
        break;
    case LOGOUT:
        qtout << "Logout Complete\n";
        break;
    case GET:
        printValue(session.get(object));
        break;
    case POST:
        printValue(session.post(object, data));
        break;
    case DELETE:
        printValue(session.del(object));
        break;
    }
    qtout << "\n";
    qtout.flush();
}

void FBHack::force(const QString & param, QString & target) {
    while (target.isEmpty()) {
        qtout << param;
        qtout.flush();
        target = qtin.readLine();
    }
}

void FBHack::indent(size_t indent) {
    for (size_t i = 0; i != indent; i++) {
        qtout << "    ";
    }
}

void FBHack::printList(const QVariantList & list, size_t depth) {
    qtout << "[\n";
    const size_t bodyDepth = depth + 1;
    for(QVariantList::const_iterator iter = list.constBegin(); iter != list.constEnd(); ++iter) {
        indent(bodyDepth);
        printValue(*iter, bodyDepth);
        qtout << ",\n";
    }
    indent(depth);
    qtout << "]";
}

void FBHack::printMap(const QVariantMap & map, size_t depth) {
    qtout << "{\n";
    const size_t bodyDepth = depth + 1;
    for(QVariantMap::const_iterator iter = map.constBegin(); iter != map.constEnd(); ++iter) {
        indent(bodyDepth);
        qtout << iter.key() << ": ";
        printValue(*iter, bodyDepth);
        qtout << "\n";
    }
    indent(depth);
    qtout << "}";
}

void FBHack::printValue(const QVariant & value, size_t depth) {
    switch(value.type()) {
    case QVariant::Map:
        printMap(value.toMap(), depth);
        break;
    case QVariant::List:
        printList(value.toList(), depth);
        break;
    default:
        qtout << value.toString();
    }
}
