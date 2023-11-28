#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::SetBoundary(glm::vec2 lowerleftcorner, glm::vec2 size)
{
	LowerLeftCorner_ = lowerleftcorner;
	UpperRightCorner_ = size + lowerleftcorner;

	GridColNum_ = floor(size.y / SupportRadius_);
	GridRowNum_ = floor(size.x / SupportRadius_);
	SingleGridSize_ = glm::vec2(size.x / GridColNum_, size.y / GridRowNum_);

	Position_.clear();
	Velocity_.clear();
	Acceleration_.clear();
}

void ParticleSystem::AddFluidParticles(glm::vec2 lowerleftcorner, glm::vec2 size, glm::vec2 initial_velocity, float resolution1D)
{
	glm::vec2 upperrightcorner = lowerleftcorner + size;
	if (upperrightcorner.x > UpperRightCorner_.x ||
		upperrightcorner.y > UpperRightCorner_.y ||
		lowerleftcorner.x < LowerLeftCorner_.x ||
		lowerleftcorner.y < LowerLeftCorner_.y) {
		std::cout << "Fluid Out Of Boundary!" << std::endl;
		return;
	}

	int width = size.x / resolution1D;
	int height = size.y / resolution1D;

	std::vector<glm::vec2> position(width * height);
	std::vector<glm::vec2> velocity(width * height, initial_velocity);
	std::vector<glm::vec2> acceleration(width * height);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int index = j * height + i;
			position[index] = lowerleftcorner +
				glm::vec2((i + 0.5) * resolution1D, (j + 0.5) * resolution1D);
			if (j % 2 == 0) {
				position[index].x += ParticleRadius_;
			}
		}
	}

	Position_.insert(Position_.end(), position.begin(), position.end());
	Velocity_.insert(Velocity_.end(), velocity.begin(), velocity.end());
	Acceleration_.insert(Acceleration_.end(), acceleration.begin(), acceleration.end());
	// update the particle number
	ParticleCount_ = Position_.size();
}

void ParticleSystem::SearchNeighbors()
{
	BuildHashTable();
	Neighbors_ = std::vector<NeighborList>(ParticleCount_, NeighborList(0));

	for (int i = 0; i < ParticleCount_; i++) {
		glm::vec2 relativePostion = Position_[i] - LowerLeftCorner_;
		int ColumnIndex = floor(relativePostion.x / SingleGridSize_.x);
		int RowIndex = floor(relativePostion.y / SingleGridSize_.y);
		int GridHash = GetGridHashIDbyPosition(Position_[i]);

		// choose current grid as the center, we just need to traverse 3*3 grids to get all neighbors
		for (int col = -1; col < 1; col++) {
			for (int row = -1; row < 1; row++) {
				if (col + ColumnIndex < 0 ||
					col + ColumnIndex > GridColNum_ ||
					row + RowIndex < 0 ||
					row + RowIndex > GridRowNum_) {
					continue; // for corner/edge grids, just ignore neighbor grids outsides the boundary
				}


				// search potential neighbors by hash function
				int NeighborHash = HashFunction(RowIndex + col, ColumnIndex + row, 0);
				for (int j : GridHashTable_[NeighborHash]) {
					if (i == j) {
						continue; // exclude current particle
					}
					NeighborInfo info{};
					info.dirVector = Position_[i] - Position_[j];
					info.distance = glm::length(info.dirVector);
					info.distance2 = info.distance * info.distance;
					info.index = j;
					if (info.distance < SupportRadius_) {
						Neighbors_[i].push_back(info);
					}
				}
			}
		}
	}
	
}

void ParticleSystem::BuildHashTable()
{
	GridHashTable_ = std::vector<std::vector<int>>(GridHashTableSize_, std::vector<int>(0));
	for (int i = 0; i < ParticleCount_; i++) {
		int GridHash = GetGridHashIDbyPosition(Position_[i]);
		if (GridHash != -1)
			GridHashTable_[GridHash].push_back(i);
	}
}

int ParticleSystem::GetGridHashIDbyPosition(glm::vec2 position)
{
	if (position.x < LowerLeftCorner_.x ||
		position.y < LowerLeftCorner_.y ||
		position.x > UpperRightCorner_.x ||
		position.y > UpperRightCorner_.y) {
		return -1;
	}

	glm::vec2 relativePostion = position - LowerLeftCorner_;
	int ColumnIndex = floor(relativePostion.x / SingleGridSize_.x);
	int RowIndex = floor(relativePostion.y / SingleGridSize_.y);
	return HashFunction(RowIndex, ColumnIndex, 0);
}

int ParticleSystem::HashFunction(int rowIndex, int columnIndex, int depthIndex)
{
	int p1 = 73856093;
	int p2 = 19349663;
	int p3 = 83492791;

	return ((p1 * rowIndex) ^ (p2 * columnIndex) ^ (p3 * depthIndex)) % GridHashTableSize_;
}

