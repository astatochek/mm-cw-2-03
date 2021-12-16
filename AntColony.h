#pragma once


#include <utility>
#include <vector>
#include <map>

#include "Functions.h"

struct ant
{
	long long tabooList;
	std::vector <int> trace; // индексы городов из списка обязательных
	int Location; // индекс горола из списка обязательных
	int startLocation;
	int lim; // колическов обязательных к посещению городов
	int dist;

	ant(int ik, int k)
	{
		startLocation = ik;
		Location = ik;
		trace.push_back(ik);
		tabooList = (long long)1 << ik;
		lim = k;
		dist = 0;
	}

	void Update(int ik, int k)
	{
		startLocation = ik;
		Location = ik;
		trace = std::vector <int> {ik};
		tabooList = (long long)1 << ik;
		lim = k;
		dist = 0;
	}
};
struct unit
{
	long long mask;
	std::vector <int> path;
	int dist;
	double PheromoneLevel;
	double Evaporation;
	double BonusPheromones;

	unit(double CurrentPheromoneLevel, double CurrentEvapoartion, elem elem)
	{
		dist = elem.dist;
		path = elem.path;
		mask = elem.mask;
		PheromoneLevel = CurrentPheromoneLevel;
		Evaporation = CurrentEvapoartion;
		BonusPheromones = 0;
	}

	void UpdatePheromones()
	{
		PheromoneLevel = PheromoneLevel * Evaporation + BonusPheromones;
		BonusPheromones = 0;
	}

};
struct Construct
{
    double CurrentPheromoneLevel;
    double CurrentEvaporation;
    int alpha;
    int beta;
    int gamma;
    int Fit;
    Construct(double phVal, double evVal, int alphaVal, int betaVal, int gammaVal)
    {
        CurrentPheromoneLevel = phVal;
        CurrentEvaporation = evVal;
        alpha = alphaVal;
        beta = betaVal;
        gamma = gammaVal;
        Fit = 0;
    }
};

class Colony
{
	std::map <int, int> ref;

	double CurrentPheromoneLevel; // 100
	double CurrentEvaporation;    // 0.9

	int alpha;                    // 2
	int beta;                     // 3
	int gamma;                    // 3
	int antMult;                  // 2

	int antNum;



	std::vector <int> MustGo;
	std::vector <ant*> Ants;
	std::vector <std::vector <unit>> M;
	std::pair <std::vector <int>, int> bestPath;
public:
	Colony(std::vector<std::vector<elem>> Logistics, std::map<int, int> refObj, std::vector <int> MustGoTowns,
		double CurrentPheromoneLevelVal, double CurrentEvaporationVal, int alphaVal, int betaVal, int gammaVal, int antMultVal);

	void Update();
	void ProcessAnt(ant* ant);
	void ProcessAlgorithm(int Iterations);
    std::pair <std::vector <int>, int> PrepareOutput();

    int Fitness();
};






std::pair <std::vector <int>, int> ants(std::vector <std::vector <int>> M,
	std::vector <std::vector <elem>> Logistics, long long mask, std::map <int, int> ref, std::vector <int> MustGoTowns, bool flag);