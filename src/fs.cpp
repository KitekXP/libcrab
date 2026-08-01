#include <crab/fs.hpp>



namespace crab {
	namespace fs {
		uid_t uid(const std::string path) {
			struct stat st;
			if (stat(path.c_str(), &st) != 0) {
				return static_cast<uid_t>(-1);
			}
			return st.st_uid;
		}

		gid_t gid(const std::string path) {
			struct stat st;
			if (stat(path.c_str(), &st) != 0) {
				return static_cast<gid_t>(-1);
			}
			return st.st_gid;
		}

		std::string owner(const std::string path) {
			uid_t u = crab::fs::uid(path);
			struct passwd *pw = getpwuid(u);
			if (!pw) {
				return "";
			}
			return pw->pw_name;
		}

		std::string group(const std::string path) {
			uid_t g = crab::fs::gid(path);
			struct group *gr = getgrgid(g);
			if (!gr) {
				return "";
			}
			return gr->gr_name;
		}

		int dec_perms(const std::string path) {
			struct stat st;
			if (stat(path.c_str(), &st) != 0) {
				return -1;
			}
			return st.st_mode & 07777;
		}
	
		int set_perms(const std::string path, const uint perms) {
			return chmod(path.c_str(), perms);
		}

		int set_owner(const std::string path, const std::string owner_name) {
			struct passwd *pw = getpwnam(owner_name.c_str());
			if (!pw) {
				return -1;
			}
			return chown(path.c_str(), pw->pw_uid, -1);
		}

		int set_group(const std::string path, const std::string group_name) {
			struct group *gr = getgrnam(group_name.c_str());
			if (!gr) {
				return -1;
			}
			return chown(path.c_str(), -1, gr->gr_gid);
		}

		int create_file(const std::string path, const uint perms) {
			FILE *fptr = std::fopen(path.c_str(), "w");
			if (!fptr) {
				return -1;
			}
			std::fclose(fptr);
			return crab::fs::set_perms(path, perms);
		}

		int create_dir(const std::string path, const uint perms) {
			return mkdir(path.c_str(), perms);
		}

		int remove_file(const std::string path) {
			return std::remove(path.c_str());
		}

		int remove_dir(const std::string path) {
			return rmdir(path.c_str());
		}

		int remove_recurse(const std::string path) {
			return std::filesystem::remove_all(path);
		}

		int hardlink(const std::string src, const std::string dst) {
			return link(src.c_str(), dst.c_str());
		}

		int link(const std::string src, const std::string dst) {
			return symlink(src.c_str(), dst.c_str());
		}

		std::string read_link(const std::string path) {
			char buf[PATH_MAX];
			ssize_t len = readlink(path.c_str(), buf, sizeof(buf) - 1);
			if (len != -1) {
				buf[len] = '\0';
				return std::string(buf);
			}
			return "";
		}

		std::string read_path(const std::string path) {
			return realpath(path.c_str(), NULL);
		}

		int exists(const std::string path) {
			return std::filesystem::exists(path);
		}

		int is_dir(const std::string path) {
			return std::filesystem::is_directory(path);
		}

		int is_file(const std::string path) {
			return std::filesystem::is_regular_file(path);
		}

		int is_link(const std::string path) {
			return std::filesystem::is_symlink(path);
		}

		int move(const std::string src, const std::string dst) {
			return std::rename(src.c_str(), dst.c_str());
		}

		int copy_file(const std::string src, const std::string dst) {
			std::ifstream src_file(src, std::ios::binary);
			if (!src_file.is_open()) {
				return -1;
			}
			std::ofstream dst_file(dst, std::ios::binary);
			if (!dst_file.is_open()) {
				return -1;
			}
			dst_file << src_file.rdbuf();
			return dst_file.good() ? 0 : -1;
		}

		int copy_dir(const std::string src, const std::string dst) {
			std::error_code ec;
			std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing, ec);
			return ec ? -1 : 0;
		}

		int install(const std::string src, const std::string dst, const uint perms, const std::string owner, const std::string group) {
			struct passwd *pw = getpwnam(owner.c_str());
			if (owner.empty()) {
				pw = getpwuid(getuid());
			}

			if (!pw) {
				return -1;
			}

			struct group *gr = getgrnam(group.c_str());
			if (group.empty()) {
				gr = getgrgid(pw->pw_gid);
			}

			if (!gr) {
				return -1;
			}

			int ret = crab::fs::copy_dir(src, dst);
			if (ret != 0) {
				return ret;
			}
			ret = crab::fs::set_perms(dst, perms);
			if (ret != 0) {
				return ret;
			}
			ret = crab::fs::set_owner(dst, pw->pw_name);
			if (ret != 0) {
				return ret;
			}
			return crab::fs::set_group(dst, gr->gr_name);
		}
	}

	namespace posix {
		int read(const std::string path, std::vector<std::string> *buf, const size_t size) {
			int filedesc = open(path.c_str(), O_RDONLY);
			if (filedesc < 0) {
				return filedesc;
			}

			char *internal_buf[size];
			int ret = read(path.c_str(), buf, size);
			if (ret < 0) {
				return ret;
			}

			buf->assign(internal_buf, internal_buf + size);

			ret = close(filedesc);
			if (ret < 0) {
				return ret;
			}
			return 0;
		}

		ssize_t write(const std::string path, const char *buf, const size_t size) {
			int fd = open(path.c_str(), O_WRONLY);
			if (fd < 0) {
				return fd;
			}

			ssize_t written = ::write(fd, buf, size);
			if (written < 0) {
				return written;
			}

			written = ::close(fd);
			if (written < 0) {
				return written;
			}

			return written;
		}
	}

	namespace fifo {
		int create(const std::string path, const uint perms) {
			return mkfifo(path.c_str(), perms);
		}

		int remove(const std::string path) {
			return unlink(path.c_str());
		}

		using posix::read;
		using posix::write;
	}

	namespace chrdev {
		int create(const std::string path, const uint major, const uint minor, const uint perms) {
			return mknod(path.c_str(), perms, makedev(major, minor));
		}

		int remove(const std::string path) {
			return unlink(path.c_str());
		}

		using posix::read;
		using posix::write;
	}
}