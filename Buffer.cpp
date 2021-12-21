#include "Buffer.h"

Buffer::Buffer(const int bufferSize) :
    bufferSize_(bufferSize)
{
    requestVector_ = std::vector<requestPointer>(bufferSize);
    requestVector_.reserve(bufferSize);
}

bool Buffer::addRequest(const requestPointer newRequest, step_t& step)
{
  bool isAdded = false;
  bool isFull = false;
  int oldPointer = lastAddedPointer;
  // search free place to add by circle 
  while (!isAdded && !isFull)
  {
    if (requestVector_[lastAddedPointer] == nullptr)
    {
      requestVector_[lastAddedPointer] = newRequest;

      step.time = newRequest->getStartTime();
      step.bufferStateVector[lastAddedPointer] = newRequest;
      step.actionString = "Request " +  std::to_string(newRequest->getSerialNumber()) + " generated and added to buffer.";
      
      lastAddedPointer++;
      if (lastAddedPointer == bufferSize_)
      {
        lastAddedPointer = 0;
      }
      step.addPointer = lastAddedPointer;
      isAdded = true;
    }
    else
    {
      lastAddedPointer++;
      if (lastAddedPointer == bufferSize_)
      {
        lastAddedPointer = 0;
      }
    }
    if (lastAddedPointer == oldPointer)
    {
      isFull = true;
    }
  }
  return isAdded;

}

bool Buffer::isEmpty()
{
  for (int i = 0; i < bufferSize_; i++)
  {
    if (requestVector_[i] != nullptr)
    {
      return false;
    }
  }
  return true;
}


int Buffer::getBufferSize() const
{
    return bufferSize_;
}

requestPointer Buffer::getRequest(step_t & step)
{
  int i = 0;
  requestPointer tmpPointer = nullptr;
  while (tmpPointer == nullptr && i < bufferSize_)
  {
    if (requestVector_[lastGetPointer] != nullptr)
    {
      tmpPointer = requestVector_[lastGetPointer];
      requestVector_[lastGetPointer] = nullptr;
      step.bufferStateVector[lastGetPointer] = nullptr;

    }
    lastGetPointer++;
    if (lastGetPointer >= bufferSize_)
    {
      lastGetPointer = 0;
    }
    i++;
  }
  if (tmpPointer != nullptr)
  {
    step.getPointer = lastGetPointer;
  }
  return tmpPointer;
}

int Buffer::getIndexToRemove()
{
  int index = -1;

  if (requestVector_[0] != nullptr) {
    index = 0;
    for (int i = 1; i < bufferSize_; i++)
    {
      if ((requestVector_[i] != nullptr) && (requestVector_[index]->getStartTime() > requestVector_[i]->getStartTime()))
      {
        index = i;
      }
    }
  }
 
  return index;
}

int Buffer::GetAddPointer()
{
  return lastAddedPointer;
}

int Buffer::GetPullPointer() 
{
  return lastGetPointer;
}

void Buffer::removeRequest(Statistic& statistic, step_t& step)
{
  int index = getIndexToRemove();
  if (index >= 0 && index< bufferSize_ )
  {
    //there is an element to reject
    step.time = requestVector_[index]->getStartTime(); 
    step.actionString = "Request " + std::to_string(requestVector_[index]->getSerialNumber()) + " rejected from buffer.";
    step.bufferStateVector[index] = nullptr;

    statistic.incRejected(requestVector_[index]->getSourceNumber());
    statistic.addStep(step);

    requestVector_[index] = nullptr;
  }

}
