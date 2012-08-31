#include "fbhack.hpp"

#include <QApplication>

int main(int argc, char * argv[]) {

    QApplication app(argc, argv);

    FBHack hack;
    hack.init(app);

    return app.exec();
}
