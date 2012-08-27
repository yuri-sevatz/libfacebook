#ifndef LIBFACEBOOK_SETTINGS_HPP
#define LIBFACEBOOK_SETTINGS_HPP

#include <QString>

namespace facebook {
namespace app {

struct Settings {
    QString apiKey;
    QString baseUrl;
};

} /* namespace app */
} /* namespace facebook */

#endif // LIBFACEBOOK_SETTINGS_HPP
