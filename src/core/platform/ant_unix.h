#pragma once

#include "core/common.h"
#include "core/compiler_definitions.h"
#include "core/utility/cstring_utility.h"

namespace Ant{
namespace core {

inline
void GetEnginePath (char* buffer, unsigned int buffer_size) {
	char process[1024];
	char exepath[1024];

	snprintf(process, 1024, "/proc/%d/exe", getpid());
	ssize_t ret = readlink(process, exepath, 1024);

	if (ret == -1) {
		fprintf(stderr, "[FATAL] Error encountered whilst trying to get path of executable. Reason: %s\n", strerror(errno));
		std::abort();
	}

	size_t index = find_last_of(exepath, '/');
	if (index == npos) {
		fputs("[FATAL] Error encountered whilst trying to get path of executable. Reason: path does not contain a single '/'", stderr);
		std::abort();
	}

	if (index > buffer_size) {
		fprintf(stderr, "[FATAL] Error encountered whilst trying to get path of executable. Reason: passed buffer is too short to contain complete path, buffer size: %u, path length: %lu", buffer_size, index);
		std::abort();
	}

	memcpy(buffer, &exepath, index + 1);
	buffer[index + 1] = '\0';
}

}
}
