#include "client.hpp"

#include "clientprivate.hpp"

namespace facebook {

Client::Client() :
    d_ptr(new ClientPrivate()) {

}

Client::~Client() {

}

bool Client::login(const twirl::Login & login) {
    return d_ptr->login(login);
}

bool Client::logout() {
    return d_ptr->logout();
}

Token Client::acquireToken(const App & app, const QStringList & permissions) {
    return d_ptr->acquireToken(app, permissions);
}

QVariantMap Client::get(const QString & object) {
    return d_ptr->get(object);
}

QVariantMap Client::post(const QString & object, const QVariantMap & data) {
    return d_ptr->post(object, data);
}

QVariantMap Client::post(const QString & object, const QByteArray & data) {
    return d_ptr->post(object, data);
}

QVariantMap Client::del(const QString & object) {
    return d_ptr->del(object);
}

const Token & Client::token() const {
    return d_ptr->token;
}

void Client::setToken(const Token & token) {
    d_ptr->token = token;
}

bool Client::load(twirl::Session & session) {
    return d_ptr->load(session);
}
bool Client::save(twirl::Session & session) {
    return d_ptr->save(session);
}

} /* namespace facebook */
