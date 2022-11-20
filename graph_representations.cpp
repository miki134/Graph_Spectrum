#include "graph_representations.hpp"

#include <time.h>

int AdjacencyMatrix::degree(int vertex)
{
    int deg = 0;
    for (auto u = 0; u < data.size(); u++)
        deg += data[vertex].neighbors[u];
    return deg;
}

void AdjacencyMatrix::eulerianCycleDfs(int v, std::list<int>& cycle)
{
    for (size_t u = 0; u < data.size(); u++) {
        if (data[v].neighbors[u]) {
            data[v].neighbors[u] = data[u].neighbors[v] = false;
            eulerianCycleDfs(u, cycle);
        }
    }
    cycle.push_front(v);
}

bool AdjacencyMatrix::contains(std::vector<int>& collection, int element)
{
    for (auto test : collection)
        if (test == element)
            return true;
    return false;
}

bool AdjacencyMatrix::hamiltonianCycleDfs(int v, std::vector<int>& visited)
{
    /*auto t1 = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    if (dt >= t_max) {
        did_timeout = true;
        return false;
    }*/

    if (contains(visited, v)) {
        if (visited.size() == data.size() && v == 0) {
            visited.push_back(v);
            return true;
        }
        return false;
    }

    visited.push_back(v);
    for (size_t u = 0; u < data.size(); u++) {
        if (data[v].neighbors[u] && hamiltonianCycleDfs(u, visited)) {
            return true;
        }
    }

    visited.pop_back();
    return false;
}

bool AdjacencyMatrix::isSafe(int v, std::vector<std::vector<bool>> graph, int path[], int pos)
{
    if (graph[path[pos - 1]][v] == 0)
        return false;
    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return false;
    return true;
}

bool AdjacencyMatrix::hamCycleUtil(std::vector<std::vector<bool>> graph, int path[], int pos)
{
    int V = graph.size();
    if (pos == V)
    {
        if (graph[path[pos - 1]][path[0]] == 1)
            return true;
        else
            return false;
    }

    for (int v = 1; v < V; v++)
    {
        if (isSafe(v, graph, path, pos))
        {
            path[pos] = v;
            if (hamCycleUtil(graph, path, pos + 1) == true)
                return true;
            path[pos] = -1;
        }
    }
    return false;
}

void AdjacencyMatrix::printSolution(int path[], int V)
{
    std::cout << "Solution Exists:";
    std::cout << " Following is one Hamiltonian Cycle \n" << std::endl;
    for (int i = 0; i < V; i++)
        std::cout << path[i] << "  ";
    std::cout << path[0] << std::endl;
}

AdjacencyMatrix::AdjacencyMatrix(std::vector< std::vector<bool> > _data)
{
    for (int it = 0; it < _data.size(); it++)
    {
        Vertex v;
        v.neighbors = _data[it];
        v.label = std::to_string(it);
        data.push_back(v);
    }
}

AdjacencyMatrix::AdjacencyMatrix(std::vector<bool> _data)
{
    Vertex v;
    v.neighbors = _data;
    v.label = std::to_string(data.size()+1);
    data.push_back(v);
}

AdjacencyMatrix::AdjacencyMatrix(std::string file_path)
{
    std::ifstream input(file_path);
    if (!input.good())
    {
        std::cerr << "[error] (bad file) " << file_path << "\n";
        exit(1);
    }

    int i = 0;
    for (std::string line; getline(input, line);)
    {
        std::istringstream line_input(line);
        Vertex v;

        std::vector<bool> line_data;
        for (bool byte; line_input >> byte;)
            line_data.push_back(byte);
        v.neighbors = line_data;
        v.label = std::to_string(i++);
        data.push_back(v);
    }
}

//AdjacencyMatrix::AdjacencyMatrix(std::vector< std::vector<int> > list)
//{
//    auto listData = list;
//    std::vector< std::vector<bool> > tempData(listData.size(), std::vector<bool>(listData.size()));
//
//    for (int i = 0; i < listData.size(); i++)
//    {
//        for (int y = 0; y < listData[i].size(); y++)
//        {
//            tempData[i][listData[i][y]] = true;
//            tempData[listData[i][y]][i] = true;
//        }
//    }
//    data = tempData;
//}

void AdjacencyMatrix::setData(std::vector<std::vector<bool>> _data)
{
    for (int it = 0; it < _data.size(); it++)
    {
        Vertex v;
        v.neighbors = _data[it];
        v.label = std::to_string(it);
        data.push_back(v);
    }
}

std::vector<Vertex> AdjacencyMatrix::getData()
{
    return data;
}

void AdjacencyMatrix::setLabel(std::string _label)
{
    label = _label;
}

std::string AdjacencyMatrix::getLabel()
{
    return label;
}

bool AdjacencyMatrix::is_edge(int u, int v)
{
    return data[u].neighbors[v] == true;
}

void AdjacencyMatrix::print()
{
    std::cout << "AdjacencyMatrix " << label << " [\n";
    for (auto row : data)
    {
        std::cout << row.label << ":  ";
        for (int col : row.neighbors)
            std::cout << col << " ";
        std::cout << "\n";
    }
    std::cout << "]\n";
}

//int AdjacencyMatrix::countBackedgesAdjMatrix()
//{
//    auto adj_matrix = data;
//
//    int backedges = 0;
//    for (int row = 0; row < adj_matrix.size(); row++)
//        for (int col = 0; col < adj_matrix[row].size(); col++)
//            if (adj_matrix[row][col])
//                if (GraphUtils::checkForBackedge(row, col))
//                    backedges += 1;
//
//    if (backedges != 0)
//        GraphUtils::cyclic_graphs += 1;
//    return backedges;
//}

bool AdjacencyMatrix::isEulerian()
{
    for (auto vertex = 0; vertex < data.size(); vertex++)
        if (degree(vertex) % 2 != 0)
            return false;
    return true;
}

void AdjacencyMatrix::makeEulerian()
{
    srand(time(NULL));

    for (int i = 0; i < data.size(); i++) {
        int deg = degree(i);

        if (deg % 2 != 0) {
            int x;

            do
            {
                x = rand() % data.size();
            } while (degree(x) <= 2 || data[i].neighbors[x]);

            if (data[i].neighbors[x]) {
                data[i].neighbors[x] = false;
                data[x].neighbors[i] = false;
            }
            else {
                data[i].neighbors[x] = true;
                data[x].neighbors[i] = true;
            }
        }
    }
}

void AdjacencyMatrix::makeCoherent()
{
    srand(time(NULL));

    for (int i = 0; i < data.size(); i++) {
        int deg = degree(i);

        if (deg == 0) {
            int x;

            do
            {
                x = rand() % data.size();
            } while (degree(x) <= 1 || data[i].neighbors[x]);

            if (data[i].neighbors[x]) {
                data[i].neighbors[x] = false;
                data[x].neighbors[i] = false;
            }
            else {
                data[i].neighbors[x] = true;
                data[x].neighbors[i] = true;
            }
        }
    }
}

bool AdjacencyMatrix::isCoherent()
{
    for (auto v = 0; v < data.size(); v++)
        if (degree(v) == 0)
            return false;
    return true;
}

std::vector<int> AdjacencyMatrix::hamiltonianCycle()
{
    //t0 = std::chrono::steady_clock::now();

    std::vector<int> visited;
    if (hamiltonianCycleDfs(0, visited)) {
        return visited;
    }
    else {
        std::cout << "no hamilton cycle";
        return std::vector<int>();
    }
}

std::list<int> AdjacencyMatrix::eulerianCycle()
{
    auto tempData = data;
    std::list<int> cycle;
    eulerianCycleDfs(0, cycle);
    data = tempData;
    return cycle;
}

bool AdjacencyMatrix::hamCycle(std::vector<std::vector<bool>> graph)
{
    int V = graph.size();
    int *path = new int[V];
    for (int i = 0; i < V; i++)
        path[i] = -1;
    path[0] = 0;
    if (hamCycleUtil(graph, path, 1) == false)
    {
        std::cout << "\nSolution does not exist" << std::endl;
        return false;
    }
    printSolution(path, V);
    return true;
}

void AdjacencyMatrix::removeVertex(const int toRemove)
{
    int a = 0;
    for (auto i = data.begin(); i != data.end(); i++) {
        if (a++ == toRemove)
        {
            data.erase(i);
            break;
        }
    }
    a = 0;
    for (auto y = 0; y < data.size(); y++) {
        for (auto i = data[y].neighbors.begin(); i != data[y].neighbors.end(); i++) {
            if (a++ == toRemove)
            {
                data[y].neighbors.erase(i);
                a = 0;
                break;
            }
        }
    }
}

void AdjacencyMatrix::removeVertices(std::set<int> toRemove)
{
    int a = 0;
    auto iter = data.begin();
    while (iter !=  data.end())
    {
        if (toRemove.find(a++) != toRemove.end())
            iter = data.erase(iter);
        else
            ++iter;
    }

    for (auto y = 0; y < data.size(); y++) {
        a = 0;
        auto iter = data[y].neighbors.begin();
        while (iter != data[y].neighbors.end())
        {
            if (toRemove.find(a++) != toRemove.end())
                iter = data[y].neighbors.erase(iter);
            else
                ++iter;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const AdjacencyMatrix& am)
{
    auto data = am.data;
    for (auto ver1 = 0; ver1 < am.data.size(); ++ver1)
    {
        for (auto ver2 = 0; ver2 < am.data.size(); ++ver2)
        {
            os << data[ver1].neighbors[ver2] << " ";
        }
        os << "\n";
    }
    return os;
}