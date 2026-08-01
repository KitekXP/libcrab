#include <crab/var.hpp>

namespace crab {
        template <typename T>
    std::vector<T> ptr2vec(const T* array, uint size) {
        std::vector<T> vec;
        if (!array || size > vec.max_size()) return vec;
        vec.assign(array, array + size); // High-speed memory copy constructor
        return vec;
    }
}