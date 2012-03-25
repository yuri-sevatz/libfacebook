#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace facebook {
namespace auth {

class Token
{
public:
    Token();
    Token(const Token & token);
    virtual ~Token();

    bool isEmpty() const;

    const std::string & value() const;
    void setValue(const std::string & value);
    void resetValue();

    Token & operator =(const Token & rhs);

private:
    std::string key;
};

} /* namespace auth */
} /* namespace facebook */

#endif // TOKEN_HPP
