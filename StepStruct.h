#ifndef STEP_H
#define STEP_H

#include "request.h"

#include <vector>
#include <string>
#include <memory>

struct step_t
{
  double time;
  int rejected;
  int processed;
  int devicePointer;
  std::string action;
  std::vector<std::shared_ptr<Request>> packageStateVector;
  std::vector<std::shared_ptr<Request>> sourceStateVector;
  std::vector<std::shared_ptr<Request>> bufferStateVector;
  std::vector<std::shared_ptr<Request>> deviceStateVector;
};
#endif