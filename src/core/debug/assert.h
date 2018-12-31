#pragma once

#include "core/common.h"

#ifdef ANT_DEBUG
#define CASSERT(EX) (void)(EX ? nullptr : ::Ant::core::__assertion_failed(#EX, "", __FILE__, __LINE__))
#define ASSERT(EX, MSG) (void)(EX ? nullptr : ::Ant::core::__assertion_failed(#EX, MSG, __FILE__, __LINE__))
#else
#define CASSERT(EX) ((void) 0)
#define ASSERT(EX, MSG) ((void) 0)
#endif

namespace Ant {
namespace core {

inline void* __assertion_failed(const char* ex, const char* msg, const char* file, unsigned int line) {
	unsigned int last;
	for (unsigned int i = 0; i < strlen(file); ++i) {
		#ifdef ANT_PLATFORM_UNIX
			if (file[i] == '/')
		#else
			if (file[i] == '\\')
		#endif
		
		last = i;
	}

	const char* filename = file + last + 1;
	
	fputc('\n', stdout);
	fputs("**************************************************************\n", stdout);
	fputs("|                      ASSERTION FAILED                      |\n", stdout);
	fputs("**************************************************************\n", stdout);
	fprintf(stdout, "File: %s   Line: %d\n", filename, line);
	fprintf(stdout, "Expression: %s\n", ex);
	if (strlen(msg) != 0)
		fprintf(stdout, "%s\n", msg);
	fputs("\n\n", stdout);

	std::abort();

	return reinterpret_cast<void*>(&__assertion_failed);
}

}
}
