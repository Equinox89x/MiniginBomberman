#pragma once
#include "PathwayCreatorComponent.h"
#include "States.h"
#include <GameObject.h>
#include <queue>
#include <unordered_map>
#include <vector>

namespace dae
{
	class PathFinding
	{
	public:
		struct Node
		{
			Node(const PathWay* pathway, float gCost, float hCost) : Pathway{ pathway }, GCost{ gCost }, HCost{ hCost }
			{
				FCost = gCost + hCost;
				if (pathway->PathStats.PathType != MathLib::EPathType::Tile)
				{
					FCost += 9999;
				}
				Id = pathway->id;
			}
			int	  Id;
			float GCost{0.f}; // Cost from start node
			float HCost; // Heuristic cost to goal
			float FCost;

			const PathWay* Pathway{ nullptr };

			bool operator>(const Node& other) const { return FCost > other.FCost; }
		};

		static float Heuristic(int id1, int id2);

		static std::vector<int> AStar(Scene* scene, int startId, int goalId);
	
	};
} // namespace dae