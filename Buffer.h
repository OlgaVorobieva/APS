#ifndef BUFFERQUEUE_H
#define BUFFERQUEUE_H

#include "request.h"
#include "Statistic.h"
#include "mainwindow.h"
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
    bool addRequest(const requestPointer newRequest, step_t & step);
    int getBufferSize() const;

    requestPointer getRequest(step_t & step);
    void removeRequest(Statistic& statistic, step_t& step);
    int getIndexToRemove();

    int GetAddPointer();
    int GetPullPointer();

private:
    int bufferSize_ = 0;    
    std::vector<requestPointer> requestVector_;
    std::vector<int> packageVector_;

    int lastAddedPointer = 0;
    int lastGetPointer = 0;
};

#endif
