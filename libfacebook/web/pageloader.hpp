#ifndef WEB_PAGELOADER_HPP
#define WEB_PAGELOADER_HPP

#include <QObject>

class QUrl;
class QWebPage;

namespace web {

class PageLoader : public QObject {
    Q_OBJECT
public:
    PageLoader(QWebPage & page);
    virtual ~PageLoader();

    // Synchronously load a target into a page using a blocking call.
    // Runs the application event loop.
    bool load();
    bool load(const QUrl & url);

private:
    PageLoader();
    Q_DISABLE_COPY(PageLoader);
    Q_SLOT void onLoad(bool success);

    enum State {
        IDLE,
        FAIL,
        DONE,
    } state;

    QWebPage & page;
};

}

#endif // WEB_PAGELOADER_HPP
