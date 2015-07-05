#pragma once

#include <array>
#include <cassert>

const int chunkSize = 16;

class Chunk final {
	using Array		= std::array<bool , chunkSize>;
	using Array2D	= std::array<Array, chunkSize>;

	Array2D cells_;

public:
	explicit Chunk() {
		for(Array& x : cells_)
			x.fill(false);
	}

	bool get(int x, int y) const {
		assert(0 <= x && x < chunkSize);
		assert(0 <= y && y < chunkSize);

		return cells_[y][x];
	}

	void set(int x, int y, bool state) {
		assert(0 <= x && x < chunkSize);
		assert(0 <= y && y < chunkSize);

		cells_[y][x] = state;
	}
};
