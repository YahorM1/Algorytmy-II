#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include "project.h"
using namespace std;

void displayMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }
}

void displayAssignment(const Matrix& assignment, const vector<Krasnoludek>& krasnoludki, const vector<Zloze>& zloza) {
    for (int i = 0; i < assignment.size(); i++) {
        const Krasnoludek& krasnoludek = krasnoludki[i];
        const Zloze& zloze = zloza[assignment[i][0]];
        cout << krasnoludek.imie << " -> Zloze " << zloze.nazwa << endl;
    }
}

void addEdge(vector<vector<Edge>>& graph, const Vertex& source, const Vertex& destination, int weight) {
    Edge edge;
    edge.source = source;
    edge.destination = destination;
    edge.weight = weight;

    graph[source.id].push_back(edge);
    graph[destination.id].push_back(edge);
}

vector<Edge> findMinimumSpanningTree(const vector<vector<Edge>>& graph) {
    // Inicjalizacja wierzchołka początkowego
    Vertex startVertex = graph[0][0].source;

    // Tworzenie kolejki priorytetowej i dodawanie krawędzi wychodzących z wierzchołka startowego
    priority_queue<Edge> pq;
    for (const Edge& edge : graph[startVertex.id]) {
        pq.push(edge);
    }

    // Inicjalizacja tablicy odwiedzonych wierzchołków
    vector<bool> visited(graph.size(), false);
    visited[startVertex.id] = true;

    // Inicjalizacja minimalnego drzewa spinającego
    vector<Edge> minimumSpanningTree;

    // Główna pętla algorytmu
    while (!pq.empty()) {
        Edge currentEdge = pq.top();
        pq.pop();

        Vertex currentVertex;
        if (visited[currentEdge.source.id]) {
            currentVertex = currentEdge.destination;
        } else {
            currentVertex = currentEdge.source;
        }

        if (!visited[currentVertex.id]) {
            visited[currentVertex.id] = true;
            minimumSpanningTree.push_back(currentEdge);

            // Dodaj wszystkie krawędzie wychodzące z bieżącego wierzchołka do kolejki priorytetowej
            for (const Edge& edge : graph[currentVertex.id]) {
                pq.push(edge);
            }
        }
    }

    return minimumSpanningTree;
}

int findMin(const Matrix& matrix) {
    int minVal = numeric_limits<int>::max();
    for (const auto& row : matrix) {
        for (int val : row) {
            if (val < minVal) {
                minVal = val;
            }
        }
    }
    return minVal;
}


void reduceMatrix(Matrix& matrix) {
    // Redukcja wierszy
    for (auto& row : matrix) {
        int minVal = *min_element(row.begin(), row.end());
        for (int& val : row) {
            val -= minVal;
        }
    }

    // Redukcja kolumn
    for (size_t col = 0; col < matrix[0].size(); ++col) {
        int minVal = numeric_limits<int>::max();
        for (size_t row = 0; row < matrix.size(); ++row) {
            if (matrix[row][col] < minVal) {
                minVal = matrix[row][col];
            }
        }
        for (size_t row = 0; row < matrix.size(); ++row) {
            matrix[row][col] -= minVal;
        }
    }
}

bool isColumnAssigned(const Matrix& assignment, int col) {
    for (const auto& row : assignment) {
        if (row[col] == 1) {
            return true;
        }
    }
    return false;
}

int findAssignmentForColumn(const Matrix& matrix, Matrix& assignment, int col) {
    int row = -1;
    int minVal = numeric_limits<int>::max();

    for (size_t i = 0; i < matrix.size(); ++i) {
        if (matrix[i][col] == 0 && !isColumnAssigned(assignment, col) && matrix[i][col] < minVal) {
            minVal = matrix[i][col];
            row = i;
        }
    }

    if (row != -1) {
        assignment[row][col] = 1;
    }

    return row;
}

Matrix hungarianAlgorithm(const Matrix& matrix) {
    Matrix assignment(matrix.size(), vector<int>(matrix[0].size(), 0));

    // Redukcja macierzy
    Matrix reducedMatrix = matrix;
    reduceMatrix(reducedMatrix);

    // Przypisanie początkowe
    for (size_t col = 0; col < reducedMatrix[0].size(); ++col) {
        findAssignmentForColumn(reducedMatrix, assignment, col);
    }

    // Kolejne przypisania
    for (size_t col = 0; col < reducedMatrix[0].size(); ++col) {
        while (!isColumnAssigned(assignment, col)) {
            int row = findAssignmentForColumn(reducedMatrix, assignment, col);
            if (row != -1) {
                for (size_t c = 0; c < reducedMatrix[0].size(); ++c) {
                    if (c != col) {
                        assignment[row][c] = 0;
                    }
                }
            } else {
                int minVal = numeric_limits<int>::max();
                for (size_t r = 0; r < reducedMatrix.size(); ++r) {
                    if (reducedMatrix[r][col] < minVal) {
                        minVal = reducedMatrix[r][col];
                    }
                }
                for (size_t r = 0; r < reducedMatrix.size(); ++r) {
                    reducedMatrix[r][col] -= minVal;
                }
            }
        }
    }

    return assignment;
}


int fordFulkerson(vector<vector<Edge>>& graph, const Vertex& source, const Vertex& sink) {
    // Inicjalizacja przepływu początkowego na wszystkich krawędziach
    for (auto& edges : graph) {
        for (auto& edge : edges) {
            edge.weight = 0;
        }
    }

    int maxFlow = 0;

    while (true) {
        // Inicjalizacja tablicy odwiedzonych wierzchołków
        vector<bool> visited(graph.size(), false);

        // Inicjalizacja tablicy przechowującej minimalną pojemność krawędzi na ścieżce
        vector<int> minCapacity(graph.size(), numeric_limits<int>::max());

        // Inicjalizacja tablicy przechowującej poprzednią krawędź na ścieżce
        vector<Edge*> previous(graph.size(), nullptr);

        // Tworzenie kolejki BFS i dodawanie źródła
        queue<Vertex> q;
        q.push(source);
        visited[source.id] = true;

        while (!q.empty()) {
            Vertex currentVertex = q.front();
            q.pop();

            // Przeglądanie wszystkich sąsiadujących krawędzi
            for (Edge& edge : graph[currentVertex.id]) {
                Vertex neighborVertex;
                if (edge.source.id == currentVertex.id) {
                    neighborVertex = edge.destination;
                } else {
                    neighborVertex = edge.source;
                }

                // Jeśli sąsiad nie był odwiedzony i przepustowość krawędzi jest większa od 0
                if (!visited[neighborVertex.id] && edge.capacity > edge.weight) {
                    visited[neighborVertex.id] = true;
                    minCapacity[neighborVertex.id] = min(minCapacity[currentVertex.id], edge.capacity - edge.weight);
                    previous[neighborVertex.id] = &edge;

                    q.push(neighborVertex);

                    // Jeśli osiągnięto ujście, zakończ
                    if (neighborVertex.id == sink.id) {
                        break;
                    }
                }
            }
        }

        // Jeśli nie znaleziono ścieżki, zakończ
        if (!visited[sink.id]) {
            break;
        }

        // Zwiększ przepływ na ścieżce o minimalną pojemność krawędzi
        int pathFlow = minCapacity[sink.id];
        maxFlow += pathFlow;

        Vertex currentVertex = sink;
        while (currentVertex.id != source.id) {
            Edge* edge = previous[currentVertex.id];

            // Znajdź krawędź w przeciwnym kierunku
            for (Edge& reverseEdge : graph[currentVertex.id]) {
                if (reverseEdge.source.id == edge->destination.id && reverseEdge.destination.id == edge->source.id) {
                    reverseEdge.weight -= pathFlow;
                    break;
                }
            }

            edge->weight += pathFlow;

            if (edge->source.id == currentVertex.id) {
                currentVertex = edge->destination;
            } else {
                currentVertex = edge->source;
            }
        }
    }

    return maxFlow;
}

