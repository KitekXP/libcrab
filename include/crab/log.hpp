#pragma once

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>

#include <crab/io.hpp>

#define CRAB_LOG_INFO "[INFO] "
#define CRAB_LOG_WARN "[WARN] "
#define CRAB_LOG_ERROR "[ERROR] "
#define CRAB_LOG_DEBUG "[DEBUG] "



namespace crab {
    class log {
    private:
        FILE *log_out;
        int8_t verbosity;

        // Helper proxy for << stream operators
        enum Level : int8_t { ERR = 0, INFO = 1, WARN = 2, DBG = 3 };

        template <int8_t MinLevel>
        struct LogStream {
            log &logger;
            const char *prefix;

            LogStream(log &l, const char *p) : logger(l), prefix(p) {}

            template <typename T>
            LogStream &operator<<(const T &msg) {
                if (logger.verbosity >= MinLevel) {
                    crab::FP2Str(logger.log_out) << prefix << msg;
                }
                return *this;
            }

            // support stream manipulators like std::endl
            LogStream &operator<<(std::ostream& (*manip)(std::ostream&)) {
                if (logger.verbosity >= MinLevel) {
                    crab::FP2Str(logger.log_out) << manip;
                }
                return *this;
            }
        };

    public:
        log() : log_out(stdout), verbosity(1) {}

        // Overload >> to set output file pointer
        log &operator>>(FILE *fptr) {
            log_out = fptr;
            return *this;
        }

        // Set verbosity level
        log &set_verbosity(const int8_t level) {
            verbosity = level;
            return *this;
        }

        // Methods returning stream proxies for each severity
        LogStream<INFO> info() { return LogStream<INFO>(*this, CRAB_LOG_INFO); }
        LogStream<WARN> warn() { return LogStream<WARN>(*this, CRAB_LOG_WARN); }
        LogStream<ERR> err() { return LogStream<ERR>(*this, CRAB_LOG_ERROR); }
        LogStream<DBG> dbg() { return LogStream<DBG>(*this, CRAB_LOG_DEBUG); }
    };
}