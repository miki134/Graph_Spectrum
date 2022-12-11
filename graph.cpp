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

std::vector<AdjacencyMatrix> makeSubgraphForMatrix(AdjacencyMatrix& matrix)
{
    std::vector<AdjacencyMatrix> ret;
    auto data = matrix.getData();
    for (int it = 0; it < data.size(); it++)
    {
        ret.push_back(makeSubgraphForVertex(matrix, it));
    }
    return ret;
}

AdjacencyMatrix readAdjacencyMatrix(std::string& path, bool print = false)
{
    AdjacencyMatrix matrix(path);

    if(print) 
        matrix.print();

    return matrix;
}

bool compareDouble(long double db1, long double db2)
{
    int epsilon = 4;
    int multi = epsilon * 10;
    return (std::round(db1*multi) / multi) == (std::round(db2*multi) / multi);
}

bool compareSpectrum(std::vector<long double> sp1, std::vector<long double> sp2)
{
    if (sp1.size() != sp2.size())
        return false;

    bool ret = true;
    for (int i = 0; i < sp1.size(); i++)
    {
        if (!compareDouble(sp1[i], sp2[i]))
            ret = false;
    }

    return ret;
}

Json::Value createResult(AdjacencyMatrix &graph, AdjacencyMatrix &spectrumMatrix)
{
    Json::Value resultJson;
    resultJson["neighborhoodDegree"] = 1;
    resultJson["matrixUnrollingType"] = "row-majorOrder";
    resultJson["numberOfVertices"] = graph.getNumberOfVertices();
    resultJson["numberOfEdges"] = graph.getNumberOfEdges();

    Json::Value vec1(Json::arrayValue);
    auto graphOrder = graph.toRowOrder();
    for (auto it : graphOrder)
        vec1.append(it);
    resultJson["graph"] = vec1;

    Json::Value vec2(Json::arrayValue);
    auto order = spectrumMatrix.toRowOrder();
    for (auto it : order)
        vec2.append(it);

    resultJson["spectrumMatrix"] = vec2;

    return resultJson;
}

void calculateSpectrumMatrix(std::string &inputPath, std::string& outputPath)
{
    AdjacencyMatrix graph = readAdjacencyMatrix(inputPath, false);

    auto subGraphs = makeSubgraphForMatrix(graph);
    for (auto& it : subGraphs) {
        sito(it);
    }

    AdjacencyMatrix spectrumMatrix(graph);
    spectrumMatrix.setNullGraph();
    auto spectrumGraphData = spectrumMatrix.getData();

    for (int i = 0; i < subGraphs.size(); i++)
    {
        for (int y = 0; y < subGraphs.size(); y++)
        {
            if (compareSpectrum(subGraphs[i].getSpectrum(), subGraphs[y].getSpectrum()))
                spectrumGraphData[i].neighbors[y] = 1;
        }
    }

    std::cout << graph.toGraphViz();
    
    spectrumMatrix = AdjacencyMatrix(spectrumGraphData);
    //spectrumMatrix.print();

    auto result = createResult(graph, spectrumMatrix);
    //std::cout << result << std::endl;

    std::fstream file(outputPath, std::fstream::out);
    file << result << std::endl;
}

#pragma warning(disable : 4996)
int main(int argc, char **argv) {

    std::string inputPath;
    std::string outputPath;

    if (argc == 1) {
        inputPath = ".\\test_10.txt";
        outputPath = ".\\export.json";
    }
    else if (argc == 2) {
        inputPath = argv[1];
        outputPath = ".\\export.json";
    }
    else if (argc == 3) {
        inputPath = argv[1];
        outputPath = argv[2];
    }

    calculateSpectrumMatrix(inputPath, outputPath);
    
    return 0;
}
