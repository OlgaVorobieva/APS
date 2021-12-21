#include "Statistic.h"

Statistic::Statistic(int requestQuantity, int sourcesQuantity, int bufferSize, int deviceQuantity, double lambda, double a, double b):
    requestQantity_(requestQuantity),
    bufferSize_(bufferSize),
    sourceQantity_(sourcesQuantity),
    devicesQuantity_(deviceQuantity),
    lambda_(lambda),
    a_(a),
    b_(b)
{
    rejectedRequestQantity_ = std::vector<int>(sourcesQuantity, 0);
    processedRequestQantity_ = std::vector<int>(sourcesQuantity, 0);
    timeInSystem_ = std::vector<double>(sourcesQuantity, 0.0);
    deviceWorkingTime_ = std::vector<double>(deviceQuantity, 0.0);
    waitingTime_ = std::vector<std::vector<double>>(sourcesQuantity);
    serviceTime_ = std::vector<std::vector<double>>(sourcesQuantity);
}

void Statistic::incProcessed(int sourceNumber)
{
    processedRequestQantity_[sourceNumber]++;
}

void Statistic::incRejected(int sourceNumber)
{
    rejectedRequestQantity_[sourceNumber]++;
}

int Statistic::getProcessed()
{
    int processed = 0;
    for (int i = 0; i < processedRequestQantity_.size(); i++)
    {
        processed += processedRequestQantity_[i];
    }
    return processed;
}

int Statistic::getProcessed(int sourceNumber)
{
    return processedRequestQantity_[sourceNumber];
}

int Statistic::getRejected()
{
    int rejected = 0;
    for (int i = 0; i < rejectedRequestQantity_.size(); i++)
    {
        rejected += rejectedRequestQantity_[i];
    }
    return rejected;
}

int Statistic::getRejected(int sourceNumber)
{
    return rejectedRequestQantity_[sourceNumber];
}

void Statistic::incTimeInSystem(int sourceNumber, double time)
{
    timeInSystem_[sourceNumber] += time;
}

double Statistic::getTimeInSystem(int sourceNumber)
{
    return timeInSystem_[sourceNumber];
}

double Statistic::getTotalTime(std::vector<double> time)
{
    return std::accumulate(time.begin(), time.end(), 0);
}

void Statistic::incWaitingTime(int sourceNumber, double time)
{
    waitingTime_[sourceNumber].push_back(time);
}

double Statistic::getWaitingTimeDisp(int sourceNumber)
{
    double sum = 0.0;
    int requestsQuantity =  (processedRequestQantity_[sourceNumber] + rejectedRequestQantity_[sourceNumber]);
    double averageWaitTime = getTotalTime(waitingTime_[sourceNumber]) / requestsQuantity;

    for (int i = 0; i < waitingTime_[sourceNumber].size(); i++)
    {
        sum += pow(waitingTime_[sourceNumber][i] - averageWaitTime, 2);
    }
    return sum / requestsQuantity;
}

void Statistic::incServiceTime(int sourceNumber, double time)
{
    serviceTime_[sourceNumber].push_back(time);
}

double Statistic::getServiceTimeDisp(int sourceNumber)
{
    double sum = 0.0;
    double averageServiceTime = getTotalTime(serviceTime_[sourceNumber]) / processedRequestQantity_[sourceNumber];

    for (int i = 0; i < serviceTime_[sourceNumber].size(); i++)
    {
        sum += pow(serviceTime_[sourceNumber][i] - averageServiceTime, 2);
    }
    return sum / processedRequestQantity_[sourceNumber];
}

void Statistic::incDeviceWorkingTime(int deviceNumber, double time)
{
    deviceWorkingTime_[deviceNumber] += time;
}

double Statistic::getDeviceWorkingTime(int deviceNumber)
{
    return deviceWorkingTime_[deviceNumber];
}

void Statistic::incSystemTime(double time)
{
    systemTime_ = time;
}
double Statistic::getSystemTime()
{
    return systemTime_;
}

void Statistic::addStep(step_t step)
{
    steps_.push_back(step);
}

std::vector<step_t> Statistic::getSteps()
{
    return steps_;
}

int Statistic::getRequestQantity()
{
    return requestQantity_;
}

int Statistic::getBufferSize()
{
    return bufferSize_;
}

int Statistic::getSourceQantity()
{
    return sourceQantity_;
}

int Statistic::getDevicesQuantity()
{
    return devicesQuantity_;
}

double Statistic::getLambda()
{
    return lambda_;
}

double Statistic::getA_()
{
    return a_;
}

double Statistic::getB_()
{
    return b_;
}
