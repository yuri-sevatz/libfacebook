#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

#include <string>

namespace facebook {
namespace auth {

class Credentials
{
public:
    Credentials();
    Credentials(const std::string & username, const std::string & password);
    virtual ~Credentials();

    const std::string & username() const;
    void setUsername(const std::string & username);
    void resetUsername();

    const std::string & password() const;
    void setPassword(const std::string & password);
    void resetPassword();

private:
    std::string login;
    std::string pass;
};

} /* namespace auth */
} /* namespace facebook */

#endif // CREDENTIALS_HPP
