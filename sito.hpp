#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "./graph_representations.hpp"

#ifndef SITO_H
 #define SITO_H
 typedef int ta[16][16];
 typedef int tu[16];
#endif

int connected(int N, ta A);
int integral(int N, ta A);
int sito(std::vector<std::string> vec, AdjacencyMatrix & matrix);
void printt(ta A);
