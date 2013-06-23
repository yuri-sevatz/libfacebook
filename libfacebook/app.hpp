#ifndef LIBFACEBOOK_APP_HPP
#define LIBFACEBOOK_APP_HPP

#include <QSharedDataPointer>

#include <QString>

namespace facebook {

class AppData;

class App {
public:
    App();
    App(const QString & apiKey, const QString & baseUrl);
    App(const App & other);
    App & operator=(const App & other);
    virtual ~App();

    const QString & apiKey() const;
    void setApiKey(const QString & apiKey);
    void resetApiKey();

    const QString & baseUrl() const;
    void setBaseUrl(const QString & baseUrl);
    void resetBaseUrl();

private:
    QSharedDataPointer<AppData> d_ptr;
};

} /* namespace facebook */

#endif // LIBFACEBOOK_APP_HPP
