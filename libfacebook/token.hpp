#ifndef LIBFACEBOOK_TOKEN_HPP
#define LIBFACEBOOK_TOKEN_HPP

#include <QSharedDataPointer>

#include <QDateTime>
#include <QString>

namespace facebook {

class TokenData;

class Token {
public:
    Token();
    Token(const QString & value);
    Token(const QString & value, const QDateTime & expiry);
    Token(const Token & other);
    Token & operator=(const Token & other);
    virtual ~Token();

    bool isExpired() const;
    bool isValid() const;

    const QString & value() const;
    void setValue(const QString & value);
    void resetValue();

    const QDateTime & expiry() const;
    void setExpiry(const QDateTime & expiry);
    void resetExpiry();

    void clear();

private:
    QSharedDataPointer<TokenData> d_ptr;
};

} /* namespace facebook */

#endif // LIBFACEBOOK_TOKEN_HPP
