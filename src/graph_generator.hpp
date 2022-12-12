#pragma once

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "./graph_representations.hpp"

class GraphGenerator
{
private:
    unsigned int vertices;
    double density = 1.0;

public:

    GraphGenerator(unsigned int _vertices, double _density) :
        vertices(_vertices),
        density(_density) {}

    void setVertices(unsigned int _v) {
        vertices = _v;
    }

    void setDensity(double _d) {
        density = _d;
    }

    bool generateMS(const std::string& file_path, bool eulerGraph = false)
    {
        std::vector<std::vector<bool>> tab(vertices, std::vector<bool>(vertices, false));

        unsigned int maxEdgesCount = vertices * (vertices - 1) / 2;
        unsigned int edgesCount = density * maxEdgesCount;

        srand(time(NULL));

        unsigned int edgesIter = 0;

        while (edgesIter < edgesCount)
        {
            unsigned int ver1 = rand() % vertices;
            unsigned int ver2 = rand() % (vertices - ver1) + ver1;
            if (!tab[ver1][ver2] && ver1 != ver2)
            {
                tab[ver1][ver2] = 1;
                tab[ver2][ver1] = 1;
                edgesIter++;
            }
        }

        AdjacencyMatrix am(tab);

        std::ofstream file(file_path);
        file << am;

        file.close();

        return true;
    }
};
