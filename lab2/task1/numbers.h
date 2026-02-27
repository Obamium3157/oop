#ifndef OOP_NUMBERS_H
#define OOP_NUMBERS_H

#include <vector>

std::vector<double> ReadNumbers();
double ComputePositiveMean(const std::vector<double> & numbers);
void ProcessNumbers(std::vector<double> & numbers);
void PrintSortedNumbers(std::vector<double> numbers);

#endif //OOP_NUMBERS_H