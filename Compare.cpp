#include "Compare.h"

bool cmp::operator() (const requestPointer &a, const requestPointer &b)
{
    return a -> getStartTime() < b -> getStartTime();
}

bool cmp::operator() (Source &a, Source &b)
{
    return a.getRequestTime() < b.getRequestTime();
}
