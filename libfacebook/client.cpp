#include "client.hpp"

#include "clientprivate.hpp"

namespace facebook {

Client::Client() :
    d_ptr(new ClientPrivate())
{

}

Client::~Client()
{

}

void Client::login(const app::Settings & settings, const auth::Credentials & credentials)
{
    return d_ptr->login(settings, credentials);
}

QVariantMap Client::get(const QString & object)
{
    return d_ptr->get(object);
}

const auth::Token & Client::token() const
{
    return d_ptr->token();
}

} /* namespace facebook */
