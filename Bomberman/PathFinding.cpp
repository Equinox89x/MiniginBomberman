#include "PathFinding.h"

float dae::PathFinding::Heuristic(int id1, int id2)
{
	int x1 = id1 % GridSize;
	int y1 = id1 / GridSize;
	int x2 = id2 % GridSize;
	int y2 = id2 / GridSize;
	return static_cast<float>(std::abs(x1 - x2) + std::abs(y1 - y2));
}

std::vector<int> dae::PathFinding::AStar(Scene* scene, int startId, int goalId)
{
	auto* comp{ scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto& pathways{ comp->GetPathways() };

	std::priority_queue<Node, std::vector<Node>, std::greater<>> openSet;
	std::map<int, int>											 cameFrom;
	std::map<int, float>										 gCostMap;
	std::map<int, float>										 hCostMap;

	openSet.push(Node(&pathways.find(startId)->second, 0.f, Heuristic(startId, goalId)));
	gCostMap[startId] = 0.0f;
	hCostMap[startId] = Heuristic(startId, goalId);

	while (!openSet.empty())
	{
		Node current = openSet.top();
		openSet.pop();

		if (current.Id == goalId)
		{
			// Reconstruct path
			std::vector<int> path;
			for (int at = goalId; at != startId; at = cameFrom[at])
			{
				path.push_back(at);
			}
			path.push_back(startId);
			std::reverse(path.begin(), path.end());
			return path;
		}

		const PathWay& currentNode{ pathways.find(current.Id)->second };
		for (const int neighborId : currentNode.GetNeighbours())
		{
			if (pathways.find(neighborId) == pathways.end())
				continue; // Skip if neighbor is not in the map

			float tentativeGCost = gCostMap[current.Id] + 1.0f; // Assuming cost of 1 for each step
			if (gCostMap.find(neighborId) == gCostMap.end() || tentativeGCost < gCostMap[neighborId])
			{
				cameFrom[neighborId] = current.Id;
				gCostMap[neighborId] = tentativeGCost;
				hCostMap[neighborId] = Heuristic(neighborId, goalId);
				openSet.push(Node(&pathways.find(neighborId)->second, tentativeGCost, hCostMap[neighborId]));
			}
		}
	}

	return {}; // Return empty path if goal is not reachable
}
