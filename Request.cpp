#include "request.h"


Request::Request(double startTime, int sourceNumber, int requestNumber) :
    startTime_(startTime),
    requestNumber_(requestNumber),
    sourceNumber_(sourceNumber)
{
}

int Request::getSourceNumber()
{
    return sourceNumber_;
}

int Request::getRequestNumber()
{
    return requestNumber_;
}

double Request::getStartTime()
{
    return startTime_;
}

int Request::getSerialNumber()
{
  return serialNumber_;
}

void Request::setSerialNumber(int number)
{
  serialNumber_ = number;
}

std::shared_ptr<Request> Request::getNewRequest(double startTime, int sourceNumber, int requestNumber)
{
    return std::shared_ptr<Request>(new Request(startTime, sourceNumber, requestNumber));
}
