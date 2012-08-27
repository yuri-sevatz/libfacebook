#ifndef FBHACK_HPP
#define FBHACK_HPP

#include "bootstrap.hpp"

class FBHack : public Bootstrap
{
public:
    FBHack();
    virtual ~FBHack();

protected:
    void main();

private:
    void help();
};

#endif // FBHACK_HPP
