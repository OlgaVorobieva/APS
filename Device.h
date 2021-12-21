#ifndef DEVICE_H
#define DEVICE_H

#include "Request.h"
#include "Statistic.h"
#include "StepStruct.h"

#include <random>
#include <memory>
#include <iostream>
#include <QTGlobal>

class Device
{
public:
    Device(int deviceNumber, double a, double b);

    void getRequest(const requestPointer newRequest, step_t & step);

    bool isAvaliable(Statistic &statistic, step_t & step);
    bool isAvaliable(double lastRequestTime, Statistic &statistic, step_t & step);
    void fillStatistic(Statistic &statistic, step_t & step);
    int getDeviceNumber() const;
    double getTaskStartTime() const;

private:
    double a_;
    double b_;
    double taskStartTime_ = 0;
    double taskEndTime_ = 0;
    const int deviceNumber_;
    requestPointer currentRequest_;
};

#endif
