#pragma once
struct Output
{
	Output() = default;
	Output(size_t, size_t);
	bool operator !=( const Output&) const;
	bool operator ==(const Output&) const;
	size_t blockNumber;
	size_t txeNumber;
};

