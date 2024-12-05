#include "AOCHelper.h"

#include <fstream>
#include <streambuf>

std::string AOCHelper::readInput(const std::string &fileName)
{
  std::ifstream file(fileName);

  std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return str;
}
