#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include <map>
#include <random>

#include "Functions.h"
#include "AntColony.h"

int cnt(long long num)
{
	int res = 0;
	while(num)
	{
		res += num % 2;
		num /= 2;
	}
	return res;
}

Colony::Colony(std::vector<std::vector<elem>> Logistics, std::map<int, int> refObj, std::vector <int> MustGoTowns,
	double CurrentPheromoneLevelVal, double CurrentEvaporationVal, int alphaVal, int betaVal, int gammaVal, int antMultVal)
{
	ref = refObj;
	MustGo = MustGoTowns;
	CurrentPheromoneLevel = CurrentPheromoneLevelVal;
	CurrentEvaporation = CurrentEvaporationVal;
	alpha = alphaVal;
	beta = betaVal;
	gamma = gammaVal;
	antMult = antMultVal;
	bestPath = std::make_pair(std::vector <int> {}, INT_MAX);

	antNum = ref.size() * antMult;

	for (int i = 0; i < antNum; i++)
	{
		auto * tmpAnt = new ant(i % ref.size(), ref.size());
		Ants.push_back(tmpAnt);
	}

		

	for (int i = 0; i < Logistics.size(); i++)
	{
		std::vector <unit> tmp;
		
		for (int j = 0; j < Logistics[i].size(); j++)
		{
			tmp.push_back(unit(CurrentPheromoneLevel, CurrentEvaporation, Logistics[i][j]));
		}
		
		M.push_back(tmp);
	}

}

void Colony::Update()
{
	for (int i = 0; i < Ants.size(); i++)
		Ants[i]->Update(i % ref.size(), ref.size());
	
	for (int i = 0; i < ref.size(); i++)
		for (int j = 0; j < ref.size(); j++)
			M[i][j].UpdatePheromones();
}

void Colony::ProcessAnt(ant* ant)
{
	while (ant->tabooList < pow((long long)2, ant->lim) - 1)
	{
		std::random_device rd;
		std::mt19937_64 engine(rd());
		std::uniform_real_distribution<double> val{ 0.0, 1.0 };


		int from = ant->Location;
		int to;

		double sum = 0;
		
		for (int l = 0; l < ant->lim; l++)
		{
			if ((ant->tabooList | ((long long) 1 << l)) != ant->tabooList)
			{
				to = l;
				sum += pow(cnt(M[from][to].mask), gamma) * pow(M[from][to].PheromoneLevel, alpha) / pow(M[from][to].dist, beta);
			}	
		}

		std::vector <double> distribution;
		double addition = 0;

		for (int l = 0; l < ant->lim; l++)
		{
			if ((ant->tabooList | ((long long)1 << l)) != ant->tabooList)
			{
				to = l;
				addition += pow(cnt(M[from][to].mask), gamma) * pow(M[from][to].PheromoneLevel, alpha) / pow(M[from][to].dist, beta) / sum;
				distribution.push_back(addition);
			}
		}

		double chance = val(engine);

		int ind = 0;
		
		for (int l = 0; l < ant->lim; l++)
		{
			if ((ant->tabooList | ((long long)1 << l)) != ant->tabooList)
			{
				to = l;
				if (chance <= distribution[ind])
				{
					ant->tabooList |= M[from][to].mask;
					ant->dist += M[from][to].dist;
					ant->trace.push_back(to); //insert(ant->trace.end(), M[from][to].path.begin(), M[from][to].path.end());
					ant->Location = to;
					M[from][to].BonusPheromones += (double)cnt(M[from][to].mask) / M[from][to].dist;
					break;
				}
				ind++;
			}
		}
	}
	ant->dist += M[ant->Location][ant->startLocation].dist;
	ant->trace.push_back(ant->startLocation); //insert(ant->trace.end(), M[ant->Location][ant->startLocation].path.begin(), M[ant->Location][ant->startLocation].path.end())

	if (bestPath.second > ant->dist)
	{
		bestPath.first = ant->trace;
		bestPath.second = ant->dist;
	}
	
	ant->Update(ant->startLocation, ref.size());
}

void Colony::ProcessAlgorithm(int Iterations)
{
	while(Iterations--)
	{
		for (auto& ant : Ants)
			ProcessAnt(ant);
		Update();
	}
}

std::pair <std::vector <int>, int> Colony::PrepareOutput()
{
    std::vector <int> path;
    for (int i = 0; i < bestPath.first.size() - 1; i++)
        path.insert(path.end(), M[bestPath.first[i]][bestPath.first[i+1]].path.begin(), M[bestPath.first[i]][bestPath.first[i+1]].path.end());
    path.push_back(path.front());
    return std::make_pair(path, bestPath.second);
}


// Генетичекий подбор констант
int Colony::Fitness()
{
    int Iter = 50;
    ProcessAlgorithm(Iter);
    return PrepareOutput().second;
}

Construct Hybridization(Construct first, Construct second, std::vector<std::vector<elem>> Logistics, std::map<int, int> refObj, std::vector <int> MustGoTowns)
{
    Construct Res((first.CurrentPheromoneLevel + second.CurrentPheromoneLevel) / 2,
                  (first.CurrentEvaporation + second.CurrentEvaporation) / 2,
                  first.alpha, second.beta, second.gamma);
    Colony C(Logistics, refObj, MustGoTowns, Res.CurrentPheromoneLevel, Res.CurrentEvaporation, Res.alpha, Res.beta, Res.gamma, 2);
    Res.Fit = C.Fitness();
    return Res;
}

std::vector <Construct> getNewPopulation(std::vector <Construct> Population, std::vector<std::vector<elem>> Logistics, std::map<int, int> refObj, std::vector <int> MustGoTowns)
{
    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_real_distribution<double> val{ 0.0, 0.9999999 };

    int n = Population.size();
    double sum = 0;
    for (auto &constr : Population)
        sum += 1 / (double)constr.Fit;
    double add = 0;
    std::vector <double> Partition;
    for (int i = 0; i < n; i++)
    {
        add += (1 / (double)Population[i].Fit) / sum;
        Partition.push_back(add);
    }

    std::vector <Construct> Parents;

    for (int i = 0; i < n; i++)
    {
        double chance = val(engine);
        for (int j = 0; j < n; j++)
        {
            if (chance <= Partition[j])
            {
                Parents.push_back(Population[j]);
                break;
            }
        }
    }

    std::vector <Construct> Res;
    for (int id = 0; id < n / 2; id++)
    {
        int i = engine() % n, j = engine() % n;
        while (i == j)
            j = engine() % n;
        Res.push_back(Hybridization(Parents[i], Parents[j], Logistics, refObj, MustGoTowns));
        Res.push_back(Hybridization(Parents[j], Parents[i], Logistics, refObj, MustGoTowns));
    }

    return Res;
}

Construct Genetic(std::vector<std::vector<elem>> Logistics, std::map<int, int> refObj, std::vector <int> MustGoTowns)
{
    double PheromoneUpperBound = 200;
    double PheromoneLowerBound = 50;
    double EvaporationUpperBound = 0.99;
    double EvaporationLowerBound = 0.7;
    int alphaUpperBound = 4;
    int alphaLowerBound = 1;
    int betaUpperBound = 4;
    int betaLowerBound = 1;
    int gammaUpperBound = 4;
    int gammaLowerBound = 1;

    int antMult = 2;

    std::vector <Construct> Population;
    for (int alpha = alphaLowerBound; alpha <= alphaUpperBound; alpha++)
    {
        for (int beta = betaLowerBound; beta <= betaUpperBound; beta++)
        {
            for (int gamma = gammaLowerBound; gamma <= gammaUpperBound; gamma++)
            {
                Construct Tmp(PheromoneUpperBound, EvaporationUpperBound, alpha, beta, gamma);
                Colony C1(Logistics, refObj, MustGoTowns, Tmp.CurrentPheromoneLevel, Tmp.CurrentEvaporation, Tmp.alpha, Tmp.beta, Tmp.gamma, antMult);
                Tmp.Fit = C1.Fitness();
                Population.push_back(Tmp);
                Tmp.CurrentPheromoneLevel = PheromoneLowerBound;
                Colony C2(Logistics, refObj, MustGoTowns, Tmp.CurrentPheromoneLevel, Tmp.CurrentEvaporation, Tmp.alpha, Tmp.beta, Tmp.gamma, antMult);
                Tmp.Fit = C2.Fitness();
                Population.push_back(Tmp);
                Tmp.CurrentEvaporation = EvaporationLowerBound;
                Colony C3(Logistics, refObj, MustGoTowns, Tmp.CurrentPheromoneLevel, Tmp.CurrentEvaporation, Tmp.alpha, Tmp.beta, Tmp.gamma, antMult);
                Tmp.Fit = C3.Fitness();
                Population.push_back(Tmp);
                Tmp.CurrentPheromoneLevel = PheromoneUpperBound;
                Colony C4(Logistics, refObj, MustGoTowns, Tmp.CurrentPheromoneLevel, Tmp.CurrentEvaporation, Tmp.alpha, Tmp.beta, Tmp.gamma, antMult);
                Tmp.Fit = C4.Fitness();
                Population.push_back(Tmp);
            }
        }
    }

    int Iterations = 20;

    while(Iterations--)
        Population = getNewPopulation(Population, Logistics, refObj, MustGoTowns);

    int minFit = INT_MAX;
    Construct Res(0, 0, 0, 0, 0);
    for (auto &construct : Population)
    {
        if (construct.Fit < minFit)
        {
            Res = construct;
            minFit = construct.Fit;
        }
    }

    return Res;
}
// ...

std::pair <std::vector <int>, int> ants(std::vector <std::vector <int>> M,
	std::vector <std::vector <elem>> Logistics, long long mask, std::map <int, int> ref, std::vector <int> MustGoTowns, bool flag)
{

	double CurrentPheromoneLevel = 500;
	double CurrentEvaporation = 0.9;

	int alpha = 2;
	int beta = 3;
	int gamma = 3;

    int antMult = 2;

    if (flag) // хотим запускать генетический
    {
        Construct Constants = Genetic(Logistics, ref, MustGoTowns);
        std::cout << "\nPheromones: " << Constants.CurrentPheromoneLevel <<
                  "\nEvaporation: " << Constants.CurrentEvaporation << "\nalpha: " <<
                  Constants.alpha << "\nbeta: " << Constants.beta << "\ngamma: " << Constants.gamma << "\n";
        CurrentPheromoneLevel = Constants.CurrentPheromoneLevel;
        CurrentEvaporation = Constants.CurrentEvaporation;
        alpha = Constants.alpha;
        beta = Constants.beta;
        gamma = Constants.gamma;
    }

	int Iterations = 1000;
	
	Colony C(Logistics, ref, MustGoTowns, CurrentPheromoneLevel, CurrentEvaporation, alpha, beta, gamma, antMult);
	
	C.ProcessAlgorithm(Iterations);
    return C.PrepareOutput();
}