#include "statistic.h"
#include "Compare.h"

#include <numeric>
#include <cmath>

Statistic::Statistic(int requestAmount, int sourceAmount, int bufferSize, int deviceAmount, double lambda, double alpha, double beta)
{
  this->requestAmount = requestAmount;
  this->sourceAmount = sourceAmount;
  this->bufferSize = bufferSize;
  this->deviceAmount = deviceAmount;
  this->lambda = lambda;
  this->alpha = alpha;
  this->beta = beta;
  totalSimulationTime = 0.0;
  rejectedRequestVector = std::vector<int>(sourceAmount, 0);
  processedRequestVector = std::vector<int>(sourceAmount, 0);
  deviceWorkingTimeVector = std::vector<double>(deviceAmount, 0.0);
  simulationTimeVector = std::vector<double>(sourceAmount, 0.0);
  waitingTimeVector = std::vector<std::vector<double>>(sourceAmount);
  servicingTimeVector = std::vector<std::vector<double>>(sourceAmount);
}

void Statistic::incrementProcessed(int sourceNumber)
{
  processedRequestVector[sourceNumber]++;
}

void Statistic::incrementRejected(int sourceNumber)
{
  rejectedRequestVector[sourceNumber]++;
}

void Statistic::addSimulationTime(int sourceNumber, double time)
{
  simulationTimeVector[sourceNumber] += time;
}

void Statistic::addWaitingTime(int sourceNumber, double time)
{
  waitingTimeVector[sourceNumber].push_back(time);
}

void Statistic::addServicingTime(int sourceNumber, double time)
{
  servicingTimeVector[sourceNumber].push_back(time);
}

void Statistic::addDeviceWorkingTime(int deviceNumber, double time)
{
  deviceWorkingTimeVector[deviceNumber] += time;
}

void Statistic::setTotalSimulationTime(double time)
{
  totalSimulationTime = time;
}

void Statistic::addStep(step_t& step)
{
  stepVector.push_back(step);
}

int Statistic::getProcessed()
{
  int processed = 0;

  for (int i = 0; i < (int)processedRequestVector.size(); i++)
  {
    processed += processedRequestVector[i];
  }
  return processed;
}

int Statistic::getProcessed(int sourceNumber)
{
  return processedRequestVector[sourceNumber];
}

int Statistic::getRejected()
{
  int rejected = 0;

  for (int i = 0; i < (int)rejectedRequestVector.size(); i++)
  {
    rejected += rejectedRequestVector[i];
  }
  return rejected;
}

int Statistic::getRejected(int sourceNumber)
{
  return rejectedRequestVector[sourceNumber];
}

int Statistic::getRequestAmount()
{
  return requestAmount;
}

int Statistic::getSourceAmount()
{
  return sourceAmount;
}

int Statistic::getDeviceAmount()
{
  return deviceAmount;
}

int Statistic::getBufferSize()
{
  return bufferSize;
}

double Statistic::getRejectProbality(int sourceNumber)
{
  return (double)rejectedRequestVector[sourceNumber] /
    (double)(processedRequestVector[sourceNumber] + rejectedRequestVector[sourceNumber]);
}

double Statistic::getWaitingTime(int sourceNumber)
{
  return calculateDoubleVector(waitingTimeVector[sourceNumber]) /
    (processedRequestVector[sourceNumber] + rejectedRequestVector[sourceNumber]);
}

double Statistic::getWaitingTimeDispersion(int sourceNumber)
{
  int requestAmount = (processedRequestVector[sourceNumber] + rejectedRequestVector[sourceNumber]);
  double sum = 0.0, averageWaitingTime = calculateDoubleVector(waitingTimeVector[sourceNumber]) / requestAmount;

  for (int i = 0; i < (int)waitingTimeVector[sourceNumber].size(); i++)
  {
    sum += std::pow(waitingTimeVector[sourceNumber][i] - averageWaitingTime, 2);
  }
  return sum / requestAmount;
}

double Statistic::getServicingTime(int sourceNumber)
{
  return calculateDoubleVector(servicingTimeVector[sourceNumber]) /
    (processedRequestVector[sourceNumber] + rejectedRequestVector[sourceNumber]);
}

double Statistic::getServicingTimeDispersion(int sourceNumber)
{
  double sum = 0.0, averageServiceTime = calculateDoubleVector(servicingTimeVector[sourceNumber]) /
    processedRequestVector[sourceNumber];

  for (int i = 0; i < (int)servicingTimeVector[sourceNumber].size(); i++)
  {
    sum += std::pow(servicingTimeVector[sourceNumber][i] - averageServiceTime, 2);
  }
  return sum / processedRequestVector[sourceNumber];
}

double Statistic::getSimulationTime(int sourceNumber)
{
  return simulationTimeVector[sourceNumber] /
    (processedRequestVector[sourceNumber] + rejectedRequestVector[sourceNumber]);
}

double Statistic::getDeviceUsage(int deviceNumber)
{
  return deviceWorkingTimeVector[deviceNumber] / totalSimulationTime;
}

double Statistic::getLambda()
{
  return lambda;
}

double Statistic::getAlpha()
{
  return alpha;
}

double Statistic::getBeta()
{
  return beta;
}

std::vector<step_t> Statistic::getStepVector()
{
  return stepVector;
}

double Statistic::calculateDoubleVector(std::vector<double> vector)
{
  return std::accumulate(vector.begin(), vector.end(), 0.0);
}


