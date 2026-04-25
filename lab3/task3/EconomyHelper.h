#ifndef OOP_ECONOMYHELPER_H
#define OOP_ECONOMYHELPER_H

#include "IActor.h"

#include <vector>

int ReadIterationCount(int argc, char* argv[]);
void PrintFinalState(const std::vector<IActor*>& actors, const Bank& bank);
void AssertFinalState(const std::vector<IActor*>& actors, const Bank& bank);

#endif //OOP_ECONOMYHELPER_H
