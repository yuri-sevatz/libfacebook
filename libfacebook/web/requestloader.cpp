#include "requestloader.hpp"

#include <QCoreApplication>

#include <QtNetwork/QNetworkReply>

namespace web {

RequestLoader::RequestLoader() {

}

RequestLoader::~RequestLoader() {

}

QNetworkReply * RequestLoader::load(QNetworkReply * reply) {
    if (reply) {
        while(reply->isRunning()) {
            QCoreApplication::processEvents();
        }
    }

    return reply;
}

}