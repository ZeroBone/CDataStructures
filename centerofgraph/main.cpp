#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

// #define WEIGHT_INFINITY std::numeric_limits<weight_t>::max()
#define WEIGHT_INFINITY 1000000
typedef unsigned int weight_t;

/////////////////////////////////////////

typedef struct linkedlist_node_s {
    struct linkedlist_node_s* previous;
    struct linkedlist_node_s* next;
} doublelinkedlist_node_t;

template <class T>
class LinkedList {

public:
    T* firstNode;
    T* lastNode;

};

/////////////////////////////////////////

class OrientedGraph {

public:
    const size_t nodes;

protected:
    explicit OrientedGraph(const size_t nodes): nodes(nodes) {}

public:
    virtual ~OrientedGraph() = default;

    virtual void createPath(size_t i, size_t j, weight_t weight) = 0;

    virtual void debugPrint() = 0;

    static void printWeight(weight_t weight) {
        if (weight == WEIGHT_INFINITY) {
            std::cout << "INF";
        }
        else {
            std::cout << weight;
        }
    }

};

class MatrixOrientedGraph : public OrientedGraph {

private:
    weight_t** matrix;

public:
    explicit MatrixOrientedGraph(const size_t nodes): OrientedGraph(nodes) {

        matrix = new weight_t*[nodes];

        for (size_t i = 0; i < nodes; i++) {
            matrix[i] = new weight_t[nodes];

            for (size_t j = 0; j < nodes; j++) {
                matrix[i][j] = i == j ? 0 : WEIGHT_INFINITY;
            }

        }

    }

    ~MatrixOrientedGraph() override {

        for (size_t i = 0; i < nodes; i++) {
            delete[] matrix[i];
        }

        delete[] matrix;

    }

    void createPath(const size_t i, const size_t j, const weight_t weight) override {
        matrix[i][j] = weight;
    }

    void debugPrint() override;

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

    weight_t* rowSums = new weight_t[nodes];

    for (size_t y = 0; y < nodes; y++) {
        rowSums[y] = matrix[y][0];
    }

    for (size_t y = 0; y < nodes; y++) {
        for (size_t x = 1; x < nodes; x++) {
            rowSums[y] += matrix[y][x];
        }
    }

    return rowSums;

    /*
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
     */

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

//////////////////////////////

class TriangleMatrixOrientedGraph : public OrientedGraph {

private:
    weight_t** matrix;

public:
    explicit TriangleMatrixOrientedGraph(const size_t nodes): OrientedGraph(nodes) {

        matrix = new weight_t*[nodes];

        matrix[0] = nullptr;

        for (size_t i = 1; i < nodes; i++) {
            matrix[i] = new weight_t[i];

            for (size_t j = 0; j < i; j++) {
                matrix[i][j] = WEIGHT_INFINITY;
            }

        }

    }

    ~TriangleMatrixOrientedGraph() override {

        for (size_t i = 1; i < nodes; i++) {
            delete[] matrix[i];
        }

        delete[] matrix;

    }

    void createPath(size_t i, size_t j, const weight_t weight) override {

        if (j > i) {
            i ^= j;
            j ^= i;
            i ^= j;
        }

        matrix[i][j] = weight;
    }

    void debugPrint() override;

    void findShortestRoutes();

    weight_t* sumUpToFindEs();

};

void TriangleMatrixOrientedGraph::debugPrint() {

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

            if (j == i) {
                printf("%2d   ", 0);
            }
            else if (j >= i) {
                if (matrix[j][i] == WEIGHT_INFINITY) {
                    std::cout << "INF  ";
                }
                else {
                    printf("%2d   ", matrix[j][i]);
                }
            }
            else if (matrix[i][j] == WEIGHT_INFINITY) {
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

void TriangleMatrixOrientedGraph::findShortestRoutes() {
    // Floyd-Warshall algorithm

    // for each phase
    /*for (size_t k = 0; k < nodes; k++) {
        for (size_t i = 0; i < nodes; i++) {
            for (size_t j = 0; j < nodes; j++) {
                if (matrix[i][k] + matrix[k][j] < matrix[i][j]) {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }
    }*/

    for (size_t k = 0; k < nodes; k++) {
        for (size_t i = 0; i < nodes; i++) {
            for (size_t j = 0; j < i; j++) {

                /*if (matrix[i][k] + matrix[k][j] < matrix[i][j]) {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }*/

                // std::cout << "I: " << i << " J: " << j << " K: " << k << std::endl;

                weight_t a = k > i ? matrix[k][i] : matrix[i][k];
                weight_t b = k > j ? matrix[k][j] : j == 0 ? 0 : matrix[j][k];
                // weight_t* current = j > i ? &matrix[j][i] : &matrix[i][j];

                if (a + b < matrix[i][j]) {
                    matrix[i][j] = a + b;
                }

            }
        }
    }

}

weight_t* TriangleMatrixOrientedGraph::sumUpToFindEs() {

    weight_t* rowSums = new weight_t[nodes];

    rowSums[0] = 0;
    for (size_t y = 1; y < nodes; y++) {
        rowSums[y] = matrix[y][0];
    }

    for (size_t y = 0; y < nodes; y++) {

        for (size_t x = 1; x < y; x++) {
            rowSums[y] += matrix[y][x];
        }

        for (size_t x = y + 1; x < nodes; x++) {
            rowSums[y] += matrix[x][y];
        }

        /*if (y >= nodes / 2) {
            weight_t correspondingRowSum = rowSums[nodes - 1 - y];
            rowSums[nodes - 1 - y] += rowSums[y];
            rowSums[y] += correspondingRowSum;
        }*/

    }

    return rowSums;

}

//////////////////////////////

/*typedef struct {
    size_t index;
    weight_t weight;
} listorgraphel_t;*/

class ListOrientedGraph : public OrientedGraph {

private:
    std::vector<std::pair<size_t, weight_t>>* list;

public:
    explicit ListOrientedGraph(const size_t nodes): OrientedGraph(nodes) {

        list = new std::vector<std::pair<size_t, weight_t>>[nodes];

    }

    ~ListOrientedGraph() override {
        delete[] list;
    }

    void createPath(const size_t i, const size_t j, const weight_t weight) override {
        list[i].emplace_back(j, weight);
    }

    void debugPrint() override;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuninitialized"
    weight_t* dijkstraMinPathLength(size_t startNode) {

        weight_t* d = new weight_t[nodes];
        bool* used = new bool[nodes];

        for (size_t i = 0; i < nodes; i++) {

            d[i] = WEIGHT_INFINITY;
            used[i] = false;

        }

        d[startNode] = 0;

        for (size_t i = 0; i < nodes; i++) {
            size_t v;
            bool vFound = false;

            for (size_t j = 0; j < nodes; j++) {
                if (!used[j] && (!vFound || d[j] < d[v])) {
                    v = j;
                    vFound = true;
                }
            }

            if (d[v] == WEIGHT_INFINITY) {
                break;
            }

            used[v] = true;

            for (std::pair<size_t, weight_t> e : list[v]) {
                // e.first - node id
                // e.second - weight

                if (d[v] + e.second < d[e.first]) {
                    d[e.first] = d[v] + e.second;
                }

            }

        }

        /*std::cout << "Min path length: ";

        for (size_t i = 0; i < nodes; i++) {
            std::cout << d[i] << ' ';
        }
        std::cout << std::endl;*/

        return d;

    }
#pragma clang diagnostic pop

};

void ListOrientedGraph::debugPrint() {

    for (size_t i = 0; i < nodes; i++) {
        printf("%2u: ", (unsigned int)i + 1);
        for (std::pair<size_t, weight_t> p : list[i]) {
            // first is node id, second is weight
            std::cout << '(' << p.first + 1 << ", " << p.second << ") ";
        }
        std::cout << std::endl;
    }

}

////////////////////////////////

#undef WEIGHT_INFINITY

typedef enum {
    IMPL_MATRIX,
    IMPL_TRIANGLE_MATRIX,
    IMPL_LIST
} graph_implementation_t;

int main() {

    std::ifstream citiesFile("cities.txt");

    size_t citiesCount;
    citiesFile >> citiesCount;

    std::cout << "There are " << citiesCount << " cities." << std::endl;

    graph_implementation_t impl;

    {
        char graphType;

        citiesFile >> graphType;

        if (graphType == 'm') {
            impl = IMPL_MATRIX;
        }
        else if (graphType == 't') {
            impl = IMPL_TRIANGLE_MATRIX;
        }
        else {
            impl = IMPL_LIST;
        }
    }

    OrientedGraph* graph;

    if (impl == IMPL_MATRIX) {
        graph = new MatrixOrientedGraph(citiesCount);
    }
    else if (impl == IMPL_TRIANGLE_MATRIX) {
        graph = new TriangleMatrixOrientedGraph(citiesCount);
    }
    else {
        graph = new ListOrientedGraph(citiesCount);
    }

    {
        size_t firstCity, secondCity;
        weight_t distance;

        while (citiesFile >> firstCity) {

            citiesFile >> secondCity;
            citiesFile >> distance;

            graph->createPath(firstCity - 1, secondCity - 1, distance);

        }
    }

    std::cout << "Graph:" << std::endl;
    graph->debugPrint();

    if (impl == IMPL_MATRIX) {

        ((MatrixOrientedGraph*)graph)->findShortestRoutes();

        std::cout << "Shortest routes:" << std::endl;
        graph->debugPrint();

        weight_t* e = ((MatrixOrientedGraph*)graph)->sumUpToFindEs();

        std::cout << "Row sums:" << std::endl;

        size_t minRowIndex = 0;
        for (size_t i = 0; i < graph->nodes; i++) {

            std::cout << e[i];

            if (e[i] < e[minRowIndex]) {
                minRowIndex = i;
            }

            if (i != graph->nodes - 1) {
                std::cout << ' ';
            }

        }

        std::cout << std::endl;

        std::cout << "Graph mediane node: " << minRowIndex + 1 << std::endl;
        std::cout << "Graph mediane: ";
        OrientedGraph::printWeight(e[minRowIndex]);
        std::cout << std::endl;

        /*for (size_t i = 0; i < graph->nodes; i++) {
            std::cout << "e(" << i + 1 << ") = ";
            OrientedGraph::printWeight(e[i]);
            std::cout << std::endl;
        }

        size_t diameterIndex = ((MatrixOrientedGraph*)graph)->findDiameter(e);
        size_t radiusIndex = ((MatrixOrientedGraph*)graph)->findRadius(e);

        std::cout << "Graph diameter: ";
        OrientedGraph::printWeight(e[diameterIndex]);
        std::cout << std::endl;

        std::cout << "Graph radius: ";
        OrientedGraph::printWeight(e[radiusIndex]);
        std::cout << std::endl;

        std::cout << "Center of graph: " << radiusIndex + 1 << std::endl;
        std::cout << "Edge of graph: " << diameterIndex + 1 << std::endl;*/

        delete[] e;

    }
    else if (impl == IMPL_TRIANGLE_MATRIX) {

        ((TriangleMatrixOrientedGraph*)graph)->findShortestRoutes();

        std::cout << "Shortest routes:" << std::endl;
        graph->debugPrint();

        weight_t* e = ((TriangleMatrixOrientedGraph*)graph)->sumUpToFindEs();

        std::cout << "Row sums:" << std::endl;

        size_t minRowIndex = 0;
        for (size_t i = 0; i < graph->nodes; i++) {

            std::cout << e[i];

            if (e[i] < e[minRowIndex]) {
                minRowIndex = i;
            }

            if (i != graph->nodes - 1) {
                std::cout << ' ';
            }

        }

        std::cout << std::endl;

        std::cout << "Graph mediane: " << minRowIndex + 1 << std::endl;

        delete[] e;

    }
    else {
        // list graph implementation

        weight_t* matrixRowsSums = new weight_t[graph->nodes];

        for (size_t i = 0; i < graph->nodes; i++) {

            weight_t* distances = ((ListOrientedGraph*)graph)->dijkstraMinPathLength(i);

            matrixRowsSums[i] = distances[0];

            for (size_t j = 1; j < graph->nodes; j++) {

                matrixRowsSums[i] += distances[j];

            }

            delete[] distances;

        }

        size_t minRowIndex = 0;
        for (size_t i = 1; i < graph->nodes; i++) {
            if (matrixRowsSums[i] < matrixRowsSums[minRowIndex]) {
                minRowIndex = i;
            }
        }

        std::cout << "Graph mediane: " << minRowIndex + 1 << std::endl;

    }

    delete graph;

    system("pause");

    return 0;

}