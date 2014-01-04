#ifndef FBHACK_HPP
#define FBHACK_HPP

#include "bootstrap.hpp"

#include <QStringList>
#include <QTextStream>

class QJsonDocument;
class QJsonArray;
class QJsonObject;
class QJsonValue;

class FBHack : public Bootstrap {
public:
    FBHack();
    virtual ~FBHack();

protected:
    void main();

private:
    void indent(size_t depth);
    void printDocument(const QJsonDocument & json);
    void printArray(const QJsonArray & array, size_t depth);
    void printObject(const QJsonObject & object, size_t depth);
    void printValue(const QJsonValue & value, size_t depth);
    void help(const QStringList & args);
    QString require(const QString & param);
    void require(const QString & param, QString & target);

    QTextStream qtout;
    QTextStream qterr;
    QTextStream qtin;
};

#endif // FBHACK_HPP
