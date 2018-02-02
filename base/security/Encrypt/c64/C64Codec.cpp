#include "stdafx.h"
#include "c64codec.h"

const UINT BITS_MASK[] = {
	0x00000001,	0x00000003,	0x00000007, 0x0000000F,
	0x0000001F,	0x0000003F,	0x0000007F, 0x000000FF,
	0x000001FF,	0x000003FF,	0x000007FF, 0x00000FFF,
	0x00001FFF,	0x00003FFF,	0x00007FFF, 0x0000FFFF,
	0x0001FFFF,	0x0003FFFF,	0x0007FFFF, 0x000FFFFF,
	0x001FFFFF,	0x003FFFFF,	0x007FFFFF, 0x00FFFFFF,
	0x01FFFFFF,	0x03FFFFFF,	0x07FFFFFF, 0x0FFFFFFF,
	0x1FFFFFFF,	0x3FFFFFFF,	0x7FFFFFFF, 0xFFFFFFFF,
};

// 字符表
//			0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
//		0x	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
//		1x	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V
//		2x	W	X	Y	Z	a	b	c	d	e	f	g	h	i	j	k	l
//		3x	m	n	o	p	q	r	s	t	u	v	w	x	y	z	-	_

void C64Codec::EncodeBytes(const BYTE *src, size_t cb, std::vector<char>& tag)
{
	size_t cbCrypt = CalcEncodedSize(cb);
	tag.resize(cbCrypt);
	char*	ptag = &tag.front();

	const BYTE *pd = src, *pe = src + cb;
	size_t	rmbits = 8;
	BYTE	rd = *pd;
	while (pd < pe)
	{
		BYTE	re;
		if (rmbits > 6)
		{
			re = rd & 0x3F;
			rd >>= 6;
			rmbits -= 6;
		}
		else
		{
			re = rd;
			++pd;
			rd = pd < pe ? *pd : 0;	// 可能越界

			size_t	padbits = 6 - rmbits;
			if (padbits != 0)
			{
				re |= (rd & BITS_MASK[padbits - 1]) << rmbits;
				rd >>= padbits;
			}
			rmbits += 2;
		}

		*ptag++ = EncodeByte(re);
	}
	ASSERT(pd == pe);
	ASSERT(ptag - &tag.front() == cbCrypt);
}

void C64Codec::DecodeBytes(const char* src, size_t cb, std::vector<BYTE>& tag)
{
	size_t cbDecode = CalcDecodedSize(cb);
	tag.resize(cbDecode);
	BYTE* ptag = &tag.front();

	const BYTE *pd = (const BYTE*)(src), *pe = pd + cb;
	size_t rmbits = 6;
	BYTE rd = DecodeByte(*pd++);

	while (pd < pe)
	{
   		BYTE re;

		if (rmbits == 0)
		{
			rd = DecodeByte(*(pd++));
			rmbits = 6;
		}
		else
		{
			re = rd;
			rd = DecodeByte(*(pd++));

			size_t padbits = 8 - rmbits;
			if (padbits != 0)
			{
				re |= (rd & BITS_MASK[padbits - 1]) << rmbits;
				rd >>= padbits;
			}

			rmbits -= 2;

			*ptag++ = re;
		}
	}

	ASSERT(pd == pe);
	ASSERT(ptag - &tag.front() == cbDecode);
}

// -------------------------------------------------------------------------- //

#if TEST_UNITTEST_ON

static class TestC64Codec
{
public:
	TestC64Codec()
	{
		REGISTER_TEST_SUITE(TestCalcSize);
		REGISTER_TEST_SUITE(TestEncodeBytes);
		REGISTER_TEST_SUITE(TestDecodeBytes);
		REGISTER_TEST_SUITE(TestCodec);
	}

private:
	static void TestCalcSize()
	{
		size_t	s[] = { 0, 1, 2, 3, 4, 5, 10, 18, 32 };
		size_t	t[] = { 0, 2, 3, 4, 6, 7, 14, 24, 43 };

		for (size_t i = 0; i < countof(s); ++i)
		{
			TEST_ASSERT(C64Codec::CalcEncodedSize(s[i]) == t[i]);
			TEST_ASSERT(C64Codec::CalcDecodedSize(t[i]) == s[i]);
		}

		for (size_t i = 0; i < 100; ++i)
		{
			TEST_ASSERT(C64Codec::CalcDecodedSize(C64Codec::CalcEncodedSize(i)) == i);
			if ((i & 3) != 1)
				TEST_ASSERT(C64Codec::CalcEncodedSize(C64Codec::CalcDecodedSize(i)) == i);
		}
	}

	static void CheckC64Codes(const std::vector<char>& codes, const char* cch, size_t cb)
	{
		TEST_ASSERT(codes.size() == cb);
		for (size_t i = 0; i < cb; ++i)
			TEST_ASSERT(codes[i] == cch[i]);
	}
	static void TestEncodeBytes()
	{
		std::vector<char>	tag;
		{
			BYTE	s[] = { 0x9C, 0x3E, 0xB6, 0x9C, 0x3E, 0xB6 };

			C64Codec::EncodeBytes(s, 1, tag);
			CheckC64Codes(tag, "S2", 2);

			C64Codec::EncodeBytes(s, 2, tag);
			CheckC64Codes(tag, "Sw3", 3);

			C64Codec::EncodeBytes(s, 3, tag);
			CheckC64Codes(tag, "SwZj", 4);

			C64Codec::EncodeBytes(s, 4, tag);
			CheckC64Codes(tag, "SwZjS2", 6);

			C64Codec::EncodeBytes(s, 5, tag);
			CheckC64Codes(tag, "SwZjSw3", 7);

			C64Codec::EncodeBytes(s, 6, tag);
			CheckC64Codes(tag, "SwZjSwZj", 8);
		}
	}

	static void CheckC64Decode(const std::vector<BYTE>& codes, const BYTE* pBytes, size_t cb)
	{
		TEST_ASSERT(codes.size() == cb);
		for (size_t i = 0; i < cb; ++i)
			TEST_ASSERT(codes[i] == pBytes[i]);
	}
	static void TestDecodeBytes()
	{
		BYTE	s[] = { 0x9C, 0x3E, 0xB6, 0x9C, 0x3E, 0xB6 };

		std::vector<BYTE> tag;

		C64Codec::DecodeBytes("S2", 2, tag);
		CheckC64Decode(tag, s, 1);

		C64Codec::DecodeBytes("Sw3", 3, tag);
		CheckC64Decode(tag, s, 2);

		C64Codec::DecodeBytes("SwZj", 4, tag);
		CheckC64Decode(tag, s, 3);

		C64Codec::DecodeBytes("SwZjS2", 6, tag);
		CheckC64Decode(tag, s, 4);

		C64Codec::DecodeBytes("SwZjSw3", 7, tag);
		CheckC64Decode(tag, s, 5);

		C64Codec::DecodeBytes("SwZjSwZj", 8, tag);
		CheckC64Decode(tag, s, 6);
	}

	static size_t Rand()
	{
		static unsigned __int64 uRandSeed = 235347ul;
		unsigned __int64 temp = uRandSeed * 2654435761ul;
		uRandSeed = (temp >> 20) ;
		uRandSeed += 7ul;
		return (size_t)uRandSeed;
	}
	static void TestCodec()
	{
		for (size_t n = 1; n < 1000; ++n)
		{
			std::vector<BYTE> src(n);
			size_t rw = n >> 2;
			size_t r, c;
			size_t x;

			for (r = 0; r < rw; ++r)
			{
				x = Rand();
				for (size_t c = 0; c < 4; ++c)
				{
					src[r * 4 + c] = x & 0xFF;
					x >>= 8;
				}
			}
			x = Rand();
			for (c = 0; c < n - (rw << 2); ++c)
			{
				src[c] = x & 0xFF;
				x >>= 8;
			}

			std::vector<char> enc;
			std::vector<BYTE> dec;
			C64Codec::EncodeBytes(&src.front(), src.size(), enc);
			C64Codec::DecodeBytes(&enc.front(), enc.size(), dec);

			TEST_ASSERT(dec.size() == src.size());
			for (size_t i = 0; i < n; ++i)
				TEST_ASSERT(src[i] == dec[i]);
		}
	}

} s_dummy;

#endif