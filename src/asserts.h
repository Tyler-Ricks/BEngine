#ifndef ASSERTS_H
#define ASSERTS_H

#include <cstdint>
#include <cstdlib>
#include "logger.h"

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define DEBUG_BREAK() __builtin_trap()
#else
    #define DEBUG_BREAK() std::abort()
#endif

namespace BAssert
{
	inline void report_assertion_failure(const char* expression,
		const char* file,
		int32_t line,
		const char* func,
		const char* message = nullptr)
	{
		if (message != nullptr)
		{
			LOG_FATAL(
				"ASSERT FAILED: {}\nMessage: {}\nfile: {}:{}\nfunction: {}",
				expression, message, file, line, func);
		}
		else
		{
			LOG_FATAL(
				"ASSERT FAILED: {}\nfile: {}:{}\nfunction: {}",
				expression, file, line, func);
		}

		// this may be unnecessary
		Logger::get_instance().flush_log_queue();
		DEBUG_BREAK();
	}
} // namespace BAssert

#ifdef _DEBUG
#define ASSERTIONS_ENABLED
#ifdef ASSERTIONS_ENABLED

#define BASSERT(expr)														\
{																			\
	do																		\
	{																		\
		if(!expr)															\
		{																	\
			BAssert::report_assertion_failure(								\
				#expr, __FILE__, __LINE__, __func__)						\
		}																	\
	}																		\
	while(0)																\
}																			\


#define BASSERT_MSG(expr, message)											\
{																			\
	do																		\
	{																		\
		if(!expr)															\
		{																	\
			BAssert::report_assertion_failure(								\
				#expr, __FILE__, __LINE__, __func__, message)				\
		}																	\
	}																		\
	while(0)																\
}																			\
}																			\

#endif // end ASSERTIONS_ENABLED
#endif // end _DEBUG
#endif // end ASSERTS_H