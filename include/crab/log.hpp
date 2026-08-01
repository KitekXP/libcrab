#pragma once

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
	template <int8_t MinVerbosity>
	struct LogStream {
		log &logger;
		const char *prefix;

		template <typename T>
		LogStream &operator<<(const T &msg) {
			if (logger.verbosity > MinVerbosity) {
				crab::FP2Str(logger.log_out) << prefix << msg;
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

        // Note: Renamed from verbosity() to avoid colliding with the verbosity variable
        void set_verbosity(const int8_t level) {
            verbosity = level;
        }

        // Stream objects for each severity using your updated names
        LogStream<0> inf{*this, CRAB_LOG_INFO};
        LogStream<1> warn{*this, CRAB_LOG_WARN};
        LogStream<-2> err{*this, CRAB_LOG_ERROR};
        LogStream<2> dbg{*this, CRAB_LOG_DEBUG};
    };
}