#include "graph_representations.hpp"

#include <time.h>

int AdjacencyMatrix::degree(int vertex)
{
    int deg = 0;
    for (auto u = 0; u < data.size(); u++)
        deg += data[vertex].neighbors[u];
    return deg;
}

AdjacencyMatrix::AdjacencyMatrix()
{
}

AdjacencyMatrix::AdjacencyMatrix(std::vector<Vertex> _data)
{
    data = _data;
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

std::set<int> AdjacencyMatrix::getAdjacencyForVertex(int vertex)
{
    std::set<int> ret;

    for (int it = 0; it < data[vertex].neighbors.size(); it++)
        if (data[vertex].neighbors[it])
            ret.insert(it);
    return ret;
}

int AdjacencyMatrix::getNumberOfVertices()
{
    return size();
}

int AdjacencyMatrix::getNumberOfEdges()
{
    int count = 0;
    for (unsigned int y = 0; y < data.size(); y++)
    {
        for (unsigned int x = 0; x < y; x++)
        {
            if (is_edge(y, x))
                count++;
        }
    }
    return count;
}

std::string AdjacencyMatrix::toGraphViz()
{
    std::string s = "graph g {\n";
    for (int i = 0; i < data.size(); i++)
        s += data[i].label + ";\n";

    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < i; j++)
            if(data[i].neighbors[j] == 1)
                s += data[i].label + "--" + data[j].label + ";\n";

    s += "}\n";
    return s;
}

std::string AdjacencyMatrix::toStringMatrix()
{
    std::string ret = "";

    for (int it = 0; it < data.size(); it++) {
        ret += data[it].label + ": ";

        for (int it2 = 0; it2 < data[it].neighbors.size(); it2++)
        {
            ret += std::to_string(data[it].neighbors[it2]);

            if (it2 < data[it].neighbors.size() - 1) {
                ret += "|";
            }
        }

        if (it < data.size() - 1) {
            ret += "\n";
        }
    }

    return ret;
}

std::vector<int> AdjacencyMatrix::toRowOrder()
{
    std::vector<int> result;

    for (auto it : data)
        for (auto it2 : it.neighbors)
            result.push_back(it2);

    return result;
}

std::vector<int> AdjacencyMatrix::toColumnOrder()
{
    std::vector<int> result;

    for (int it2 = 0; it2 < data.size(); it2++)
        for (auto it : data)
            result.push_back(it.neighbors[it2]);

    return result;
}

void AdjacencyMatrix::setSpectrum(std::vector<long double> _spectrum)
{
    spectrum = _spectrum;
}

std::vector<long double> AdjacencyMatrix::getSpectrum()
{
    return spectrum;
}


template <typename Type> std::string to_str(const Type & t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

std::string AdjacencyMatrix::getSpectrumAsString()
{
    std::string ret = "[";

    for (int it = 0; it < spectrum.size(); it++) {
        int multi = EPS * 10;
        ret += to_str(std::round(spectrum[it]*multi) / multi);
        //ret += std::to_string(it);

        if (it < spectrum.size() - 1)
            ret += "|";
    }

    ret += "]";
    return ret;
}

int AdjacencyMatrix::size() const
{
    return data.size();
}

void AdjacencyMatrix::setNullGraph()
{
    for (auto &it : data)
    {
        for (int it2 = 0; it2 < it.neighbors.size(); it2++) {
            it.neighbors[it2] = 0;
        }
    }
}

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