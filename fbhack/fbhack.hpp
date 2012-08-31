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
    void help(const QStringList & args);
    void force(const QString & param, QString & target);

    QTextStream qtout;
    QTextStream qtin;
};

#endif // FBHACK_HPP
