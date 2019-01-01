#pragma once

#include "core/common.h"
#include "core/debug/assert.h"
#include "core/utility/utility.h"
#include "core/utility/cstring_utility.h"

#define LOGF(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::FATAL,   true, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGE(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::ERROR,   true, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGW(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::WARNING, true, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGI(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::INFO,    true, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGD(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::DEBUG,   true, __FILE__, __LINE__, MSG, ##__VA_ARGS__)

#define LOGF_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::FATAL,   false, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGE_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::ERROR,   false, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGW_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::WARNING, false, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGI_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::INFO,    false, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
#define LOGD_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::DEBUG,   false, __FILE__, __LINE__, MSG, ##__VA_ARGS__)

namespace Ant {
namespace core {

enum class LOGLEVEL : u8 {FATAL, ERROR, WARNING, INFO, DEBUG};

namespace {
	static constexpr char FORMAT_CHAR = '%';

	size_t __count_format_char (const char* str) {
		size_t count = 0;
		int length = strlength(str);
		CASSERT(length != -1);

		for (int i = 0; i < length; ++i) {
			if (str[i] == FORMAT_CHAR) {
				if (i + 1 < length && str[i + 1] == FORMAT_CHAR) continue;
				else ++count;
			}
		}

		return count;
	}

	[[maybe_unused]]
	void __formated_log_helper(FILE* stream, char* curr) {
		fputs(curr, stream);
	}

	template<typename Head, typename ...Tail>
	void __formated_log_helper (FILE* stream, char* curr, Head&& head, Tail&& ...tail) {
		if(*curr == '\0') return;

		while (*curr != '%') {
			fputc(*curr, stream);
			++curr;
		}

		if (*curr == FORMAT_CHAR) {
			if (*(curr + 1) == FORMAT_CHAR) {
				fputc(FORMAT_CHAR, stream);
				++curr;
			}

			else
				fputs(to_string(head).get_data(), stream);
		}

		__formated_log_helper(stream, ++curr, forward<Tail>(tail)...);
	}

	template<typename ...Args>
	void __formated_log (FILE* stream, const char* format_string, Args&& ...args) {
		ASSERT(__count_format_char(format_string) == sizeof...(Args), "Wrong number of formating characters in format string");
		
		__formated_log_helper(stream, const_cast<char*>(format_string), forward<Args>(args)...);
	}
}

template<typename ...Args>
void Log (LOGLEVEL level, bool write_msg_header, const char* file, unsigned int line, const char* format_string, Args ...args) {
	FILE* stream = stdout;

	// lock_guard
	if (write_msg_header) {
		switch(level) {
			case LOGLEVEL::DEBUG:
				fputs("[DEBUG] ", stream);
				break;

			case LOGLEVEL::INFO:
				fputs("[INFO] ", stream);
				break;

			case LOGLEVEL::FATAL:
				stream = stderr;
				fputs("[FATAL] ", stream);
				break;

			case LOGLEVEL::ERROR:
				stream = stderr;
				fputs("[ERROR] ", stream);
				break;

			case LOGLEVEL::WARNING:
				fputs("[WARNING] ", stream);
				break;
		}
		
		#ifdef ANT_PLATFORM_UNIX
			size_t pos = find_last_of(file, '/');
		#else
			size_t pos = find_last_of(file, '\\');
		#endif

		CASSERT(pos != npos);

		fprintf(stream, "%s @ %u ", file + pos + 1, line);
	}

	__formated_log(stream, format_string, forward<Args>(args)...);
	fputc('\n', stream);

	if (level == LOGLEVEL::FATAL)
		std::abort();
}

}
}
