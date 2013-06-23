#ifndef LIBFACEBOOK_APPDATA_HPP
#define LIBFACEBOOK_APPDATA_HPP

#include <QSharedData>

#include <QString>

namespace facebook {

class AppData : public QSharedData {
public:
    AppData();
    AppData(const QString & apiKey, const QString & baseUrl);
    virtual ~AppData();

    QString apiKey;
    QString baseUrl;
};

inline AppData::AppData() {}

inline AppData::AppData(const QString & apiKey, const QString & baseUrl) :
    apiKey(apiKey),
    baseUrl(baseUrl) {

}

inline AppData::~AppData() {}

}

#endif // LIBFACEBOOK_APPDATA_HPP
