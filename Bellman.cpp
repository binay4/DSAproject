#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <ctime>
#include <string>

#define MAX_VERTICES 70

struct AdjListNode {
    int dest;
    int weight;
    AdjListNode* next;

    AdjListNode(int dest, int weight)
        : dest(dest), weight(weight), next(nullptr) {}
};

struct AdjList {
    AdjListNode* head;
    AdjList() : head(nullptr) {}
};

class Graph {
public:
    int V;
    std::vector<AdjList> array;
    std::vector<std::string> IATA_codes;

    Graph(int V) : V(V), array(V), IATA_codes(V) {}

    void addEdge(int src, int dest, int weight) {
        AdjListNode* newNode = new AdjListNode(dest, weight);
        newNode->next = array[src].head;
        array[src].head = newNode;
    }

    int findVertexIndex(const std::string& iata) {
        for (int i = 0; i < V; ++i) {
            if (IATA_codes[i] == iata) {
                return i;
            }
        }
        return -1;
    }
};

void readAirports(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::getline(file, line); // Skip header

    int index = 0;
    while (std::getline(file, line) && index < graph.V) {
        std::stringstream ss(line);
        std::string name, city, country, iata;
        std::getline(ss, name, ',');
        std::getline(ss, city, ',');
        std::getline(ss, country, ',');
        std::getline(ss, iata, ',');
        
        graph.IATA_codes[index] = iata;
        index++;
    }

    file.close();
}

void readRoutes(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string sourceIATA, destIATA;
        int weight;
        std::getline(ss, sourceIATA, ',');
        std::getline(ss, destIATA, ',');
        ss >> weight;

        int src = graph.findVertexIndex(sourceIATA);
        int dest = graph.findVertexIndex(destIATA);

        if (src != -1 && dest != -1) {
            graph.addEdge(src, dest, weight);
        }
    }

    file.close();
}

void printPath(const std::vector<int>& parent, int j, const Graph& graph) {
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j], graph);
    std::cout << " -> " << graph.IATA_codes[j];
}

void printSolution(const std::vector<int>& dist, const std::vector<int>& parent, int src, int dest, const Graph& graph) {
    std::cout << "Vertex   Distance from Source   Path\n";
    if (dist[dest] == INT_MAX) {
        std::cout << "No path from " << graph.IATA_codes[src] << " to " << graph.IATA_codes[dest] << "\n";
        return;
    }
    std::cout << graph.IATA_codes[dest] << " \t\t " << dist[dest] << " \t\t\t " << graph.IATA_codes[src];
    printPath(parent, dest, graph);
    std::cout << "\n";
}

void bellmanFord(Graph& graph, int src, int dest) {
    int V = graph.V;
    std::vector<int> dist(V, INT_MAX);
    std::vector<int> parent(V, -1);

    dist[src] = 0;

    for (int i = 1; i <= V - 1; i++) {
        for (int u = 0; u < V; u++) {
            AdjListNode* pCrawl = graph.array[u].head;
            while (pCrawl != nullptr) {
                int v = pCrawl->dest;
                int weight = pCrawl->weight;
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
                pCrawl = pCrawl->next;
            }
        }
    }

    for (int u = 0; u < V; u++) {
        AdjListNode* pCrawl = graph.array[u].head;
        while (pCrawl != nullptr) {
            int v = pCrawl->dest;
            int weight = pCrawl->weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                std::cout << "Graph contains negative-weight cycle\n";
                return;
            }
            pCrawl = pCrawl->next;
        }
    }

    printSolution(dist, parent, src, dest, graph);
}

int main() {
    Graph graph(MAX_VERTICES);

    readAirports("airports_final.csv", graph);
    readRoutes("routes_final.csv", graph);

    std::string sourceIATA, destIATA;

    std::cout << "Enter the source airport IATA code: ";
    std::cin >> sourceIATA;
    std::cout << "Enter the destination airport IATA code: ";
    std::cin >> destIATA;

    int src = graph.findVertexIndex(sourceIATA);
    int dest = graph.findVertexIndex(destIATA);

    if (src != -1 && dest != -1) {
        clock_t start_time = clock();

        bellmanFord(graph, src, dest);

        clock_t end_time = clock();

        double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;

        std::cout << "Time taken by Bellman-Ford algorithm: " << time_taken << " seconds\n";
    } else {
        std::cout << "Source or destination airport not found.\n";
    }

    return 0;
}
