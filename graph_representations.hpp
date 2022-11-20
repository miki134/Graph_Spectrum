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

    int degree(int vertex);
    void eulerianCycleDfs(int v, std::list<int> &cycle);
    bool contains(std::vector<int> &collection, int element);
    bool hamiltonianCycleDfs(int v, std::vector<int> &visited);
    bool isSafe(int v, std::vector<std::vector<bool>> graph, int path[], int pos);
    bool hamCycleUtil(std::vector<std::vector<bool>> graph, int path[], int pos);
    void printSolution(int path[], int V);
    
public:
    AdjacencyMatrix(std::vector< std::vector<bool> > _data);
    AdjacencyMatrix(std::vector<bool> _data);
    AdjacencyMatrix(std::string file_path);
    //AdjacencyMatrix(std::vector< std::vector<int> > list);

    void setData(std::vector<std::vector<bool>> _data);
    std::vector<Vertex> getData();

    void setLabel(std::string _label);
    std::string getLabel();

    bool is_edge(int u, int v);

    void print();

    int countBackedgesAdjMatrix();

    bool isEulerian();
    void makeEulerian();

    void makeCoherent();
    bool isCoherent();

    //graph cycles
    std::vector<int> hamiltonianCycle();
    std::list<int> eulerianCycle();

    bool hamCycle(std::vector<std::vector<bool>> graph);
   
    void removeVertex(const int toRemove);
    void removeVertices(std::set<int> toRemove);

    friend std::ostream& operator<<(std::ostream& os, const AdjacencyMatrix& am);
};