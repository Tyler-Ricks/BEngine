#include "BTime.h"

namespace {
	std::chrono::steady_clock::time_point start;
}


void BTime::init()
{
	start = std::chrono::steady_clock::now();
}

ticks_ns BTime::now_ns()
{
	using namespace std::chrono;
	// for now, if start was not initialized, do do so
	if (start == steady_clock::time_point{}) { init(); }

	return duration_cast<nanoseconds>
		(steady_clock::now() -  start).count();
}


// this converts ns to ms, but maybe have more variants in the future
void BTime::format_stamp_ns(ticks_ns time_ns, char* stamp_buffer)
{
	snprintf(	stamp_buffer, STAMP_SIZE_T, 
				"%llu ms", (unsigned long long)ns_to_ms(time_ns));
}

