#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <type_traits>

#include <termios.h>
#include <unistd.h>



namespace crab {
	namespace term {
		struct termios get_in();
		int set_in(struct termios t);
		struct termios get_out();
		int set_termout(struct termios t);
		struct termios get_err();
		int set_err(struct termios t);
		void flag_raw(struct termios &t);
	}

	std::string prompt(std::string prompt_str, size_t length = 128, bool silent = false, char mask = '\0');

	class FileStream {
	private:
		FILE *file;
	
	public:
		explicit FileStream(FILE *f);

		FileStream& operator<<(const char *s);
		FileStream& operator<<(const std::string &s);
		FileStream& operator<<(char c);
		FileStream& operator<<(bool b);

		// Templates must remain in the header
		template <typename T, typename = typename std::enable_if<
			std::is_arithmetic<T>::value && 
			!std::is_same<T, char>::value &&
			!std::is_same<T, wchar_t>::value &&
			!std::is_same<T, bool>::value
		>::type>
		FileStream& operator<<(T val) {
			if (file) {
				std::string s = std::to_string(val);
				std::fputs(s.c_str(), file);
			}
			return *this;
		}
	};

	FileStream FP2Str(FILE *f);
}