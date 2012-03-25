#include "credentials.hpp"

namespace facebook {
namespace auth {

Credentials::Credentials() {

}

Credentials::Credentials(const std::string &username, const std::string &password)
{
    setUsername(username);
    setPassword(password);
}

Credentials::~Credentials() {

}

const std::string & Credentials::username() const {
    return login;
}
void Credentials::setUsername(const std::string &username) {
    login = username;
}
void Credentials::resetUsername() {
    login.clear();
}

const std::string & Credentials::password() const {
    return pass;
}
void Credentials::setPassword(const std::string &password) {
    pass = password;
}
void Credentials::resetPassword() {
    pass.clear();
}

} /* namespace auth */
} /* namespace facebook */

