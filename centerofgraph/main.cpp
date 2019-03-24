#include <iostream>
#include <fstream>

// #define WEIGHT_INFINITY std::numeric_limits<weight_t>::max()
#define WEIGHT_INFINITY 1000000
typedef unsigned int weight_t;

class MatrixOrientedGraph {

public:
    const size_t nodes;

private:
    weight_t** matrix;

public:
    explicit MatrixOrientedGraph(const size_t nodes): nodes(nodes) {

        matrix = new weight_t*[nodes];

        for (size_t i = 0; i < nodes; i++) {
            matrix[i] = new weight_t[nodes];

            for (size_t j = 0; j < nodes; j++) {
                matrix[i][j] = i == j ? 0 : WEIGHT_INFINITY;
            }

        }

    }

    ~MatrixOrientedGraph() {

        for (size_t i = 0; i < nodes; i++) {
            delete[] matrix[i];
        }

        delete[] matrix;

    }

    void createNode(const size_t i, const size_t j, const weight_t weight) {
        matrix[i][j] = weight;
    }

    void debugPrint();

    void findShortestRoutes();

    weight_t* sumUpToFindEs();

    size_t findRadius(const weight_t* e);

    size_t findDiameter(const weight_t* e);

};

void MatrixOrientedGraph::debugPrint() {

    std::cout << "  |";

    for (size_t i = 0; i < nodes; i++) {
        printf("%2u    ", (unsigned int)i + 1);
    }

    std::cout << std::endl;
    std::cout << "--+";

    for (size_t i = 0; i < nodes; i++) {
        std::cout << "-----";
        if (i != nodes - 1) {
            std::cout << '-';
        }
    }

    std::cout << std::endl;

    for (size_t i = 0; i < nodes; i++) {

        printf("%2d|", (unsigned int)i + 1);

        for (size_t j = 0; j < nodes; j++) {

            if (matrix[i][j] == WEIGHT_INFINITY) {
                std::cout << "INF  ";
            }
            else {
                printf("%2d   ", matrix[i][j]);
            }

            if (j != nodes - 1) {
                std::cout << ' ';
            }

        }

        std::cout << std::endl;

    }

    std::cout << "--+";

    for (size_t i = 0; i < nodes; i++) {
        std::cout << "-----";
        if (i != nodes - 1) {
            std::cout << '-';
        }
    }

    std::cout << std::endl;

}

void MatrixOrientedGraph::findShortestRoutes() {
    // Floyd-Warshall algorithm

    // for each phase
    for (size_t k = 0; k < nodes; k++) {
        for (size_t i = 0; i < nodes; i++) {
            for (size_t j = 0; j < nodes; j++) {
                if (matrix[i][k] + matrix[k][j] < matrix[i][j]) {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }

        /*for (size_t j = 0; j < nodes; j++) {
            for (size_t i = 0; i < nodes; i++) {

                if (matrix[i][k] == WEIGHT_INFINITY && matrix[k][j] == INFINITY) {
                    continue;
                }

                matrix[i][j] = MIN(matrix[i][j], matrix[i][k] + matrix[k][j]);

            }
        }*/
    }

}

weight_t* MatrixOrientedGraph::sumUpToFindEs() {

    weight_t* es = new weight_t[nodes];

    memcpy(es, matrix[0], nodes * sizeof(weight_t));

    for (size_t x = 0; x < nodes; x++) {
        for (size_t y = 1; y < nodes; y++) {
            if (matrix[y][x] > es[x]) {
                es[x] = matrix[y][x];
            }
        }
    }

    return es;

}

size_t MatrixOrientedGraph::findRadius(const weight_t* e) {

    size_t radiusIndex = 0;

    for (size_t i = 1; i < nodes; i++) {
        if (e[i] < e[radiusIndex]) {
            radiusIndex = i;
        }
    }

    return radiusIndex;

}

size_t MatrixOrientedGraph::findDiameter(const weight_t* e) {

    size_t diameterIndex = 0;

    for (size_t i = 1; i < nodes; i++) {
        if (e[i] > e[diameterIndex]) {
            diameterIndex = i;
        }
    }

    return diameterIndex;

}

void printE(weight_t eValue) {
    if (eValue == WEIGHT_INFINITY) {
        std::cout << "INF";
    }
    else {
        std::cout << eValue;
    }
}

#undef WEIGHT_INFINITY

int main() {

    std::ifstream citiesFile("cities.txt");

    size_t citiesCount;
    citiesFile >> citiesCount;

    std::cout << "There are " << citiesCount << " cities." << std::endl;

    MatrixOrientedGraph* graph = new MatrixOrientedGraph(citiesCount);

    {
        size_t firstCity, secondCity;
        weight_t distance;

        while (citiesFile >> firstCity) {

            citiesFile >> secondCity;
            citiesFile >> distance;

            graph->createNode(firstCity - 1, secondCity - 1, distance);

        }
    }

    std::cout << "Graph:" << std::endl;
    graph->debugPrint();

    graph->findShortestRoutes();

    std::cout << "Shortest routes:" << std::endl;
    graph->debugPrint();

    weight_t* e = graph->sumUpToFindEs();
    for (size_t i = 0; i < graph->nodes; i++) {
        std::cout << "e(" << i + 1 << ") = ";
        printE(e[i]);
        std::cout << std::endl;
    }

    size_t diameterIndex = graph->findDiameter(e);
    size_t radiusIndex = graph->findRadius(e);

    std::cout << "Graph diameter: ";
    printE(e[diameterIndex]);
    std::cout << std::endl;

    std::cout << "Graph radius: ";
    printE(e[radiusIndex]);
    std::cout << std::endl;

    std::cout << "Center of graph: " << radiusIndex + 1 << std::endl;

    delete[] e;
    delete graph;

    system("pause");

    return 0;
}