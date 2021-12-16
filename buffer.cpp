#include "Buffer.h"

Buffer::Buffer(const int bufferSize) :
    bufferSize_(bufferSize),
    freeSpace_(bufferSize)
{
    requestVector_ = std::vector<requestPointer>(bufferSize);
    requestVector_.reserve(bufferSize);
}

void Buffer::addRequest(const requestPointer newRequest, step_t & step)
{
    for (int i = 0; i != bufferSize_; i++)
    {
        if (requestVector_[i] == nullptr)
        {
            requestVector_[i] = newRequest;
            step.time = newRequest->getStartTime();
            step.bufferStateVector[i] = newRequest;
            step.actionString = "Request" + std::to_string(newRequest->getSourceNumber() + 1)+ "." + std::to_string(newRequest->getRequestNumber()) +" generated and added to buffer.";
            break;
        }
    }
}

void Buffer::minusFreeSpace()
{
    freeSpace_--;
}

bool Buffer::isEmpty()
{
    return freeSpace_ == bufferSize_;
}

bool Buffer::isFull()
{
    return freeSpace_ == 0;
}

int Buffer::getBufferSize() const
{
    return bufferSize_;
}

int Buffer::getFreeSpace() const
{
    return freeSpace_;
}

int Buffer::findPrioritySource()
{
    int i = 0, min = INT_MAX;
    while ((min != 0) && (i != static_cast<int>(requestVector_.size()))) {
        if (requestVector_[i] != nullptr)
        {
            if (min > requestVector_[i]->getSourceNumber())
            {
                min = requestVector_[i]->getSourceNumber();
            }
        }
        i++;
    }
    return min;
}

requestPointer Buffer::getRequest(step_t & step)
{
    if (packageVector_.size() == 0)
    {
        int s = findPrioritySource();

        for (int i = 0; i < bufferSize_; i++)
        {
            if (requestVector_[i] == nullptr)
            {
                continue;
            }
            if (requestVector_[i] ->getSourceNumber() == s)
            {
                packageVector_.push_back(i);
            }
        }
    }
    step.bufferStateVector[packageVector_[0]] = nullptr;

    requestPointer tmpPointer = requestVector_[packageVector_[0]];
    requestVector_[packageVector_[0]] = nullptr;
    packageVector_.erase(packageVector_.begin());
    freeSpace_++;

    return tmpPointer;
}
