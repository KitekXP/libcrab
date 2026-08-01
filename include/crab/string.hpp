#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include <crab/uint.hpp>

namespace crab {
	template <typename T>
	std::vector<T> ptr2vec(const T* array, uint size) {
		std::vector<T> vec = std::vector<T>(array, array + size);
		return vec;
	}
}