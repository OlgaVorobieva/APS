#ifndef STATISTIC_H
#define STATISTIC_H

#include "StepStruct.h"

#include <vector>

class Statistic
{
public:
  Statistic(int, int, int, int, double, double, double);

  void incrementProcessed(int);
  void incrementRejected(int);
  void addSimulationTime(int, double);
  void addWaitingTime(int, double);
  void addServicingTime(int, double);
  void addDeviceWorkingTime(int, double);
  void addStep(step_t& step);
  void setTotalSimulationTime(double);

  int getProcessed();
  int getProcessed(int);
  int getRejected();
  int getRejected(int);
  int getRequestAmount();
  int getSourceAmount();
  int getDeviceAmount();
  int getBufferSize();

  double getRejectProbality(int);
  double getWaitingTime(int);
  double getWaitingTimeDispersion(int);
  double getServicingTime(int);
  double getServicingTimeDispersion(int);
  double getSimulationTime(int);
  double getDeviceUsage(int);
  double getLambda();
  double getAlpha();
  double getBeta();

  std::vector<step_t> getStepVector();

private:
  int requestAmount;
  int sourceAmount;
  int deviceAmount;
  int bufferSize;
  double lambda;
  double alpha;
  double beta;
  double totalSimulationTime;

  std::vector<int> processedRequestVector;
  std::vector<int> rejectedRequestVector;
  std::vector<double> simulationTimeVector;
  std::vector<double> deviceWorkingTimeVector;
  std::vector<std::vector<double>> waitingTimeVector;
  std::vector<std::vector<double>> servicingTimeVector;
  std::vector<step_t> stepVector;

  double calculateDoubleVector(std::vector<double>);
};

#endif