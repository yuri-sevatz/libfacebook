#ifndef FBHACK_HPP
#define FBHACK_HPP

#include "bootstrap.hpp"

#include <QStringList>
#include <QTextStream>

class FBHack : public Bootstrap {
public:
    FBHack();
    virtual ~FBHack();

protected:
    void main();

private:
    void indent(size_t depth);
    void printList(const QVariantList & list, size_t depth);
    void printMap(const QVariantMap & map, size_t depth);
    void printValue(const QVariant & value, size_t depth = 0);
    void help(const QStringList & args);
    void force(const QString & param, QString & target);

    QTextStream qtout;
    QTextStream qtin;
};

#endif // FBHACK_HPP
