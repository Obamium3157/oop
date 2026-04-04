#ifndef OOP_ECONOMYHELPER_H
#define OOP_ECONOMYHELPER_H

#include "IActor.h"

#include <vector>

int readIterationCount(int argc, char* argv[]);
void printFinalState(const std::vector<IActor*>& actors, const Bank& bank);

#endif //OOP_ECONOMYHELPER_H
