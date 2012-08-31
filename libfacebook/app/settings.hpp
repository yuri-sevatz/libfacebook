#ifndef LIBFACEBOOK_SETTINGS_HPP
#define LIBFACEBOOK_SETTINGS_HPP

#include <QString>
#include <QStringList>

namespace facebook {
namespace app {

struct Settings {
    QString apiKey;
    QString baseUrl;
    QStringList permissions;
};

} /* namespace app */
} /* namespace facebook */

#endif // LIBFACEBOOK_SETTINGS_HPP
