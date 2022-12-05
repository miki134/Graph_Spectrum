#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "./graph_representations.hpp"

typedef std::vector<std::vector<int>> ta;
typedef std::vector<std::vector<int>> tu;

int connected(int N, ta A);
int integral(int N, ta A);
std::vector<long double> sito(std::vector<std::string> vec, AdjacencyMatrix & matrix);
void printt(ta A);
