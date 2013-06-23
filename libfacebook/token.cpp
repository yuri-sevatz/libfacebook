#include "token.hpp"
#include "tokendata.hpp"

namespace facebook {

Token::Token() : d_ptr(new TokenData()) {}

Token::Token(const QString & value) : d_ptr(new TokenData(value)) {}

Token::Token(const QString & value, const QDateTime & expiry) : d_ptr(new TokenData(value, expiry)) {}

Token::Token(const Token & other) : d_ptr(other.d_ptr) {}

Token & Token::operator=(const Token & other) {
    d_ptr = other.d_ptr;
    return *this;
}

Token::~Token() {}

bool Token::isExpired() const {
    return d_ptr->isExpired();
}

bool Token::isValid() const {
    return d_ptr->isValid();
}

const QString & Token::value() const {
    return d_ptr->value;
}

void Token::setValue(const QString & value) {
    d_ptr->value = value;
}

void Token::resetValue() {
    d_ptr->resetValue();
}

const QDateTime & Token::expiry() const {
    return d_ptr->expiry;
}

void Token::setExpiry(const QDateTime & expiry) {
    d_ptr->expiry = expiry;
}

void Token::resetExpiry() {
    d_ptr->resetExpiry();
}

void Token::clear() {
    d_ptr->clear();
}

} /* namespace facebook */
