#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "./graph_representations.hpp"

typedef std::vector<std::vector<int>> ta;
typedef std::vector<std::vector<int>> tu;

std::vector<long double> sito(AdjacencyMatrix & matrix);
