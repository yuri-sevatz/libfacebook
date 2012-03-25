#include "fbhack.hpp"

int main(int argc, char * argv[]) {

    QApplication app(argc, argv);

    FBHack hack;
    hack.init(app);

    return app.exec();
}
