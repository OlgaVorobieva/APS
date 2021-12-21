#include "Device.h"

Device::Device(int deviceNumber, double a, double b) :
    deviceNumber_(deviceNumber),
    a_(a),
    b_(b)
{
}

bool Device::isAvaliable(Statistic &statistic, step_t & step)
{
    if (currentRequest_ == nullptr)
    {
        return true;
    } else {
        fillStatistic(statistic, step);
        currentRequest_ = nullptr;
        taskStartTime_ = taskEndTime_;
        return true;
    }
}

bool Device::isAvaliable(double lastRequestTime, Statistic &statistic, step_t & step)
{
    if (currentRequest_ == nullptr)
    {
        return true;
    }

    if (taskEndTime_ < lastRequestTime)
    {
        fillStatistic(statistic, step);
        currentRequest_ = nullptr;
        taskStartTime_ = taskEndTime_;
        return true;
    }
    return false;
}

void Device::getRequest(const requestPointer newRequest, step_t & step)
{
    currentRequest_ = newRequest;
    step.deviceStateVector[deviceNumber_] = newRequest;

    if ((taskStartTime_ == 0) || (taskStartTime_ < newRequest->getStartTime()))
    {
        taskStartTime_ = newRequest->getStartTime();
    }
    taskEndTime_ = taskStartTime_ + (((double)qrand()/(double)RAND_MAX) * (b_ - a_) + a_);

    step.time = taskStartTime_;
    step.deviceStateVector[deviceNumber_] = newRequest;// std::to_string(currentRequest_->getSourceNumber() + 1)+ "." +
    step.actionString = "Request " + std::to_string(currentRequest_->getSerialNumber())
            + " is on device " + std::to_string(getDeviceNumber() + 1);
}

void Device::fillStatistic(Statistic &statistic, step_t & step)
{
    step.time = taskEndTime_;
    step.deviceStateVector[deviceNumber_] = nullptr;//+ std::to_string(currentRequest_->getSourceNumber() + 1)+ "."
    step.actionString = "Request "  + std::to_string(currentRequest_->getSerialNumber())
            + " processed. Device " + std::to_string(getDeviceNumber() + 1) + " is free.";
    statistic.addStep(step);

    statistic.incProcessed(currentRequest_->getSourceNumber());
    statistic.incTimeInSystem(currentRequest_->getSourceNumber(), taskEndTime_ - currentRequest_->getStartTime());
    statistic.incDeviceWorkingTime(deviceNumber_, taskEndTime_ - taskStartTime_);
    statistic.incWaitingTime(currentRequest_->getSourceNumber(), taskStartTime_ - currentRequest_->getStartTime());
    statistic.incServiceTime(currentRequest_->getSourceNumber(), taskEndTime_ - taskStartTime_);
    statistic.incSystemTime(taskEndTime_);
}

int Device::getDeviceNumber() const
{
    return deviceNumber_;
}

double Device::getTaskStartTime() const
{
    return taskStartTime_;
}
