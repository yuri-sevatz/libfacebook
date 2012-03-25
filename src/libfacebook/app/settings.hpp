#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

namespace facebook {
namespace app {

class Settings
{
public:
    Settings();
    virtual ~Settings();

    std::string apiKey;
    std::string baseUrl;
};

} /* namespace app */
} /* namespace facebook */

#endif // SETTINGS_HPP
