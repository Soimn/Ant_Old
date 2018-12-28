#pragma once

#include "core/common.h"
#include "core/utility/utility.h"
#include "core/containers/string.h"

#define LOGF(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::FATAL,   true, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGE(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::ERROR,   true, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGW(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::WARNING, true, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGI(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::INFO,    true, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGD(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::DEBUG,   true, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)

#define LOGF_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::FATAL,   false, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGE_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::ERROR,   false, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGW_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::WARNING, false, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGI_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::INFO,    false, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)
#define LOGD_HL(MSG, ...) ::Ant::core::Log(::Ant::core::LOGLEVEL::DEBUG,   false, __FILE__, __LINE__, #MSG, ##__VA_ARGS__)

namespace Ant {
namespace core {

namespace {
	static constexpr char FORMAT_CHAR = '%';

	template<int N, typename Tag>
	constexpr size_t __count_format_char (const fixed_string<N, Tag>& str) {
		size_t count = 0;
		int length = strlength(str);
		// assert(length != -1);

		for (int i = 0; i < length; ++i) {
			if (str[i] == FORMAT_CHAR) {
				if (i + 1 < length && str[i + 1] == FORMAT_CHAR) continue;
				else ++count;
			}
		}

		return count;
	}

	template<typename Head, typename ...Tail>
	void __formated_log_helper (FILE* stream, char curr, Head&& head, Tail&& ...tail) {
		if (curr == '\0') return;

		if (curr == FORMAT_CHAR) {
			if (*(&curr + 1) == FORMAT_CHAR) {
				fputc(FORMAT_CHAR, stream);
				curr = *(&curr + 1);
			}

			else
				fputs(to_string(head), stream);
		}

		__formated_log_helper(*(&curr + 1), forward<Tail>(tail)...);
	}

	template<int N, typename Tag, typename ...Args>
	inline void __formated_log (FILE* stream, const fixed_string<N, Tag>& format_string, Args&& ...args) {
		static_assert(__count_format_char(format_string) == sizeof...(Args));
		// assert(strlength(str) != -1)
		
		__formated_log_helper(stream, format_string, forward<Args>(args)...);
	}

	template<int N, typename Tag>
	inline void __formated_log (FILE* stream, const fixed_string<N, Tag>& fstr) {
		fputs(fstr.get_data(), stream);
	}
}

enum class LOGLEVEL : u8 {FATAL, ERROR, WARNING, INFO, DEBUG};

template<int N, typename Tag, typename ...Args>
void Log (LOGLEVEL level, bool write_msg_header, const char* file, unsigned int line, const fixed_string<N, Tag>& format_string, Args ...args) {
	FILE* stream = stdout;
	fixed_array_string<7> fstr;

	switch(level) {
		case LOGLEVEL::DEBUG:
			fstr = "DEBUG";
			break;

		case LOGLEVEL::INFO:
			fstr = "INFO";
			break;

		case LOGLEVEL::FATAL:
			stream = stderr;
			fstr = "FATAL";
			break;

		case LOGLEVEL::ERROR:
			fstr = "ERROR";
			break;

		case LOGLEVEL::WARNING:
			fstr = "WARNING";
			break;
	}

	#ifdef ANT_PLATFORM_UNIX
		const char* filename = file + find_last_of(fixed_string(file), '/');
	#else
		const char* filename = file + find_last_of(fixed_string(file), '\\');
	#endif

	// lock_guard
	if (write_msg_header)
		fprintf(stream, "[%s] %s @ %u ", fstr, filename, line);
	
	__formated_log(stream, format_string, forward<Args>(args)...);
	fputc('\n', stream);
}

}
}
