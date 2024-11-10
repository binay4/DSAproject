#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include <ctime>
#include <sstream>

#define MAX_NAME_LENGTH 50
#define MAX_CITY_LENGTH 50
#define MAX_COUNTRY_LENGTH 50
#define MAX_IATA_LENGTH 4

#define MAX_VERTICES 70

struct AdjListNode {
    int dest;
    int weight;
    AdjListNode *next;
};

struct AdjList {
    AdjListNode *head;
};

class Graph {
public:
    int V;
    std::vector<AdjList> array;
    std::vector<std::string> IATA_codes;

    Graph(int V) {
        this->V = V;
        array.resize(V);
        IATA_codes.resize(V);
    }

    void addEdge(int src, int dest, int weight) {
        AdjListNode *newNode = new AdjListNode{dest, weight, nullptr};
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

    void readAirports(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string line;
        std::getline(file, line); // Skip header

        int index = 0;
        while (std::getline(file, line) && index < V) {
            std::istringstream ss(line);
            std::string name, city, country, iata;
            std::getline(ss, name, ',');
            std::getline(ss, city, ',');
            std::getline(ss, country, ',');
            std::getline(ss, iata);

            IATA_codes[index++] = iata;
        }

        file.close();
    }

    void readRoutes(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string line;
        std::getline(file, line); // Skip header

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string sourceIATA, destIATA;
            int weight;
            std::getline(ss, sourceIATA, ',');
            std::getline(ss, destIATA, ',');
            ss >> weight;

            int src = findVertexIndex(sourceIATA);
            int dest = findVertexIndex(destIATA);

            if (src != -1 && dest != -1) {
                addEdge(src, dest, weight);
            }
        }

        file.close();
    }

    void dijkstra(int src, int dest) {
        std::vector<int> dist(V, INT_MAX);
        std::vector<int> visited(V, 0);
        std::vector<int> parent(V, -1);

        dist[src] = 0;

        for (int count = 0; count < V - 1; ++count) {
            int u = minDistance(dist, visited);
            visited[u] = 1;

            AdjListNode* pCrawl = array[u].head;
            while (pCrawl) {
                int v = pCrawl->dest;
                if (!visited[v] && pCrawl->weight != -1 && dist[u] != INT_MAX &&
                    dist[u] + pCrawl->weight < dist[v]) {
                    dist[v] = dist[u] + pCrawl->weight;
                    parent[v] = u;
                }
                pCrawl = pCrawl->next;
            }
        }

        printSolution(dist, parent, src, dest);
    }

private:
    int minDistance(const std::vector<int>& dist, const std::vector<int>& visited) {
        int min = INT_MAX, min_index = -1;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }

        return min_index;
    }

    void printPath(const std::vector<int>& parent, int j) {
        if (parent[j] == -1)
            return;

        printPath(parent, parent[j]);
        std::cout << " -> " << IATA_codes[j];
    }

    void printSolution(const std::vector<int>& dist, const std::vector<int>& parent, int src, int dest) {
        std::cout << "Vertex   Distance from Source   Path\n";
        if (dist[dest] == INT_MAX) {
            std::cout << "No path from " << IATA_codes[src] << " to " << IATA_codes[dest] << std::endl;
            return;
        }
        std::cout << IATA_codes[dest] << " \t\t " << dist[dest] << " \t\t\t " << IATA_codes[src];
        printPath(parent, dest);
        std::cout << std::endl;
    }
};

int main() {
    Graph graph(MAX_VERTICES);

    graph.readAirports("airports_final.csv");
    graph.readRoutes("routes_final.csv");

    std::string sourceIATA, destIATA;
    std::cout << "Enter the source airport IATA code: ";
    std::cin >> sourceIATA;
    std::cout << "Enter the destination airport IATA code: ";
    std::cin >> destIATA;

    int src = graph.findVertexIndex(sourceIATA);
    int dest = graph.findVertexIndex(destIATA);

    if (src != -1 && dest != -1) {
        clock_t start_time = clock();
        graph.dijkstra(src, dest);
        clock_t end_time = clock();
        double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;
        std::cout << "Time taken by Dijkstra Algorithm: " << time_taken << " seconds\n";
    } else {
        std::cout << "Source or destination airport not found.\n";
    }

    return 0;
}
