#include "assert.h"

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

}
}
