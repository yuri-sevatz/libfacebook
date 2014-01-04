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

QJsonDocument Client::get(const QString & object, const QMap<QString,QString> & params) {
    return d_ptr->get(object, params);
}

QJsonDocument Client::post(const QString & object, const QMap<QString,QString> & params, const QByteArray & data) {
    return d_ptr->post(object, params, data);
}

QJsonDocument Client::del(const QString & object, const QMap<QString,QString> & params) {
    return d_ptr->del(object, params);
}

const Token & Client::token() const {
    return d_ptr->token;
}

void Client::setToken(const Token & token) {
    d_ptr->token = token;
}

void Client::resetToken() {
    d_ptr->token.clear();
}

bool Client::load(twirl::Session & session) {
    return d_ptr->load(session);
}
bool Client::save(twirl::Session & session) {
    return d_ptr->save(session);
}

} /* namespace facebook */
