#pragma once


#include <utility>
#include <vector>
#include <map>

#include "Functions.h"

std::pair <std::vector <int>, int> branchAndBound(std::vector <std::vector <elem>> Logistics,
                                                  std::map <int, int> ref, std::vector<int> towns);