#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <ctime>
#include <string>
#include <iomanip> // Include for std::setw

#define MAX_VERTICES 100
#define INF INT_MAX
#define MAX_STRING_LENGTH 20

std::vector<std::vector<int>> graph(MAX_VERTICES, std::vector<int>(MAX_VERTICES, INF));
std::vector<std::vector<int>> predecessor(MAX_VERTICES, std::vector<int>(MAX_VERTICES, -1));

struct Airport {
    std::string name;
    std::string city;
    std::string country;
    std::string IATA;
};

// Function to read airports data from CSV
int readAirports(std::vector<Airport>& airports) {
    std::ifstream file("airports_final.csv");
    if (!file) {
        std::cerr << "Error opening airports_final.csv\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::getline(file, line); // Skip header line

    int numAirports = 0;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string name, city, country, IATA;

        std::getline(ss, name, ',');
        std::getline(ss, city, ',');
        std::getline(ss, country, ',');
        std::getline(ss, IATA);

        airports.push_back({name, city, country, IATA});
        numAirports++;
    }

    file.close();
    return numAirports;
}

// Initialize the graph with INF (except the diagonal, which is 0)
void initializeGraph(int vertices) {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
}

// Add an edge (route) between two airports with the given weight (distance)
void addEdge(int u, int v, int weight) {
    graph[u][v] = weight;
}

// Floyd-Warshall algorithm to find shortest paths between all pairs of airports
void floydWarshall(int vertices) {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (graph[i][j] != INF && i != j) {
                predecessor[i][j] = i;
            } else {
                predecessor[i][j] = -1;
            }
        }
    }

    for (int k = 0; k < vertices; k++) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (graph[i][k] != INF && graph[k][j] != INF &&
                    graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    predecessor[i][j] = predecessor[k][j];
                }
            }
        }
    }
}

// Find the index of an airport based on its IATA code
int findAirportIndex(const std::vector<Airport>& airports, const std::string& IATA) {
    for (int i = 0; i < airports.size(); i++) {
        if (airports[i].IATA == IATA) {
            return i;
        }
    }
    return -1;
}

// Print the solution matrix for all pairs of airports
void printSolution(const std::vector<Airport>& airports, int numAirports) {
    int maxDigits = 0;
    for (int i = 0; i < numAirports; i++) {
        for (int j = 0; j < numAirports; j++) {
            if (graph[i][j] != INF) {
                int numDigits = std::to_string(graph[i][j]).length();
                maxDigits = std::max(maxDigits, numDigits);
            }
        }
    }
    maxDigits++;

    // Print the matrix of shortest distances
    std::cout << "Shortest distances between every pair of airports:\n";
    for (int i = 0; i < numAirports; i++) {
        for (int j = 0; j < numAirports; j++) {
            if (graph[i][j] == INF) {
                std::cout << std::setw(maxDigits) << "-1";
            } else {
                std::cout << std::setw(maxDigits) << graph[i][j];
            }
            if (j != numAirports - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

// Print the shortest path from source to destination using the predecessor matrix
void printPath(const std::vector<Airport>& airports, int source, int destination) {
    if (source == destination) {
        std::cout << airports[source].IATA << " ";
    } else if (predecessor[source][destination] == -1) {
        std::cout << "No path exists between " << airports[source].IATA
                  << " and " << airports[destination].IATA << "\n";
    } else {
        printPath(airports, source, predecessor[source][destination]);
        std::cout << "-> " << airports[destination].IATA << " ";
    }
}

// Function to read routes data and construct the graph (routes between airports)
void readRoutesAndConstructGraph(std::vector<Airport>& airports, int numAirports) {
    std::ifstream file("routes_final.csv");
    if (!file) {
        std::cerr << "Error opening routes_final.csv\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::getline(file, line); // Skip header line

    int numEdges = 0;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string sourceIATA, destinationIATA;
        int weight;

        std::getline(ss, sourceIATA, ',');
        std::getline(ss, destinationIATA, ',');
        ss >> weight;

        int u = findAirportIndex(airports, sourceIATA);
        int v = findAirportIndex(airports, destinationIATA);

        if (u != -1 && v != -1) {
            addEdge(u, v, weight);
            numEdges++;
        }
    }

    file.close();
    std::cout << "Number of airports: " << numAirports << "\n";
    std::cout << "Number of edges added: " << numEdges << "\n";
}

int main() {
    std::vector<Airport> airports;

    int numAirports = readAirports(airports);

    initializeGraph(numAirports);

    readRoutesAndConstructGraph(airports, numAirports);

    std::string sourceIATA, destinationIATA;
    std::cout << "Enter source IATA: ";
    std::cin >> sourceIATA;
    std::cout << "Enter destination IATA: ";
    std::cin >> destinationIATA;

    int sourceIndex = findAirportIndex(airports, sourceIATA);
    int destinationIndex = findAirportIndex(airports, destinationIATA);

    if (sourceIndex == -1 || destinationIndex == -1) {
        std::cerr << "Invalid source or destination IATA.\n";
        return 1;
    }

    clock_t start_time = clock(); // Start timer

    floydWarshall(numAirports);

    clock_t end_time = clock(); // end timer
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    std::cout << "Time taken by Floyd-Warshall algorithm: " << time_taken << " seconds\n";

    printSolution(airports, numAirports);

    if (graph[sourceIndex][destinationIndex] == INF) {
        std::cout << "No path found between " << sourceIATA << " and " << destinationIATA << ".\n";
    } else {
        std::cout << "Shortest distance between " << sourceIATA << " and " << destinationIATA
                  << ": " << graph[sourceIndex][destinationIndex] << "\n";
        std::cout << "Shortest path: ";
        printPath(airports, sourceIndex, destinationIndex);
        std::cout << "\n";
    }

    return 0;
}
