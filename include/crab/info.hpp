#pragma once

#include <string>
#include <vector>

#include <unistd.h>
#include <grp.h>
#include <pwd.h>

namespace crab {
    namespace usr {
        std::string name();
        uid_t uid(const std::string username);
        gid_t gid(const std::string username);
        std::string home(const std::string username);
        std::string shell(const std::string username);   
    }
    namespace uid {
        uid_t uid();
        gid_t gid(const uid_t uid);
        std::string name(const uid_t uid);
        std::string home(const uid_t uid);
        std::string shell(const uid_t uid);
    }
    namespace grp {
        std::string name();
        gid_t gid(const std::string groupname);
        std::vector<std::string> members(const std::string groupname);
    }
    namespace gid {
        gid_t gid();
        std::string name(const gid_t gid);
        std::vector<std::string> members(const gid_t gid);
    }

}