#pragma once
#include "output.h"
#include <botan/botan.h>
#include <botan/sha2_32.h>

using namespace Botan;
class Input
{
public:
	Input() = default;
	~Input();
	bool setOutput(size_t, size_t);
	bool setOutput(Output);
	bool setHash(std::vector<uint8_t>);
	bool setTailNum(size_t);
	bool match(Output, size_t, std::vector<uint8_t>);
	std::pair<Output, size_t> getInfo() const;
	std::vector<uint8_t> convertTo8();
private:
	Output output;
	secure_vector<byte> outputHash;
	size_t tailNum;
};

void converter32to8(size_t from, std::vector<uint8_t>& to);
