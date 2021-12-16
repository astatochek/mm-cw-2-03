#pragma once
#include <utility>
#include <vector>
#include <map>

#include "Functions.h"




std::pair <std::vector <int>, int> greedy(std::vector <std::vector <elem>> Logistics, std::map <int, int> ref, std::vector<int> towns)
{
	std::pair <std::vector <int>, int> bestPair = std::make_pair(std::vector <int> {}, INT_MAX);

	for (auto & pairVertInd : ref)
	{
		std::vector <int> path = { pairVertInd.first };
		int dist = 0;

		long long curMask = (long long) 1 << pairVertInd.second; // в маске 1 стоит на городах, которые мы уже посетили

		while (curMask != pow((long long) 2, ref.size()) - 1) // пока не посетим все города из списка необходимых
		{
			int min = INT_MAX, minIndex;
			for (auto & pairVI : ref)
			{
				int i = pairVI.second; // индекс в массиве m x m
				
				if ((curMask | (long long) 1 << i) != curMask && Logistics[ref[path.back()]][i].dist < min)
				{
					min = Logistics[ref[path.back()]][i].dist;
					minIndex = i;
				}
			}

			dist += Logistics[ref[path.back()]][minIndex].dist;
			curMask |= Logistics[ref[path.back()]][minIndex].mask;
			path.insert(path.end(), Logistics[ref[path.back()]][minIndex].path.begin(),
				Logistics[ref[path.back()]][minIndex].path.end());
		}

		dist += Logistics[ref[path.back()]][ref[path[0]]].dist;
		path.insert(path.end(), Logistics[ref[path.back()]][ref[path[0]]].path.begin(), 
			Logistics[ref[path.back()]][ref[path[0]]].path.end());

		if (dist < bestPair.second)
			bestPair = std::make_pair(path, dist);
	}

	return bestPair;
	
}