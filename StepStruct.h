#ifndef STEPSTRUCT_H
#define STEPSTRUCT_H

#include <vector>
#include <string>
#include "request.h"

struct step_t
{
    double time;
    std::string actionString;
    std::vector<requestPointer> bufferStateVector;
    std::vector<requestPointer> deviceStateVector;
    int addPointer;
    int getPointer;
};

#endif // STEPSTRUCT_H
