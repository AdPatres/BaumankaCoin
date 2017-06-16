#include "tail.h"

Tail::Tail(size_t integer, secure_vector<byte> destination)
{
	intValue = integer;
	//assert(destination.size() != 256);//check this assert, size can be different (257 if /0) or in different encoding that make size biiger or smaller and then change to throw exception
	address = destination;
	
}
bool Tail::operator==(const Tail& tail) const
{
	if (this->address == tail.address && this->intValue == tail.intValue)
		return true;
	return false;
}
bool Tail::setValue(size_t integer)
{
	intValue = integer;
	return true;
}
bool Tail::setAddress(secure_vector<byte> destination)
{
	
	//assert(destination.size() != 256);//check this assert, size can be different (257 if /0) or in different encoding that make size biiger or smaller and then change to throw exception
	address = destination;
	return true;
}

void converter(size_t from, std::vector<uint8_t>& to)
{
	for (auto i = 3; i >= 0; i--)
	{
		to.push_back((uint8_t)from >> 8 * i);
	}
}
std::vector<uint8_t> Tail::convertTo8()
{
	std::vector<uint8_t> to;
	converter(intValue, to);
	for (auto i : address)
	{
		to.push_back(i);
	}
	return to;
}
std::pair<size_t, secure_vector<byte>> Tail::getInfo() const
{
	return std::pair<size_t, secure_vector<byte>>(intValue, address);
}
Tail::~Tail()
{
}

bool Tail::scan(std::vector<uint8_t> data, uint32_t& position)
{
	uint8_t a[4];
	for (size_t i = 0; i < 4; i++)
	{
		a[i] = data[position + i];
	}
	intValue = *((uint32_t*)a);
	position += 4;
	address.clear();
	for (uint32_t i = 0; i < 32; i++)
	{
		address.push_back(data[position]);
		position++;
	}
	return true;
}
