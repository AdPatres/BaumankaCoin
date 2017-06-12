#pragma once
#include <botan\botan.h>
#include <assert.h>
using namespace Botan;
class Tail
{
public:
	Tail() = default;
	Tail(size_t, secure_vector<byte>);
	bool operator==(const Tail& tail) const;
	bool setValue(size_t);
	bool setAddress(secure_vector<byte>);
	std::pair<size_t, secure_vector<byte>> getInfo() const;
	std::vector<uint8_t> convertTo8();
	~Tail();
private:
	size_t intValue=0;
	secure_vector<byte> address;
};

