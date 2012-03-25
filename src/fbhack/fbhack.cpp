#include "fbhack.hpp"

#include <libfacebook/auth/token.hpp>

#include <QDebug>
#include <QApplication>
#include <QStringList>

FBHack::FBHack()
{
}

FBHack::~FBHack() {

}

void FBHack::help() {
    qDebug() << "Facebook OpenGraph Scripted Login";
    qDebug() << "Usage:" << QApplication::arguments().at(0) << "[apiKey]" << "[baseUrl]" << "[username]" << "[password]";
    qDebug() << "\tapiKey" << "\t\t-\t" << "Your Facebook App's Api Key";
    qDebug() << "\tbaseUrl" << "\t-\t" << "Your Facebook App's Base Domain Url";
    qDebug() << "\tusername" << "\t-\t" << "The user account that you wish to authenticate with over opengraph";
    qDebug() << "\tpassword" << "\t-\t" << "The password for the above user account";
}

void FBHack::main() {
    qDebug() << "Main entered";

    if (QApplication::arguments().count() < 5) {
        help();
        return;
    }

    settings.apiKey = QApplication::arguments().at(1).toStdString();
    settings.baseUrl = QApplication::arguments().at(2).toStdString();

    credentials.setUsername(QApplication::arguments().at(3).toStdString());
    credentials.setPassword(QApplication::arguments().at(4).toStdString());

    session.setAppSettings(&settings);
    session.setCredentials(&credentials);

    qDebug() << "Starting Login";

    session.login();

    //std::cout << "Auth Token: " << session.token().value();

    qDebug() << "Main Exiting!";
}
