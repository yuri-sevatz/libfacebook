#ifndef LIBFACEBOOK_CREDENTIALS_HPP
#define LIBFACEBOOK_CREDENTIALS_HPP

#include <QString>

namespace facebook {
namespace auth {

struct Credentials {
    QString login;
    QString pass;
};

} /* namespace auth */
} /* namespace facebook */

#endif // LIBFACEBOOK_CREDENTIALS_HPP
