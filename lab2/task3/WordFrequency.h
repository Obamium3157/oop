#ifndef OOP_WORD_FREQUENCY_H
#define OOP_WORD_FREQUENCY_H

#include <istream>
#include <map>
#include <string>

std::string ToLower(const std::string & word);
std::map<std::string, int> CountWordFrequencies(std::istream & input);
void PrintWordFrequencies(const std::map<std::string, int> & frequencies, std::ostream & output);


#endif //OOP_WORD_FREQUENCY_H