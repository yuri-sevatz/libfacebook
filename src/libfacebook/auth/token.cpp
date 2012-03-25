#include "token.hpp"

namespace facebook {
namespace auth {

Token::Token() {
}

Token::Token(const Token & copy) {
    *this = copy;
}

Token::~Token() {

}

bool Token::isEmpty() const {
    return key.empty();
}

const std::string & Token::value() const {
    return key;
}

void Token::setValue(const std::string &value) {
    key = value;
}

void Token::resetValue() {
    key.clear();
}

Token & Token::operator =(const Token & rhs) {
    setValue(rhs.value());
}

} /* namespace auth */
} /* namespace facebook */
