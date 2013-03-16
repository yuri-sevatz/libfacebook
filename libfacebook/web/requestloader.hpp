#ifndef WEB_REQUESTLOADER_HPP
#define WEB_REQUESTLOADER_HPP

#include <QtGlobal>

class QNetworkReply;

namespace web {

class RequestLoader {
public:
    RequestLoader();
    virtual ~RequestLoader();

    QNetworkReply * load(QNetworkReply * reply);
private:
    Q_DISABLE_COPY(RequestLoader);
};

}

#endif // WEB_REQUESTLOADER_HPP
