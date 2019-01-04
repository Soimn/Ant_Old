#pragma once

#include "core/common.h"
#include "core/utility/cstring_utility.h"

#ifdef ANT_DEBUG
#define CASSERT(EX) (void)(EX ? nullptr : ::Ant::core::__assertion_failed(#EX, "", __FILE__, __LINE__))
#define ASSERT(EX, MSG) (void)(EX ? nullptr : ::Ant::core::__assertion_failed(#EX, MSG, __FILE__, __LINE__))
#define FASSERT(EX, FORM, ...) (void)(EX ? nullptr : ::Ant::core::__assertion_failed_formated(#EX, FORM, __FILE__, __LINE__, ##__VA_ARGS__))
#else
#define CASSERT(EX) ((void) 0)
#define ASSERT(EX, MSG) ((void) 0)
#define FASSERT(EX, FORM) ((void) 0)
#endif

namespace Ant {
namespace core {

void* __assertion_failed(const char* ex, const char* msg, const char* file, unsigned int line) {
	#ifdef ANT_PLATFORM_UNIX
	const char target = '/';
	#elif
	const char target = '\\';
	#endif
	
	const char* filename = file + find_last_of(file, target) + 1;
	
	fputc('\n', stderr);
	fputs("**************************************************************\n", stderr);
	fputs("|                      ASSERTION FAILED                      |\n", stderr);
	fputs("**************************************************************\n", stderr);
	fprintf(stderr, "File: %s   Line: %d\n", filename, line);
	fprintf(stderr, "Expression: %s\n", ex);
	if (strlength(msg) != 0)
		fprintf(stderr, "%s\n", msg);
	fputs("\n\n", stderr);

	std::abort();

	return reinterpret_cast<void*>(&__assertion_failed);
}

template<typename ...Args>
void* __assertion_failed_formated(const char* ex, const char* format_string, const char* file, unsigned int line, Args ...args) {
	char buffer[256];

	#ifdef ANT_COMPILER_GCC
	#pragma GCC diagnostic ignored "-Wformat-nonliteral"
	#endif

	if (snprintf(buffer, 256, format_string, args...))
		__assertion_failed(ex, buffer, file, line);

	else
		printf("Failed to print formating string from assertion of expression %s in file %s at line %u", ex, file, line);
	
	#ifdef ANT_COMPILER_GCC
	#pragma GCC diagnostic warning "-Wformat-nonliteral"
	#endif
	
	return reinterpret_cast<void*>(&__assertion_failed_formated<Args...>);
}

}
}
