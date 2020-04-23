#ifndef TEA_H
#define TEA_H

#include <string>
#include <array>

std::array<unsigned int, 2> decipher (std::array<unsigned int, 2> input,
                                      const std::array<unsigned int, 4>&  key);

std::array<unsigned int, 2> encipher (std::array<unsigned int, 2> input,
                                      const std::array<unsigned int, 4>&  key);

void encipher (std::istream& is, std::ostream& os, std::string key);

void decipher (std::istream& is, std::ostream& os, std::string key);

#endif // TEA_H
