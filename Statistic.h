#ifndef STATISTIC_H
#define STATISTIC_H

#include <vector>
#include <numeric>
#include <cmath>
#include "StepStruct.h"

class Statistic
{
public:
    Statistic(int requestQuantity, int sourcesQuantity, int bufferSize, int deviceQuantity, double lambda, double a, double b);
    void incProcessed(int sourceNumber);
    void incRejected(int sourceNumber);
    int getProcessed();
    int getProcessed(int sourceNumber);
    int getRejected();
    int getRejected(int sourceNumber);
    void incTimeInSystem(int sourceNumber, double time);
    double getTimeInSystem(int sourceNumber);
    void incWaitingTime(int sourceNumber, double time);
    double getTotalTime(std::vector<double> time);
    double getWaitingTimeDisp(int sourceNumber);
    void incServiceTime(int sourceNumber, double time);
    double getServiceTimeDisp(int sourceNumber);
    void incDeviceWorkingTime(int deviceNumber, double time);
    double getDeviceWorkingTime(int deviceNumber);
    void incSystemTime(double time);
    double getSystemTime();
    void addStep(step_t step);
    std::vector<step_t> getSteps();
    int getRequestQantity();
    int getBufferSize();
    int getSourceQantity();
    int getDevicesQuantity();
    double getA_();
    double getB_();
    double getLambda();

private:
    std::vector<int> processedRequestQantity_;
    std::vector<int> rejectedRequestQantity_;
    std::vector<double> timeInSystem_;
    std::vector<std::vector<double>> waitingTime_;
    std::vector<std::vector<double>> serviceTime_;
    std::vector<double> deviceWorkingTime_;
    std::vector<step_t> steps_;
    double systemTime_;
    int requestQantity_;
    int bufferSize_;
    int sourceQantity_;
    int devicesQuantity_;
    double lambda_;
    double a_;
    double b_;
};

#endif // STATISTIC_H
