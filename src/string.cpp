#include <crab/string.hpp>

namespace crab {
    namespace str {
        std::string subst(std::string str, std::string from, std::string to) {
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
            return str;
        }

        std::string to_lower(std::string str) {
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            return str;
        }

        std::string to_upper(std::string str) {
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
            return str;
        }

        std::vector<std::string> split(std::string str, char delim) {
            std::vector<std::string> strings;
            char c;

            uint i = 0;
            while (i < str.length()) {
                c = str[i];
                if (c == delim) {
                    strings.push_back(str.substr(0, i));
                    str = str.substr(i + 1);
                    i = 0;
                    continue;
                }
                i++;
            }

            return strings;
        }
    }
}