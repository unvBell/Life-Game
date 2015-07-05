#pragma once

#include <memory>
#include <unordered_map>
#include "Chunk.hpp"

class IRenderer;

class World {
	struct Point {
		int x, y;

		bool operator==(Point p) const {
			return x == p.x && y == p.y;
		}
	};

	struct Hasher {
		size_t operator()(Point p) const {
			return p.x ^ p.y;
		}
	};
	
	using Chunks	= std::unordered_map<Point, Chunk, Hasher>;
	using Renderer	= std::unique_ptr<IRenderer>;

	Chunks		chunks_, buf_;
	Renderer	renderer_;

	World(const World&) =delete;
	World& operator=(const World&) =delete;

public:
	explicit World(Renderer renderer);

	void update();
	void render() const;

	bool get(int x, int y) const;
	void set(int x, int y, bool state);
};
