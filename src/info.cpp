#include <crab/info.hpp>

namespace crab {
    namespace usr {
        std::string name() {
            struct passwd *pw = getpwuid(getuid());
            if (!pw) {
                return "";
            }
            return pw->pw_name;
        }

        uid_t uid(const std::string username) {
            struct passwd *pw = getpwnam(username.c_str());
            if (!pw) {
                return -1;
            }
            return pw->pw_uid;
        }
        gid_t gid(const std::string username) {
            struct passwd *pw = getpwnam(username.c_str());
            if (!pw) {
                return -1;
            }
            return pw->pw_gid;
        }
        std::string home(const std::string username) {
            struct passwd *pw = getpwnam(username.c_str());
            if (!pw) {
                return "";
            }
            return pw->pw_dir;
        }
        std::string shell(const std::string username) {
            struct passwd *pw = getpwnam(username.c_str());
            if (!pw) {
                return "";
            }
            return pw->pw_shell;
        }
    }

    namespace uid {
        uid_t uid() {
            return getuid();
        }

        gid_t gid(const uid_t uid) {
            struct passwd *pw = getpwuid(uid);
            if (!pw) {
                return -1;
            }
            return pw->pw_gid;
        }
        std::string name(const uid_t uid) {
            struct passwd *pw = getpwuid(uid);
            if (!pw) {
                return "";
            }
            return pw->pw_name;
        }
        std::string home(const uid_t uid) {
            struct passwd *pw = getpwuid(uid);
            if (!pw) {
                return "";
            }
            return pw->pw_dir;
        }
        std::string shell(const uid_t uid) {
            struct passwd *pw = getpwuid(uid);
            if (!pw) {
                return "";
            }
            return pw->pw_shell;
        }
    }

    namespace grp {
        std::string name() {
            struct group *gr = getgrgid(getgid());
            if (!gr) {
                return "";
            }
            return gr->gr_name;
        }

        gid_t gid(const std::string groupname) {
            struct group *gr = getgrnam(groupname.c_str());
            if (!gr) {
                return -1;
            }
            return gr->gr_gid;
        }
        std::vector<std::string> members(const std::string groupname) {
            struct group *gr = getgrnam(groupname.c_str());
            std::vector<std::string> members;
            if (!gr || !gr->gr_mem) {
                return members;
            }
            
            uint size = 0;
            while (gr->gr_mem[size] != NULL) {
                size++;
            }

            uint final_size = size;
            if (size > 0) {
                final_size = size - 1;
            }

            members.assign(gr->gr_mem, gr->gr_mem + final_size);

            return members;
        }
    }

    namespace gid {
        gid_t gid() {
            return getgid();
        }

        std::string name(const gid_t gid) {
            struct group *gr = getgrgid(gid);
            if (!gr) {
                return "";
            }
            return gr->gr_name;
        }
        std::vector<std::string> members(const gid_t gid) {
            struct group *gr = getgrgid(gid);
            std::vector<std::string> members;
            if (!gr || !gr->gr_mem) {
                return members;
            }
            
            uint size = 0;
            while (gr->gr_mem[size] != NULL) {
                size++;
            }

            uint final_size = size;
            if (size > 0) {
                final_size = size - 1;
            }

            members.assign(gr->gr_mem, gr->gr_mem + final_size);

            return members;
        }
    }
}