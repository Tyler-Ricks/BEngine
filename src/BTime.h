#ifndef BTIME_H
#define BTIME_H

#include <cstdint>
#include <chrono>
#include <cstdio>

typedef uint64_t ticks_ns;
typedef uint64_t ticks_ms;
typedef double seconds_t;

constexpr size_t STAMP_SIZE_T = 16;

namespace BTime
{
	void init();
	ticks_ns now_ns();
	
	inline ticks_ms ns_to_ms(ticks_ns ns)
	{
		return ns / 1'000'000;
	}

	void format_stamp_ns(ticks_ns time_ns, char* stamp_buffer);
}
#endif // end BTIME_H