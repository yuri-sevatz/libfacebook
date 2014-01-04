#ifndef LIBFACEBOOK_FACEBOOKPAGE_HPP
#define LIBFACEBOOK_FACEBOOKPAGE_HPP

#include <libfacebook/app.hpp>

#include <QWebPage>
#include <QUrl>

namespace facebook {

class FacebookPage : public QWebPage {
    Q_OBJECT
public:
    FacebookPage();
    virtual ~FacebookPage();

    // All Navigations to a target's resource are stopped with a 404.
    const QUrl & target() const;
    void setTarget(const QUrl & res);
    void resetTarget();

    const QUrl & address() const;

    bool viewing(const App & app);
    bool viewing(const QUrl & url);

    static const QUrl & authUrl();
    static const QUrl & graphUrl();
    static const QUrl & loginUrl();
    static const QUrl & logoutUrl();
    static const QUrl & mainUrl();

protected:
    virtual bool acceptNavigationRequest(QWebFrame * frame, const QNetworkRequest & request, NavigationType type);

private:
    static QUrl::FormattingOptions uniqueUrl();

    static bool resourceOpen(const QUrl & res, const QUrl & url);
    static QUrl resource(const QUrl & url);

    QUrl requestUrl;
    QUrl targetUrl;
};

inline const QUrl & FacebookPage::address() const {
    return requestUrl;
}

inline const QUrl & FacebookPage::target() const {
    return targetUrl;
}

inline void FacebookPage::setTarget(const QUrl & res) {
    targetUrl = resource(res);
}

inline void FacebookPage::resetTarget() {
    targetUrl.clear();
}

}

#endif // LIBFACEBOOK_FACEBOOKPAGE_HPP
