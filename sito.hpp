#pragma once

#include <string>
#include <vector>
#include <iostream>

#ifndef SITO_H
 #define SITO_H
 typedef int ta[16][16];
 typedef int tu[16];
#endif

int connected(int N, ta A);
int integral(int N, ta A);
int sito(std::vector<std::string> vec);
void printt(ta A);