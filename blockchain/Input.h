#pragma once
#include "output.h"
#include <botan/botan.h>
#include <botan/sha2_32.h>
#include <iostream>
#include <botan/hex.h>

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
	bool scan(std::vector<uint8_t>, uint32_t&);
	bool match(Output, size_t, std::vector<uint8_t>);
	std::pair<Output, size_t> getInfo() const;
	std::vector<uint8_t> convertTo8();
	bool operator==(const Input& oth) { return output == oth.output && outputHash == oth.outputHash && tailNum == oth.tailNum; }
	friend std::ostream& operator<<(std::ostream&, const Input& o);
private:
	Output output;
	secure_vector<byte> outputHash = secure_vector<byte>(32,0);;
	size_t tailNum = 0;
};

void converter32to8(uint32_t from, std::vector<uint8_t>& to);
uint32_t converter8to32(std::vector<uint8_t>, uint32_t&);

std::ostream&
operator<<(std::ostream& os, const Input& o);
