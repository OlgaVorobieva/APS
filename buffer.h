#ifndef BUFFERQUEUE_H
#define BUFFERQUEUE_H

#include "request.h"
#include "Statistic.h"
#include "ProjectAPS.h"
#include "StepStruct.h"

#include <iostream>
#include <memory>
#include <vector>
#include <iterator>


class Buffer
{
public:
    Buffer(const int bufferSize);

    bool isEmpty();
    bool isFull();
    void minusFreeSpace();
    void addRequest(const requestPointer newRequest, step_t & step);
    int getBufferSize() const;
    int getFreeSpace() const;
    int findPrioritySource();

    requestPointer getRequest(step_t & step);

private:
    std::vector<requestPointer> requestVector_;
    std::vector<int> packageVector_;
    int bufferSize_ = 0;
    int freeSpace_;
};

#endif
