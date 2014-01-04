#ifndef LIBFACEBOOK_TOKENDATA_HPP
#define LIBFACEBOOK_TOKENDATA_HPP

#include <QSharedData>

#include <QDateTime>
#include <QString>

namespace facebook {

class TokenData : public QSharedData {
public:
    TokenData();
    TokenData(const QString & value);
    TokenData(const QString & value, const QDateTime & expiry);
    virtual ~TokenData();

    bool isExpired() const;
    bool isValid() const;

    void resetValue();
    void resetExpiry();

    void clear();

    QString value;
    QDateTime expiry;
};

inline TokenData::TokenData() {}

inline TokenData::TokenData(const QString & value) : value(value) {}

inline TokenData::TokenData(const QString & value, const QDateTime & expiry) :
    value(value),
    expiry(expiry) {

}

inline TokenData::~TokenData() {}

inline bool TokenData::isExpired() const {
    return !expiry.isNull() && expiry <= QDateTime::currentDateTime();
}

inline bool TokenData::isValid() const {
    return !value.isEmpty() && !isExpired();
}

inline void TokenData::resetValue() {
    value = QString();
}

inline void TokenData::resetExpiry() {
    expiry = QDateTime();
}

inline void TokenData::clear() {
    resetValue();
    resetExpiry();
}

} /* namespace facebook */

#endif // LIBFACEBOOK_TOKENDATA_HPP
