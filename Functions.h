#define _USE_MATH_DEFINES
#pragma once


#include <vector>
#include <string>
#include <map>




struct elem
{
    long long mask;
    std::vector <int> path;
    int dist;

	elem(long long m, std::vector <int> v, int d)
	{
		mask = m;
		path = v;
		dist = d;
	}
};

std::vector <int> rndSet(int n, int m);

std::pair <std::vector <int>, int> PrepareOutput(std::pair <std::vector <int>, int> bestPath, std::vector <std::vector <elem>> Logistics);

std::vector <std::vector <int> > makeMatrix(int n, bool complete);

std::vector <std::vector <std::pair <std::vector <int>, int>>> dijkstra(std::vector <std::vector <int>> M, std::map <int, int> ref, int k);