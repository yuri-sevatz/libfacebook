#ifndef BOOTSTRAP_HPP
#define BOOTSTRAP_HPP

#include <QObject>

class QApplication;

class Bootstrap : public QObject {
    Q_OBJECT
public:
    Bootstrap();
    virtual ~Bootstrap();

    void init(QApplication & app);

protected:
    virtual void main() = 0;
    Q_SIGNAL void quit();

private:
    Q_SLOT void run();
};

#endif // BOOTSTRAP_HPP
