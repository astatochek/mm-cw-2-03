#define _USE_MATH_DEFINES
#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <ctime>
#include <limits.h>
#include <random>
#include "Functions.h"

std::vector <int> rndSet(int n, int m)
{
    std::random_device rd;
    std::mt19937_64 mersenne(rd());
    std::vector <int> set(n);
    int i = 0;
    for (auto & elem : set) elem = i++;
    shuffle(begin(set), end(set), mersenne);
    set.erase(set.begin() + m, set.end());
    return set;
}

// Представление массива вершин из спсика необходимых в ответ задачи
std::pair <std::vector <int>, int> PrepareOutput(std::pair <std::vector <int>, int> bestPath, std::vector <std::vector <elem>> Logistics)
{
    std::vector <int> path;
    for (int i = 0; i < bestPath.first.size() - 1; i++)
        path.insert(path.end(), Logistics[bestPath.first[i]][bestPath.first[i+1]].path.begin(), Logistics[bestPath.first[i]][bestPath.first[i+1]].path.end());
    path.push_back(path.front());
    return std::make_pair(path, bestPath.second);
}

long long minDistance(std::vector <std::pair<std::vector <int>, int>> dist, std::vector <bool> sptSet, int n)
{

    long long min = LONG_MAX, min_index;

    for (long long i = 0; i < n; i++)
        if (!sptSet[i] && dist[i].second <= min)
        {
            min = dist[i].second;
            min_index = i;
        }

    return min_index;
}

std::vector <std::vector <int> > makeMatrix(int n, bool complete)
{
    std::mt19937 engine;
    engine.seed(std::time(nullptr));
    std::random_device device;
    engine.seed(device());

    std::vector <std::vector <int> > M(n, std::vector <int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            int elem = engine() % 1000;
            while (elem == 0)
            {
                elem = engine() % 1000;
            }
            if (complete)
            {
                M[i][j] = elem;
                M[j][i] = elem;
            }
            if (!complete)
            {
                int flag = engine();
                if (flag % 6 != 0)
                {
                    M[i][j] = 0;
                    M[j][i] = 0;
                }
                    
            	
                else
                {
                    M[i][j] = elem;
                    elem = engine() % 1000;
                    while (elem == 0)
                    {
                        elem = engine() % 1000;
                    }
                    M[j][i] = elem;
                	
                }
            }

            
        }

        if (!complete)
        {
            for (int i = 0; i < n; i++)
            {
                int cnt = 0;
                for (int j = 0; j < n; j++)
                    if (M[i][j] != 0)
                        cnt++;
                if (cnt < 2)
                {
                    int j = engine() % n;
                    while (j == i)
                        j = engine() % n;
                    int elem = engine() % 999 + 1;
                    M[i][j] = elem;
                    elem = engine() % 999 + 1;
                    M[j][i] = elem;
                    int l = engine() % n;
                    while (l == i || l == j)
                        l = engine() % n;
                    elem = engine() % 999 + 1;
                    M[i][l] = elem;
                    elem = engine() % 999 + 1;
                    M[l][i] = elem;
                }
            }
        }
    }


    return M;
}

std::vector <std::vector <std::pair <std::vector <int>, int>>> dijkstra(std::vector <std::vector <int>> M, std::map <int, int> ref, int k)
{
    std::vector <std::vector <std::pair <std::vector <int>, int>>>Res(ref.size(), std::vector <std::pair <std::vector <int>, int>>(ref.size(), std::make_pair(std::vector <int>{}, 0)));

    for (auto & elem : ref)
    {   
        int vert = elem.first;
    	
        std::vector <std::pair<std::vector <int>, int>> dist(M.size());

        std::vector <bool> sptSet(M.size(), false);

        for (int i = 0; i < M.size(); i++)
            dist[i].second = INT_MAX;

        dist[vert].second = 0;


        for (int count = 0; count < M.size() - 1; count++) {
        	
            long long u = minDistance(dist, sptSet, M.size());

            sptSet[u] = true;

            for (long long v = 0; v < M.size(); v++)
                if (!sptSet[v] && M[u][v] && dist[u].second != INT_MAX && dist[u].second + M[u][v] < dist[v].second)
                {
                    dist[v].second = dist[u].second + M[u][v];
                    dist[v].first = dist[u].first;
                    dist[v].first.push_back(v);
                }
                    
        }
    	

    	for (auto& val : ref)
    	{
            int v = val.first;

            Res[ref[vert]][ref[v]] = dist[v];
    	}

    }

	return Res;
}

