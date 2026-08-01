#pragma once

#include <cstdint>
#include <vector>

#include <crab/uint.hpp>

namespace crab {
        template <typename T>
    class vector2 {
    private:
        std::vector<std::vector<T>> vect;
    
    public:
        vector2(uint x, uint y) : vect(x, std::vector<T>(y)) {}

        static vector2 create() {
            return vector2(0, 0);
        }

        T get(uint x, uint y) const {
            return vect[x][y];
        }

        void set(uint x, uint y, T val) {
            vect[x][y] = val;
        }

        uint size_x() const {
            if (vect.empty()) {
                return 0;
            }
            return vect.size();
        }

        uint size_y() const {
            if (vect.empty()) {
                return 0;
            }
            return vect[0].size();
        }
    
    };

        template <typename T>
	class vector3 {
	private:
		std::vector<std::vector<std::vector<T>>> vect;
	
	public:
		vector3(uint x, uint y, uint z) : vect(x, std::vector<std::vector<T>>(y, std::vector<T>(z))) {}

		static vector3 create() {
			return vector3(0, 0, 0);
		}

		T get(uint x, uint y, uint z) const {
			return vect[x][y][z];
		}

		void set(uint x, uint y, uint z, T val) {
			vect[x][y][z] = val;
		}

		uint size_x() const {
			if (vect.empty()) {
				return 0;
			}
			return vect.size();
		}

		uint size_y() const {
			if (vect.empty() || vect[0].empty()) {
				return 0;
			}
			return vect[0].size();
		}

		uint size_z() const {
			if (vect.empty() || vect[0].empty() || vect[0][0].empty()) {
				return 0;
			}
			return vect[0][0].size();
		}
	};
}