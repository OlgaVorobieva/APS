#include "source.h"

Source::Source (int number, double speed) :
    sourceNumber_(number),
    sourceSpeed_(speed)
{
}

int Source::getSourceNumber()
{
    return sourceNumber_;
}

int Source::getGeneratedTasks()
{
    return generatedTasks_;
}

double Source::getRequestTime()
{
    return newRequestTime_;
}

requestPointer Source::generation()
{
    generatedTasks_++;
    newRequestTime_ += (-1 / sourceSpeed_ * log(static_cast<double>(qrand()) / static_cast<double>(RAND_MAX)));
    return Request::getNewRequest(newRequestTime_, sourceNumber_ , generatedTasks_);
}
