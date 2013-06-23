#include "app.hpp"
#include "appdata.hpp"

namespace facebook {

App::App() : d_ptr(new AppData()) {}

App::App(const QString & apiKey, const QString & baseUrl) : d_ptr(new AppData(apiKey, baseUrl)) {}

App::App(const App & other) : d_ptr(other.d_ptr) {}

App & App::operator=(const App & other) {
    d_ptr = other.d_ptr;
    return *this;
}

App::~App() {}

const QString & App::apiKey() const {
    return d_ptr->apiKey;
}

void App::setApiKey(const QString & apiKey) {
    d_ptr->apiKey = apiKey;
}

void App::resetApiKey() {
    d_ptr->apiKey.clear();
}

const QString & App::baseUrl() const {
    return d_ptr->baseUrl;
}

void App::setBaseUrl(const QString & baseUrl) {
    d_ptr->baseUrl = baseUrl;
}

void App::resetBaseUrl() {
    d_ptr->baseUrl.clear();
}

} /* namespace facebook */
