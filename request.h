#ifndef REQUEST_H
#define REQUEST_H

#include <memory>

class Request
{
public:
    Request(double startTime, int sourceNumber, int requestNumber);

    static std::shared_ptr<Request> getNewRequest(double startTime, int sourceNumber, int requestNumber);

    int getSourceNumber();
    int getRequestNumber();
    double getStartTime();
    int getSerialNumber();
    void setSerialNumber(int number);

private:
    double startTime_ = 0;
    int requestNumber_ = 0;
    int sourceNumber_ = 0;
    int serialNumber_ = 0;
};

typedef std::shared_ptr<Request> requestPointer;

#endif
