#include "./Input.h"

#include <botan/hex.h>
#include <botan/sha2_32.h>

using namespace ad_patres;
using namespace Botan;

Input::~Input() {}
void
Input::setOutput(Output out)
{
  output = out;
}

void
Input::setOutput(size_t blockNum, size_t txeNum)
{
  output = Output(blockNum, txeNum);
}

void
Input::setTailNum(size_t num)
{
  tailNum = num;
}

void
Input::setHash(std::vector<uint8_t> info)
{
  outputHash = SHA_256().process(info);
}

std::vector<uint8_t>
Input::convertTo8()
{
  std::vector<uint8_t> to;
  converter32to8(output.blockNumber, to);
  converter32to8(output.txeNumber, to);
  for (auto i : outputHash)
    to.push_back(i);
  converter32to8(tailNum, to);
  return to;
}

bool
Input::match(Output out, size_t tail, std::vector<uint8_t> info)
{
  return !(out != output || tail != tailNum
           || SHA_256().process(info) != outputHash);
}

std::pair<Output, size_t>
Input::getInfo() const
{
  return std::pair<Output, size_t>(output, tailNum);
}

bool
Input::scan(std::vector<uint8_t> data, uint32_t& position)
{
  output.blockNumber = converter8to32(data, position);
  output.txeNumber = converter8to32(data, position);
  outputHash.clear();
  for (uint32_t i = 0; i < 32; i++)
    {
      outputHash.push_back(data[position]);
      position++;
    }
  tailNum = converter8to32(data, position);
  return true;
}

void
converter32to8(uint32_t from, std::vector<uint8_t>& to)
{
  uint8_t b[4];
  b[0] = (uint8_t)from;
  b[1] = (uint8_t)(from >>= 8);
  b[2] = (uint8_t)(from >>= 8);
  b[3] = (uint8_t)(from >>= 8);
  for (auto i = 0; i < 4; i++)
    to.push_back(b[i]);
}

uint32_t
converter8to32(std::vector<uint8_t> data, uint32_t& position)
{
  uint8_t a[4];
  for (size_t i = 0; i < 4; i++)
    a[i] = data[position + i];
  uint32_t info = *((uint32_t*)a);
  position += 4;
  return info;
}

bool
Input::operator==(const Input& oth)
{
  return output == oth.output && outputHash == oth.outputHash
         && tailNum == oth.tailNum;
}

std::ostream&
ad_patres::operator<<(std::ostream& os, const Input& o)
{
  return os << "Input:" << o.output << std::endl
            << "Output hash: " << Botan::hex_encode(o.outputHash) << std::endl;
}
