#include "session.hpp"

#include "sessionprivate.hpp"
#include <libfacebook/app/settings.hpp>
#include "credentials.hpp"

#include <QEventLoop>

namespace facebook {
namespace auth {

Session::Session() :
        d_ptr(new SessionPrivate())
{

}

Session::~Session() {
    delete d_ptr;
}

const app::Settings * Session::appSettings() const {
    return d_ptr->settings;
}
void Session::setAppSettings(const app::Settings * settings) {
    d_ptr->settings = settings;
}
void Session::resetAppSettings() {
    setAppSettings(0);
}

const Credentials * Session::credentials() const {
    return d_ptr->credentials;
}
void Session::setCredentials(const Credentials * credentials) {
    d_ptr->credentials = credentials;
}

void Session::resetCredentials() {
    setCredentials(0);
}

void Session::login() {
    d_ptr->login();
}

const Token & Session::token() const {
    return d_ptr->token();
}

} /* namespace auth */
} /* namespace facebook */
