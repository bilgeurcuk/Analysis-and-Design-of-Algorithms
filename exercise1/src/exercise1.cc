#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <cstdio>
#include <queue>
#include <cfloat>
using namespace std;

struct Edge {
    int src, dest;
    float weight;
};

struct Node {
    int vertex ;
    float weight;
};

class Graph
{
public:
    vector<vector<Edge>> adjList;

    Graph(vector<Edge> const &edges, int n)
    {
        adjList.resize(n);
        // add edges to the directed graph
        for (Edge const &edge: edges)
        {
            // insert at the end
            adjList[edge.src].push_back(edge);
        }
    }
};

void split_by_commas(string &line, vector<std::string> &items){
    items.clear();
    // while there are commas in the line, split and push the first part
    // into items vector
    size_t comma_pos = line.find(',');
    size_t r_pos = line.find("\r");
    while (comma_pos != std::string::npos) {
        items.push_back(line.substr(0, comma_pos));
        line = line.substr(comma_pos+1, line.size());
        comma_pos = line.find(',');
    }
    // push the remaining last item
    items.push_back(line);
}

void printPath(vector<int> const &prev, int i, int source)
{
    if (i < 0) {
        return;
    }
    printPath(prev, prev[i], source);
    if (i != source) {
        cout << ", ";
    }
    cout << i;
}
vector<Edge> load_data( const std::string &path) {

    ifstream fin(path);
    if (!fin.good()) {
        cerr << "Error opening data file: " << path << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    vector<string> items;
    int line_number = 0;
    vector<Edge> player_data;

    while (getline(fin, line)) {
        split_by_commas(line, items);
        if ( line.empty()) {break;}

        for( int i =0; i<11; i++){
            Edge pd;
            pd.src = line_number;
            pd.dest = i;
            float s = stof(items[i]);
            pd.weight = s;
            player_data.push_back(pd);
        }
        line_number++;
    }
    fin.close();
    return player_data;
}

// Comparison object to be used to order the heap
struct comp
{
    bool operator()(const Node &lhs, const Node &rhs) const {
        return lhs.weight > rhs.weight;
    }
};

// Run Dijkstra’s algorithm on the given graph
void findShortestPaths(Graph const &graph, int source, int n, vector<Edge> const &diagonal)
{
    // create a min-heap and push source node having distance 0
    priority_queue<Node, vector<Node>, comp> min_heap;
    min_heap.push({source, 0});

    // set initial distance from the source to `v` as infinity
    vector<float> dist(n, FLT_MAX);

    // distance from the source to itself is zero
    dist[source] = 0;
    // boolean array to track vertices for which minimum
    // cost is already found
    vector<bool> done(n, false);
    done[source] = true;

    // stores predecessor of a vertex (to a print path)
    vector<int> prev(n, -1);
    Node node ;
    // run till min-heap is empty
    while (!min_heap.empty())
    {
        // Remove and return the best vertex
        node = min_heap.top();
        min_heap.pop();
        // get the vertex number
        int u = node.vertex;

        // do for each neighbor `v` of `u`
        for (auto i: graph.adjList[u])
        {
            int v = i.dest;
            float weight = i.weight;
            // Relaxation step
            if (!done[v] && (dist[u] + weight) < dist[v])
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                min_heap.push({v, float(dist[v])});

            }
        }
        // mark vertex `u` as done so it will not get picked up again
        done[u] = true;
    }
    float max_prob = 0;
    int max_prob_index = 0;
    for (int i = 0; i < n; i++)
    {
        if (i != dist[i] != FLT_MAX)
        {
            if ( dist[i]*diagonal[i].weight> max_prob){
                max_prob= dist[i]*diagonal[i].weight;
                max_prob_index = i;
            }
        }
    }
    if ( diagonal[source].weight == 0){
        printPath(prev, max_prob_index, source);
    }
    else {
        cout << source << endl;
    }
}

int main(int argc, char** argv) {
    string path = argv[1];

    int source = atoi(argv[2]);
    //string path ="/Users/bilge/Documents/ceng218/homeworks/hw3/exercise1/data/probabilities.csv";
    vector<Edge> edges = load_data(path);
    vector<Edge> diagonal ;
    int modulo_num = 0;
    int modulo_num_2 =10;

    for( int i =0; i<edges.size(); i++){
        if ( i == modulo_num_2){
            cout  << edges[i].weight << "\n";
            modulo_num_2 += 11;
        }
        else {
            cout << edges[i].weight<< ",";
        }
    }
    for( int i =0; i<edges.size(); i++){
        if( i % modulo_num ==0){
            modulo_num +=12;
            diagonal.push_back(edges[i]);
            continue;
        }
        edges[i].weight = log10(edges[i].weight) * -1;
    }
    // total number of nodes in the graph (labelled from 0 to 10)
    int n = 11;
    // construct graph
    Graph graph(edges, n*n);
    findShortestPaths(graph, source, n, diagonal);

    return 0;
}