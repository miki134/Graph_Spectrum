#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stdio.h>

class AdjacencyList;

const int EPS = 4;

struct Vertex
{
    std::vector<bool> neighbors;
    std::string label;
};

class AdjacencyMatrix
{
private:
    std::vector<Vertex> data;
    std::string label;
    std::vector<long double> spectrum;

    int degree(int vertex);

public:
    AdjacencyMatrix();
    AdjacencyMatrix(std::vector<Vertex> _data);
    AdjacencyMatrix(std::vector<std::vector<bool>> _data);
    AdjacencyMatrix(std::vector<bool> _data);
    AdjacencyMatrix(std::string file_path);

    std::set<int> getAdjacencyForVertex(int vertex);

    int getNumberOfVertices();
    int getNumberOfEdges();

    std::string toGraphViz();
    std::string toStringMatrix();

    std::vector<int> toRowOrder();
    std::vector<int> toColumnOrder();

    void setSpectrum(std::vector<long double> _spectrum);
    std::vector<long double> getSpectrum();
    std::string getSpectrumAsString();

    int size() const;

    void setNullGraph();

    void setData(std::vector<std::vector<bool>> _data);
    std::vector<Vertex> getData();

    void setLabel(std::string _label);
    std::string getLabel();

    bool is_edge(int u, int v);

    void print();
   
    void removeVertex(const int toRemove);
    void removeVertices(std::set<int> toRemove);

    friend std::ostream& operator<<(std::ostream& os, const AdjacencyMatrix& am);
};