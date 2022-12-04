#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <json/json.h>
#include "./graph_representations.hpp"
#include "sito.hpp"

AdjacencyMatrix makeSubgraphForVertex(AdjacencyMatrix& matrix, int vertex)
{
    auto v = matrix.getData()[vertex];

    std::set<int> toRemove;
    toRemove.insert(vertex);

    AdjacencyMatrix subGraph(matrix);
    subGraph.setLabel("vertex " + std::to_string(vertex));
    for (int i = 0; i < v.neighbors.size(); i++)
    {
        if(!v.neighbors[i])
            toRemove.insert(i);
    }

    subGraph.removeVertices(toRemove);

    return subGraph;
}

void makeSubgraphForMatrix(AdjacencyMatrix& matrix)
{
    auto data = matrix.getData();
    for (int it = 0; it < data.size(); it++)
    {
        makeSubgraphForVertex(matrix, it).print();
    }
}

AdjacencyMatrix readAdjacencyMatrix(std::string& path, bool print = false)
{
    AdjacencyMatrix matrix(path);

    if(print) 
        matrix.print();

    return matrix;
}

#pragma warning(disable : 4996)
int main(int argc, char **argv) {

    std::string inputPath = ".\\data\\";
    std::string outputPath = ".\\export\\exp.txt";

    Json::Value body;
    Json::Reader reader(Json::Features::all());

    inputPath += "test_10.txt";

    std::vector<std::string> temp;
    temp.push_back("0");
    temp.push_back(inputPath);
    temp.push_back(outputPath);

    AdjacencyMatrix graph =  readAdjacencyMatrix(inputPath, true);
    
    sito(temp, graph);
    makeSubgraphForMatrix(graph);
    return 0;
}
