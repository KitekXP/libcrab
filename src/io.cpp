#include <crab/io.hpp>

namespace crab {
    namespace term {
        struct termios get_in() {
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            return t;
        }

        int set_in(struct termios t) {
            return tcsetattr(STDIN_FILENO, TCSANOW, &t);
        }

        struct termios get_out() {
            struct termios t;
            tcgetattr(STDOUT_FILENO, &t);
            return t;
        }

        int set_termout(struct termios t) {
            return tcsetattr(STDOUT_FILENO, TCSANOW, &t);
        }
    
        struct termios get_err() {
            struct termios t;
            tcgetattr(STDERR_FILENO, &t);
            return t;
        }

        int set_err(struct termios t) {
            return tcsetattr(STDERR_FILENO, TCSANOW, &t);
        }

        void flag_raw(struct termios &t) {
            t.c_lflag &= ~ECHO;
            t.c_lflag &= ~ICANON;
        }
    }

    std::string prompt(std::string prompt_str = "", size_t length, bool silent, char mask) {
        std::cout << prompt_str;
        std::fflush(stdout);

        std::string buffer;
        char c;

        struct termios og  = crab::term::get_in();
        struct termios mod = og;
        
        crab::term::flag_raw(mod);
        crab::term::set_in(mod);

        while (buffer.size() < length) {
            c = std::cin.get();

            if (c == '\n' || c == '\r' || c == '\0') {
                std::cout << '\n';
                std::fflush(stdout);
                break;
            }

            if (c == '\b' || c == 127) {
                if (!buffer.empty()) {
                    buffer.pop_back();
                    if (!silent) {
                        std::cout << "\b \b";
                        std::fflush(stdout);
                    }
                }
                continue;
            }

            buffer.push_back(c);

            if (!silent) {
                std::cout << (mask != '\0' ? mask : c);
                std::fflush(stdout);
            }
        }

        crab::term::set_in(og);

        return buffer;
    }

    FileStream::FileStream(FILE *f) : file(f) {}

	FileStream& FileStream::operator<<(const char *s) {
		if (file && s) {
			std::fputs(s, file);
		}
		return *this;
	}

	FileStream& FileStream::operator<<(const std::string &s) {
		if (file) {
			std::fputs(s.c_str(), file);
		}
		return *this;
	}

	FileStream& FileStream::operator<<(char c) {
		if (file) {
			std::fputc(c, file);
		}
		return *this;
	}

	FileStream& FileStream::operator<<(bool b) {
		if (file) {
			if (b) {
				std::fputs("true", file);
			} else {
				std::fputs("false", file);
			}
		}
		return *this;
	}

	FileStream FP2Str(FILE *f) {
		return FileStream(f);
	}

    int push(FILE *fptr) {
        return std::fflush(fptr);
    }
}