#ifndef SOURCE_H
#define SOURCE_H

#include <random>
#include <QTGlobal>

#include "request.h"

class Source
{
public:
    Source(int number, double speed);

    int getSourceNumber();
    int getGeneratedTasks();
    double getRequestTime();
    requestPointer generation();

private:
    int sourceNumber_;
    int generatedTasks_ = 0;
    double sourceSpeed_ = 0.0;
    double newRequestTime_ = 0.0;
};

#endif
