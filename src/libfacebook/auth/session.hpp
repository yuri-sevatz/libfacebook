#ifndef LOGIN_HPP
#define SESSION_HPP

#include "token.hpp"

namespace facebook {

namespace app {
    class Settings;
}

namespace auth {

class Credentials;
class SessionPrivate;

class Session
{
public:
    Session();
    virtual ~Session();

    const app::Settings * appSettings() const;
    void setAppSettings(const app::Settings * settings);
    void resetAppSettings();

    const Credentials * credentials() const;
    void setCredentials(const Credentials * credentials);
    void resetCredentials();

    // Synchronous login...
    void login();
    const Token & token() const;

private:
    friend class SessionPrivate;
    SessionPrivate * const d_ptr;
};

} /* namespace auth */
} /* namespace facebook */

#endif // LOGIN_HPP
