#include "bootstrap.hpp"

#include <QApplication>
#include <QTimer>

Bootstrap::Bootstrap() {
}

Bootstrap::~Bootstrap() {

}

void Bootstrap::init(QApplication & app) {

    QTimer::singleShot(0, this, SLOT(run()));
    QObject::connect(this, SIGNAL(quit()), &app, SLOT(quit()));
}

void Bootstrap::run() {
    main();
    quit();
}
