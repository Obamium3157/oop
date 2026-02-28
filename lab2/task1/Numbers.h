#ifndef OOP_NUMBERS_H
#define OOP_NUMBERS_H

#include <vector>

std::vector<double> ReadNumbers(std::istream & in);
void ProcessNumbers(std::vector<double> & numbers);
void PrintSortedNumbers(std::ostream & out, std::vector<double> numbers);

#endif //OOP_NUMBERS_H