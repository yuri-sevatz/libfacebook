#include "pageloader.hpp"

#include <QCoreApplication>
#include <QWebFrame>
#include <QWebPage>

namespace web {

PageLoader::PageLoader(QWebPage & page) :
    page(page)
{
    QObject::connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(onLoad(bool)));
}

PageLoader::~PageLoader() {}

bool PageLoader::load() {
    state = IDLE;
    do {
        QCoreApplication::processEvents();
    } while(state == IDLE);

    return state == DONE;
}

bool PageLoader::load(const QUrl & url) {
    page.mainFrame()->load(url);
    return load();
}

void PageLoader::onLoad(bool success) {
    state = success ? DONE : FAIL;
}

}