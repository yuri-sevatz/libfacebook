#include "fbhack.hpp"

#include <QDebug>
#include <QApplication>
#include <QRegExp>

#include <libfacebook/include/config.h>

#include <libfacebook/client.hpp>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

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
    qterr(stderr),
    qtin(stdin) {
}

FBHack::~FBHack() {

}

void FBHack::help(const QStringList & args) {
    qDebug() << "Facebook OpenGraph Shell Script";
    qDebug() << "Usage:" << args.value(0);
    qDebug() << "\t-m method" << "\t" << "{login, logout, get, post, delete} - (Default: login)";
    qDebug() << "";
    qDebug() << "\t-u username" << "\t" << "Account  - (Required if no token/session)";
    qDebug() << "\t-p password" << "\t" << "Password - (Required if no token/session; omit = stdin)";
    qDebug() << "\t-b baseUrl" << "\t" << "App Domain Base Url - (Required if no token)";
    qDebug() << "\t-k apiKey" << "\t" << "App Api Key - (Required if no token)";
    qDebug() << "";
    qDebug() << "\t-t token" << "\t" << "An Access Token from a previous session - (Optional)";
    qDebug() << "\t-s session" << "\t" << "Path to save client session to skip login - (Optional)";
    qDebug() << "";
    qDebug() << "\t-o graphObj" << "\t" << "Graph Object to return - (Required if method != LOGIN)";
    qDebug() << "\t-r permissions" << "\t" << "A comma-delimited list of Permissions - (Optional)";
    qDebug() << "\t-a arg" << "\t" << "Append a key=value argument to the API call - (Optional)";
    //qDebug() << "\t-d postData" << "\t" << "Data to bundle with a POST request - (Optional)";
    qDebug() << "";
    qDebug() << "\t-h" << "\t\t" << "Show this help message";
    qDebug() << "";
    qDebug() << "Examples:";
    qDebug() << "\t" << args.value(0) << "-m get -u me@example.com -p 12345pass -b localhost.localdomain -k 123456789012345 -r user_birthday,user_photos -o me";
    qDebug() << "\t" << args.value(0) << "-m get -t 7NadXi8PGb3t7ESkq1YkSV6HIXNGWxCQyQfwO8PTaI21RrLRE6EAgmYOXdspz8TKvzNt7HDuTRvncDFDYmuDvwYFv3ctvSyOZu9pjlnaYlSJyVns -o me";
}

void FBHack::main() {
    App app;
    QStringList permissions;
    twirl::Login login;
    QString object;
    Client client;
    twirl::Session session;
    Method method = LOGIN;
    QMap<QString,QString> params;

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
            login.setUsername(args.value(++argi));
        } else if (arg == "-p") {
            login.setPassword(args.value(++argi));
        } else if (arg == "-b") {
            app.setBaseUrl(args.value(++argi));
        } else if (arg == "-k") {
            app.setApiKey(args.value(++argi));
        } else if (arg == "-r") {
            permissions = args.value(++argi).split(QRegExp("[\\s,]+"), QString::SkipEmptyParts);
        } else if (arg == "-o") {
            object = args.value(++argi);
        } else if (arg == "-a") {
            const QRegExp regex("([^=]+)=?([^=].*)");
            if (regex.exactMatch(args.value(++argi))) {
                params.insert(regex.cap(1), regex.cap(2));
            }
        } else if (arg == "-t") {
            client.setToken(Token(args.value(++argi)));
        } else if (arg == "-s") {
            session.setPath(args.value(++argi));
        } else if (arg == "-h" || arg == "-help" || arg == "--help" || arg == "/?") {
            help(args);
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
        // If the user posseses the means to login with a facebook account
        if (!login.username().isEmpty() || !session.isNull()) {
            // Require graph api settings, since we are performing user login
            if (app.baseUrl().isEmpty()) {
                qDebug() << "Please supply a baseUrl (-b)";
                return;
            } else if (app.apiKey().isEmpty()) {
                qDebug() << "Please supply an apiKey (-k)";
                return;
            }

            // If the user has no username
            if (login.username().isEmpty()) {
                // Attempt to load a session
                if (!client.load(session)) {
                    qDebug() << "Error loading session";
                    return;
                }
            } else {
                // Require a password from the user
                if (login.password().isEmpty()) {
                    login.setPassword(require("Password: "));
                }
                if (client.login(login)) {
                    qDebug() << "Logged In";
                } else {
                    qDebug() << "Error Logging In";
                    return;
                }

                // Save the session, if any
                if (!session.isNull() && !client.save(session)) {
                    qDebug() << "Error writing session";
                    return;
                }
            }

            // Acquire a token
            const Token token = client.acquireToken(app, permissions);
            if (token.isValid()) {
                client.setToken(token);
            } else {
                qDebug() << "Error Acquiring Token";
                return;
            }
        } else if (client.token().value().isEmpty()) {
            qDebug() << "Please supply a username (-u), session (-s) or token(-t)";
            return;
        }
        break;
    case LOGOUT:
        qDebug() << (client.logout() ? "Logout Complete" : "Error Logging Out");
        if (!session.isNull() && !client.save(session)) {
            qDebug() << "Error clearing session";
            return;
        }
        return;
    }

    switch(method) {
    case LOGIN:
        qtout << client.token().value();
        break;
    case GET:
        printDocument(client.get(object, params));
        break;
    case POST:
        printDocument(client.post(object, params));
        break;
    case DELETE:
        printDocument(client.del(object, params));
        break;
    }
    qtout << "\n";
    qtout.flush();
}

inline QString FBHack::require(const QString & param) {
    QString target;
    require(param, target);
    return target;
}

void FBHack::require(const QString & param, QString & target) {
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

void FBHack::printDocument(const QJsonDocument & json) {
    if (json.isObject()) {
        printObject(json.object(), 0);
    } else if (json.isArray()) {
        printArray(json.array(), 0);
    }
}

void FBHack::printArray(const QJsonArray & array, size_t depth) {
    qtout << "[\n";
    const size_t bodyDepth = depth + 1;
    for(QJsonArray::const_iterator iter = array.constBegin(); iter != array.constEnd(); ++iter) {
        indent(bodyDepth);
        printValue(*iter, bodyDepth);
        qtout << ",\n";
    }
    indent(depth);
    qtout << "]";
}

void FBHack::printObject(const QJsonObject & object, size_t depth) {
    qtout << "{\n";
    const size_t bodyDepth = depth + 1;
    for(QJsonObject::const_iterator iter = object.constBegin(); iter != object.constEnd(); ++iter) {
        indent(bodyDepth);
        qtout << iter.key() << ": ";
        printValue(*iter, bodyDepth);
        qtout << "\n";
    }
    indent(depth);
    qtout << "}";
}

void FBHack::printValue(const QJsonValue & value, size_t depth) {
    switch(value.type()) {
    case QJsonValue::Bool:
        qtout << value.toBool();
        break;
    case QJsonValue::Double:
        qtout << value.toDouble();
        break;
    case QJsonValue::String:
        qtout << value.toString();
        break;
    case QJsonValue::Array:
        printArray(value.toArray(), depth);
        break;
    case QJsonValue::Object:
        printObject(value.toObject(), depth);
        break;
    case QJsonValue::Null:
    case QJsonValue::Undefined:
    default:
        break;
    }
}

