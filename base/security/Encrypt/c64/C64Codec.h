#pragma once

extern const UINT BITS_MASK[];

class C64Codec
{
public:
	static size_t CalcEncodedSize(size_t cbPlain);
	static size_t CalcDecodedSize(size_t cbCrypt);

	static char EncodeByte(BYTE cc);
	static BYTE DecodeByte(char c64);

	static void EncodeBytes(const BYTE* src, size_t cb, std::vector<char>& tag);
	static void DecodeBytes(const char* src, size_t cb, std::vector<BYTE>& tag);
};

// 将 0-63 间的值转换为 C64 字符
inline char C64Codec::EncodeByte(BYTE cc)
{
	ASSERT(cc >= 0 && cc < 64);
	if (cc < 10)
		return cc + '0';
	else if (cc < 36)
		return cc + 'A' - 10;
	else if (cc < 62)
		return cc + 'a' - 36;
	else if (cc == 62)
		return '-';
	else
	{
		ASSERT(cc == 63);
		return '_';
	}
}

// 将 C64 字符转换为数值
inline BYTE C64Codec::DecodeByte(char c64)
{
	if (c64 >= '0' && c64 <= '9')
		return c64 - '0';
	else if (c64 >= 'A' && c64 <='Z')
		return c64 - 'A' + 10;
	else if (c64 >= 'a' && c64 <= 'z')
		return c64 -'a' + 36;
	else if (c64 == '-')
		return 62;
	else
	{
		ASSERT(c64 == '_');
		return 63;
	}
}

// 计算对应的密文大小
inline size_t C64Codec::CalcEncodedSize(size_t cbPlain)
{
	return (cbPlain * 4 + 2) / 3;
}

// 计算对应的明文大小
inline size_t C64Codec::CalcDecodedSize(size_t cbCrypt)
{
	ASSERT((cbCrypt & 3) != 1);
	size_t r = (cbCrypt >> 2) * 3;
	cbCrypt &= 3;
	return r + (cbCrypt != 0 ? cbCrypt - 1 : 0);
}
