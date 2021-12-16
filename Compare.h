#ifndef COMPARE_H
#define COMPARE_H

#include "request.h"
#include "source.h"

class cmp {
public:
    bool operator() (const requestPointer &a, const requestPointer &b);
    bool operator() (Source &a, Source &b);
};

#endif // COMPARE_H
