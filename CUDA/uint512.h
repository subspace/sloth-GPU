// safe include
#pragma once

// C++ imports
#include <cinttypes>

// custom imports
#include "uint256.h"

/*
This library will divide 512-bit unsigned integers into 2 (namely `low` and `high`) 256-bit unsigned integers.
All the implemented operations are done via manipulating `low` and `high` 128-bit parts.

Representation: say Q is a 256-bit unsigned integer.

Q = XXX.............................XXX
				(512-bit)


Q = XXX..............|..............XXX
	   (high 256-bit) (low 256-bit)


HOWEVER, keep in mind that, these `low` and `high` bits will be represented as decimal numbers when printed.
In other words, simply concatenating the DECIMAL representations of the `low` and `high` bits will produce wrong results.

Instead, bit representation of the `low` and `high` bits should be concatenated and printed,
then, a "binary->decimal" converter should be used

This library uses `uint128` custom library for its building blocks.
Understanding how `uint128` library works is crucial for understanding the functionalities of this library.

This library is not commented where the concept is borrowed from `uint128` library.
This library is only commented where the concept is unique to this library (not common with `uint128`)
*/

class uint512_t
{
public:

	uint256_t low;
	uint256_t high;

	__host__ __device__ __forceinline__ uint512_t()
	{
		low = 0;
		high = 0;
	}

	__host__ __device__ __forceinline__ uint512_t operator>>(const unsigned& shift)
	{
		uint512_t z;

		z.low = low >> shift;
		z.low = (high << (256 - shift)) | z.low;
		z.high = high >> shift;

		return z;
	}

	__host__ __device__ __forceinline__ uint512_t operator=(const uint512_t& l)
	{
		low = l.low;
		high = l.high;
	}
};

__host__ uint512_t operator-(const uint512_t& x, const uint512_t& y)
{
	uint512_t z;

	z.low = x.low - y.low;
	z.high = x.high - y.high - (x.low < y.low);

	return z;
}

/* WRYYYYYYYYYYYYYYYYYYYYYYYYYYYY */
__device__ __forceinline__ uint512_t mul256x2(const uint256_t& a, const uint256_t& b)
{
	uint512_t c;
	uint256_t temp;

	c.low = mul64_256(a.low, b.low); //alow * blow

	temp = mul64_256(a.high, b.low);
	c.low.high = c.low.high + temp.low;
	c.high.low = temp.high + (c.low.high < temp.low); //ahigh * blow

	temp = mul64_256(a.low, b.high);
	c.low.high = c.low.high + temp.low;
	c.high.low = c.high.low + temp.high + (c.low.high < temp.low);
	c.high.high = c.high.high + (c.high.low < temp.high); //alow * bhigh

	temp = mul64_256(a.high, b.high);
	c.high.low = c.high.low + temp.low;
	c.high.high = temp.high + (c.high.low < temp.low); //ahigh * bhigh

	return c;
}

__host__ __device__ __forceinline__ uint512_t mul257_256(const uint512_t& a, const uint256_t& b)
{
    // currently UNFINISHED
    
	uint512_t c;  // inside this, we are only storing a 257 bit number (we are not utilizing all 512 bits here)

	c = mul256x2(a.low, b);  // multiply the 256 bits of a, with the whole b, and store the result in c

	if (a.high.low.low & 1)  // might be unnecessary, result of the mul256x2 operation do not fits in 256-bits
		c.high = c.high + b;  // 

	return c;
}

__host__ __device__ __forceinline__ uint512_t operator-(const uint512_t& x, const uint256_t& y)
{
	uint512_t z;

	z.low = x.low - y;
	z.high = x.high - (x.low < y);

	return z;
}