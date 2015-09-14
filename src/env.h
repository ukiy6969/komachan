/// for MSVC
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1

#define strtok_r strtok_s
#define snprintf _snprintf

// Reference: http://blog.jiubao.org/2015/01/gcc-bitscanforward-bitscanreverse-msvc.html
#include <intrin.h>

_inline int __builtin_clz(unsigned int n)
{
	unsigned long index;
	_BitScanReverse(&index, n);
	return 31 - index;
}

_inline  int __builtin_ctz(unsigned int n)
{
	unsigned long index;
	_BitScanForward(&index, n);
	return index;
}

// for Cygwin(Original)
#else
#include <unistd.h>


#endif
