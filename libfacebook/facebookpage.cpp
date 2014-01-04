#include "facebookpage.hpp"

#include <libfacebook/include/config.h>

#ifdef VERBOSE_OUTPUT
#include <QDebug>
#endif

#include <QtNetwork/QNetworkRequest>
#include <QWebFrame>

namespace facebook {

FacebookPage::FacebookPage() {}

FacebookPage::~FacebookPage() {}

bool FacebookPage::acceptNavigationRequest(QWebFrame * frame, const QNetworkRequest & request, NavigationType type) {
    if (frame == mainFrame()) {
        requestUrl = request.url();
#ifdef VERBOSE_OUTPUT
        qDebug() << "<" << requestUrl << ">";
#endif
        if(targetUrl == resource(requestUrl)) {
            resetTarget();
            return false;
        }
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}

const QUrl & FacebookPage::authUrl() {
    static const QUrl auth("https://www.facebook.com/dialog/oauth");
    return auth;
}

const QUrl & FacebookPage::graphUrl() {
    static const QUrl graph("https://graph.facebook.com");
    return graph;
}

const QUrl & FacebookPage::loginUrl() {
    static const QUrl login("https://www.facebook.com/login.php");
    return login;
}

const QUrl & FacebookPage::logoutUrl() {
    static const QUrl logout("https://www.facebook.com/logout.php");
    return logout;
}

const QUrl & FacebookPage::mainUrl() {
    static const QUrl main("https://www.facebook.com");
    return main;
}

bool FacebookPage::viewing(const App & app) {
    return resourceOpen(resource(app.baseUrl()), requestUrl);
}

bool FacebookPage::viewing(const QUrl & url) {
    return resourceOpen(url, mainFrame()->url());
}

inline bool FacebookPage::resourceOpen(const QUrl & res, const QUrl & url) {
    const bool ret = res == resource(url);
#ifdef VERBOSE_OUTPUT
    if (ret) {
        qDebug() << "[" << res << "]";
    }
#endif
    return ret;
}

QUrl FacebookPage::resource(const QUrl & url) {
    QUrl res = url.adjusted(QUrl::RemoveQuery | QUrl::RemoveFragment | QUrl::StripTrailingSlash);
    // Workaround: Qt has an issue with StripTrailingSlash for paths of length 1.  Might be fixed in 5.2.x or 5.3 ?
    if (res.path(QUrl::FullyEncoded) == QChar('/')) {
        res.setPath(QString());
    }
    return res;
}

}