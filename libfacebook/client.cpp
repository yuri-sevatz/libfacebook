#include "client.hpp"

#include "clientprivate.hpp"

namespace facebook {

Client::Client() :
    d_ptr(new ClientPrivate()) {

}

Client::~Client() {

}

void Client::login(const app::Settings & settings, const auth::Credentials & credentials) {
    return d_ptr->login(settings, credentials);
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

auth::Token & Client::token() {
    return d_ptr->token();
}

const auth::Token & Client::token() const {
    return d_ptr->token();
}

} /* namespace facebook */
