#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <json/json.h>
#include "./graph_representations.hpp"
#include "sito.hpp"

AdjacencyMatrix makeFirstDegreeSubgraph(AdjacencyMatrix& matrix, int vertex)
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

AdjacencyMatrix makeSecondDegreeSubgraph(AdjacencyMatrix& matrix, int vertex)
{
    auto v = matrix.getData()[vertex];

    std::set<int> toRemove;
    toRemove.insert(vertex);

    std::set<int> secondDegreeNeighborhood;
    auto neighborhood = matrix.getNeighborhoodForVertex(vertex);
    for (auto it : neighborhood)
    {
        auto tempNeighborhood = matrix.getNeighborhoodForVertex(it);
        secondDegreeNeighborhood.insert(tempNeighborhood.begin(), tempNeighborhood.end());
    }
    secondDegreeNeighborhood.insert(neighborhood.begin(), neighborhood.end());

    AdjacencyMatrix subGraph(matrix);
    subGraph.setLabel("vertex " + std::to_string(vertex));
    for (int i = 0; i < v.neighbors.size(); i++)
    {
        if (secondDegreeNeighborhood.find(i) == secondDegreeNeighborhood.end())
            toRemove.insert(i);
    }
    subGraph.removeVertices(toRemove);
    return subGraph;
}

template<typename function>
std::vector<AdjacencyMatrix> makeNeighborhoodSubgraphs(AdjacencyMatrix& matrix, function fun)
{
    std::vector<AdjacencyMatrix> ret;
    auto data = matrix.getData();
    for (int it = 0; it < data.size(); it++)
    {
        ret.push_back(fun(matrix, it));
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

AdjacencyMatrix makeSpectrumMatrix(AdjacencyMatrix& graph, std::vector<AdjacencyMatrix> &subGraphs)
{
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

    spectrumMatrix = AdjacencyMatrix(spectrumGraphData);

    return spectrumMatrix;
}

void calculateSpectrumMatrix(std::string &inputPath, std::string& outputPath, int mode)
{
    AdjacencyMatrix graph = readAdjacencyMatrix(inputPath, false);

    std::vector<AdjacencyMatrix> subGraphs;
    if (mode == 1)
        subGraphs = makeNeighborhoodSubgraphs(graph, makeFirstDegreeSubgraph);
    else if (mode == 2)
        subGraphs = makeNeighborhoodSubgraphs(graph, makeSecondDegreeSubgraph);
    
    for (auto& it : subGraphs)
        sito(it);

    //std::cout << graph.toGraphViz();

    auto spectrumMatrix = makeSpectrumMatrix(graph, subGraphs);
    auto result = createResult(graph, spectrumMatrix);
    spectrumMatrix.print();

    std::fstream file(outputPath, std::fstream::out);
    file << result << std::endl;
}

#pragma warning(disable : 4996)
int main(int argc, char **argv) {

    std::string inputPath;
    std::string outputPath;
    int spectrumMode;

    if (argc == 1) {
        inputPath = ".\\test_10.txt";
        spectrumMode = 1;
        outputPath = ".\\export.json";
    }
    else if (argc == 2) {
        spectrumMode = std::stoi(argv[1]);
        inputPath = ".\\test_10.txt";
        outputPath = ".\\export.json";
    }
    else if (argc == 3) {
        spectrumMode = std::stoi(argv[1]);
        inputPath = argv[2];
        outputPath = ".\\export.json";
    }
    else if (argc == 4) {
        spectrumMode = std::stoi(argv[1]);
        inputPath = argv[2];
        outputPath = argv[3];
    }

    calculateSpectrumMatrix(inputPath, outputPath, spectrumMode);
    
    return 0;
}
