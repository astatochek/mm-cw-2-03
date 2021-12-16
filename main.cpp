#define _USE_MATH_DEFINES
#pragma once
#include <iostream>
#include <utility>
#include <map>
#include <vector>
#include <random>
#include <bitset>
#include <fstream>


#include "Functions.h"
#include "Greedy.h"
#include "AntColony.h"
#include "BranchAndBound.h"


int main() {
    setlocale(LC_ALL, "Russian");
    std::random_device rd;
    std::mt19937_64 mersenne(rd());

    bool metric;
    int n , m;

    std::cin >> metric >> n >> m;

    std::ofstream out;
    out.open("../file.txt");

    out << n << " " << m << "\n";

    std::vector<int> MustGoTowns = rndSet(n, m); // города, обязательные для посещения

    long long mask = 0;
    for (auto & townIndex : MustGoTowns)
        mask |= (long long)1 << townIndex;


    for (auto &vert: MustGoTowns)
        out << vert << " ";
    out << "\n";

    std::cout << "Must be in path: ";
    for (auto &vert: MustGoTowns)
        std::cout << vert << " ";
    std::cout << "\n";

    if (!metric)
    {
        for (auto &vert: MustGoTowns)
            std::cout << vert << " ";
        std::cout << "\n";
    }


    std::map<int, int> ref;

    for (int i = 0; i < m; i++)
        ref[MustGoTowns[i]] = i;

    std::vector <std::pair <double, double>> coordinates(n);

    if (metric)
    {
        //std::uniform_real_distribution<double> rnd(0.0, 1.0);
        for (int i = 0; i < n; i++)
            coordinates[i] = std::make_pair(mersenne() % 1000, mersenne() % 1000);
        for (auto &pair : coordinates)
            out << pair.first << " " << pair.second << "\n";
    }


    std::vector<std::vector<int>> Matrix = makeMatrix(n, false);

    std::vector <std::pair<int, int>> edgeList;

    if (metric)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (Matrix[i][j] != 0)
                {
                    double val = std::pow(std::pow(coordinates[i].first - coordinates[j].first, 2) + std::pow(coordinates[i].second - coordinates[j].second, 2), 0.5);
                    Matrix[i][j] = int(val);
                    //std::cout << val << " " << int(val) << "\n";
                    edgeList.push_back(std::make_pair(i, j));
                }
            }
        }
    }

    if (metric)
    {
        out << edgeList.size() << "\n";
        for (auto &pair : edgeList)
            out << pair.first << " " << pair.second << "\n";
    }





    std::vector<std::vector<std::pair<std::vector<int>, int>>> D = dijkstra(Matrix, ref, Matrix.size());


    std::vector<std::vector<elem>> Logistics(D.size(), std::vector<elem>(D.size(), elem(0, std::vector<int>{}, 0)));

    for (int i = 0; i < D.size(); i++)
    {
        for (int j = 0; j < D.size(); j++) {
            long long curMask = 0;
            for (auto &val: D[i][j].first) {
                if (ref.find(val) != ref.end())
                    curMask |= (long long) 1 << ref[val];
            }
            // маска делается рамера m (!!!)
            Logistics[i][j] = elem(curMask, D[i][j].first, D[i][j].second);
        }
    }

    std::cout << "\nLogistics done\n\n";


    // Grredy Result Output
    std::pair <std::vector <int>, int> greedyRes = greedy(Logistics, ref, MustGoTowns);

    out << greedyRes.first[0];
    std::cout << "(" << greedyRes.first[0];
    for (int i = 1; i < greedyRes.first.size(); i++)
    {
        out << " " << greedyRes.first[i];
        std::cout << " " << greedyRes.first[i];
    }
    out << "\nGreedy Result: " << greedyRes.second << "\n";
    std::cout << ")\nGreedy Result: " << greedyRes.second << "\n";



    // Ant Colony Result Output
    std::pair <std::vector <int>, int> antRes = ants(Matrix, Logistics, mask, ref, MustGoTowns, false);

    out << antRes.first[0];
    std::cout << "(" << antRes.first[0];
    for (int i = 1; i < antRes.first.size(); i++)
    {
        out << " " << antRes.first[i];
        std::cout << " " << antRes.first[i];
    }

    out << "\nAnt Colony Result: " << antRes.second << "\n";
    std::cout << ")\nAnt Colony Result: " << antRes.second << "\n";


    // Branch and Bound Output

    std::pair <std::vector <int>, int> branchRes = branchAndBound(Logistics, ref, MustGoTowns);

    out << branchRes.first[0];
    std::cout << "(" << branchRes.first[0];
    for (int i = 1; i < branchRes.first.size(); i++)
    {
        out << " " << branchRes.first[i];
        std::cout << " " << branchRes.first[i];
    }


    out << "\nBranch and Bound: " << branchRes.second;
    std::cout << ")\nBranch and Bound: " << branchRes.second << "\n";

    // End of Output

    out.close();



    return 0;
}


