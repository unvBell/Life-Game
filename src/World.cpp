#include "World.hpp"
#include "Chunk.hpp"
#include "IRenderer.hpp"

World::World(World::Renderer renderer)
	: renderer_(std::move(renderer))
{
	assert(renderer_);

	set(30, 20, true);
}

void World::update() {
	for(auto& x : chunks_) {
		int offX = x.first.x * chunkSize;
		int offY = x.first.y * chunkSize;
		bool active = false;

		for(int j=0; j<chunkSize; j++) {
			for(int i=0; i<chunkSize; i++) {
				int cnt = 0;
				cnt += get(offX+i-1, offY+j-1);
				cnt += get(offX+i  , offY+j-1);
				cnt += get(offX+i+1, offY+j-1);
				cnt += get(offX+i-1, offY+j  );
				cnt += get(offX+i+1, offY+j  );
				cnt += get(offX+i-1, offY+j+1);
				cnt += get(offX+i  , offY+j+1);
				cnt += get(offX+i+1, offY+j+1);

				if((x.second.get(i, j) && cnt == 2) || cnt == 3) {
					active = true;
					buf_[x.first].set(i, j, true);
				}
			}
		}
		
		if(active) {
			buf_[{ x.first.x-1, x.first.y   }];
			buf_[{ x.first.x+1, x.first.y   }];
			buf_[{ x.first.x  , x.first.y-1 }];
			buf_[{ x.first.x  , x.first.y+1 }];
		}
	}

	chunks_.swap(buf_);
	buf_.clear();
}


void World::render() const {
	for(auto&& x : chunks_) {
		renderer_->renderChunk(x.first.x, x.first.y, x.second);
	}
}


bool World::get(int x, int y) const {
	int cx = (int)floor(1.*x/chunkSize);
	int cy = (int)floor(1.*y/chunkSize);

	x -= cx * chunkSize;
	y -= cy * chunkSize;

	auto it = chunks_.find({ cx, cy });
	if(it == chunks_.cend())
		return false;

	return it->second.get(x, y);
}


void World::set(int x, int y, bool state) {
	int cx = (int)floor(1.*x/chunkSize);
	int cy = (int)floor(1.*y/chunkSize);

	x -= cx * chunkSize;
	y -= cy * chunkSize;

	chunks_[{ cx-1, cy }];
	chunks_[{ cx+1, cy }];
	chunks_[{ cx, cy-1 }];
	chunks_[{ cx, cy+1 }];

	chunks_[{ cx, cy }].set(x, y, state);
}
