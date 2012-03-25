#ifndef FBHACK_HPP
#define FBHACK_HPP

#include "bootstrap.hpp"

#include <libfacebook/app/settings.hpp>
#include <libfacebook/auth/credentials.hpp>
#include <libfacebook/auth/session.hpp>

class FBHack : public Bootstrap
{
public:
    FBHack();
    virtual ~FBHack();

protected:
    void main();

private:
    void help();

    facebook::app::Settings settings;
    facebook::auth::Credentials credentials;
    facebook::auth::Session session;

};

#endif // FBHACK_HPP
