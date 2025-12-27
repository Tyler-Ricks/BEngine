#ifndef ASSERTS_H
#define ASSERTS_H

#include <cstdint>

#define ASSERTIONS_ENABLED


#ifdef ASSERTIONS_ENABLED

void report_assertion_failure(const char* expression,
	const char* message,
	const char* file,
	int32_t line);

#define BASSERT(expr)														\
{																			\
																			\
		if (expr) {}														\
		else																\
		{																	\
			report_assertion_failure(#expr, "", __FILE__, __LINE__);		\
			__debugbreak();													\
	}																		\
}																			\


#define BASSERT_MSG(expr, message)											\
{																			\
																			\
		if (expr) {}														\
		else																\
		{																	\
			report_assertion_failure(#expr, message, __FILE__, __LINE__);	\
			__debugbreak();													\
	}																		\
}																			\

#ifdef _DEBUG
#define BASSERT_DEBUG(expr)													\
{																			\
		if (expr) {}														\
		else																\
		{																	\
			report_assertion_failure(#expr, "", __FILE__, __LINE__);		\
			__debugbreak();													\
	}																		\
}																			\


#else
#define BASSERT_DEBUG(expr)

#endif // end _DEBUG
#endif // end ASSERTIONS_ENABLED
#endif // end ASSERTS_H