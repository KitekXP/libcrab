#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>

#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>

#include "crab/uint.hpp"

namespace crab {
	namespace fs {
		uid_t uid(const std::string path);
		gid_t gid(const std::string path);
		std::string owner(const std::string path);
		std::string group(const std::string path);
		int dec_perms(const std::string path);
		int set_perms(const std::string path, const uint perms);
		int set_owner(const std::string path, const std::string owner);
		int set_group(const std::string path, const std::string group);
		int create_file(const std::string path, const uint perms = 0644);
		int create_dir(const std::string path, const uint perms = 0755);
		int remove_file(const std::string path);
		int remove_dir(const std::string path);
		int copy_file(const std::string src, const std::string dst);
		int copy_dir(const std::string src, const std::string dst);
		int install(const std::string src, const std::string dst, const uint perms = 0755, const std::string owner = "", const std::string group = "");
	}
	namespace posix {
		int read(const std::string path, std::vector<std::string> *buf, const size_t size);
		ssize_t write(const std::string path, const char *buf, const size_t size);
	}
	namespace fifo {
		int create(const std::string path, const uint perms = 0666);
		int remove(const std::string path);
		using posix::read;
		using posix::write;
	}
	namespace chrdev {
		int create(const std::string path, const uint major, const uint minor);
		int remove(const std::string path);
		using posix::read;
		using posix::write;
	}
}