#pragma once
#include <utility>
#include <vector>
#include <map>
#include <iostream>

#include "Functions.h"
#include "Greedy.h"

void Rec(std::vector <std::vector <elem>> Logistics, long long curMask, std::map <int, int> ref,
	std::vector <int> path, int dist, std::pair <std::vector <int>, int> & bestPair)
{
	if (curMask == pow((long long) 2, ref.size()) - 1)
	{
		dist += Logistics[path.back()][path.front()].dist;
		path.push_back(path.front()); // insert(path.end(), Logistics[ref[path.back()]][ref[path[0]]].path.begin(), Logistics[ref[path.back()]][ref[path[0]]].path.end());

		if (dist <= bestPair.second)
        {
            //std::cout << dist << "\n";
            bestPair = std::make_pair(path, dist);
        }

			
	}

	else if (dist + Logistics[path.back()][path.front()].dist <= bestPair.second)
	{
		for (int to = 0; to < ref.size(); to++)
		{
			if ((curMask | (long long) 1<<to) != curMask && (dist + Logistics[path.back()][to].dist <= bestPair.second))
			{
				long long newMask = curMask | Logistics[path.back()][to].mask;
				int newDist = dist + Logistics[path.back()][to].dist;
				std::vector <int> newPath = path;
				newPath.push_back(to);
				Rec(Logistics, newMask, ref, newPath, newDist, bestPair);
			}
		}
	}
}



std::pair <std::vector <int>, int> branchAndBound(std::vector <std::vector <elem>> Logistics,
                                                  std::map <int, int> ref, std::vector<int> towns)
{
	
	std::pair <std::vector <int>, int> bestPair = greedy(Logistics, ref, towns);

	int vert = towns.front();

	std::vector <int> path = { ref[vert] };
	int dist = 0;
	long long curMask = 0;

	curMask |= (long long)1 << ref[vert]; // в маске 1 стоит на городах, которые мы уже посетили из списка обязательных для посещения

	Rec(Logistics, curMask, ref, path, dist, bestPair);

	return PrepareOutput(bestPair, Logistics);

}