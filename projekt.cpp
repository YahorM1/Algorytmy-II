#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <functional> // Dodane dołączenie nagłówka <functional>

using namespace std;

// Struktura reprezentująca krasnoludka
struct Krasnoludek {
    string imie;
    unordered_map<string, int> umiejetnosci;  // Mapa umiejętności: nazwa umiejętności -> wartość umiejętności
};

// Struktura reprezentująca złóż
struct Zloze {
    string nazwa;
    int wydajnosc;
};

// Struktura reprezentująca wierzchołek
struct Vertex {
    int id;
    string name;
};

// Struktura reprezentująca krawędź
struct Edge {
    Vertex source;
    Vertex destination;
    int weight;
    int capacity;  // Pojemność przepływu

    // Operator mniejszości do użycia w kolejce priorytetowej
    bool operator<(const Edge& other) const {
        return weight > other.weight;
    }
};

typedef vector<vector<int>> Matrix;

// Funkcja pomocnicza do wyświetlania macierzy
void displayMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }
}

// Funkcja pomocnicza do wyświetlania przypisań krasnoludków do złóż
void displayAssignment(const Matrix& assignment, const vector<Krasnoludek>& krasnoludki, const vector<Zloze>& zloza) {
    for (int i = 0; i < assignment.size(); i++) {
        const Krasnoludek& krasnoludek = krasnoludki[i];
        const Zloze& zloze = zloza[assignment[i][0]];
        cout << krasnoludek.imie << " -> Złoże " << zloze.nazwa << endl;
    }
}

// Funkcja pomocnicza do dodawania krawędzi do grafu
void addEdge(vector<vector<Edge>>& graph, const Vertex& source, const Vertex& destination, int weight) {
    Edge edge;
    edge.source = source;
    edge.destination = destination;
    edge.weight = weight;

    graph[source.id].push_back(edge);
    graph[destination.id].push_back(edge);
}

// Implementacja algorytmu Prima
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

// Funkcja znajdująca minimalny element w macierzy
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

// Funkcja redukująca wiersze i kolumny macierzy
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

// Funkcja sprawdzająca, czy dana kolumna ma już przypisanie
bool isColumnAssigned(const Matrix& assignment, int col) {
    for (const auto& row : assignment) {
        if (row[col] == 1) {
            return true;
        }
    }
    return false;
}

// Funkcja znajdująca przypisanie dla danej kolumny
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

// Funkcja wykonująca algorytm węgierski
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

// Implementacja algorytmu Forda-Fulkersona
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


int main() {
    
   // Utworzenie listy krasnoludków
vector<Krasnoludek> krasnoludki = {
    {"Krasnal1", {{"złoto", 2}, {"węgiel", 5}, {"ruda miedzi", 3}}},
    {"Krasnal2", {{"węgiel", 4}, {"złoto", 1}, {"ruda miedzi", 2}}},
    {"Krasnal3", {{"ruda miedzi", 3}, {"węgiel", 2}, {"złoto", 4}}},
    {"Krasnal4", {{"złoto", 3}, {"węgiel", 5}, {"ruda miedzi", 2}}},
    {"Krasnal5", {{"złoto", 1}, {"ruda miedzi", 4}, {"węgiel", 3}}},
    {"Krasnal6", {{"węgiel", 4}, {"ruda miedzi", 2}, {"złoto", 5}}},
    {"Krasnal7", {{"ruda miedzi", 3}, {"złoto", 2}, {"węgiel", 4}}},
    {"Krasnal8", {{"złoto", 4}, {"węgiel", 3}, {"ruda miedzi", 1}}},
    {"Krasnal9", {{"złoto", 2}, {"ruda miedzi", 5}, {"węgiel", 4}}},
    {"Krasnal10", {{"węgiel", 3}, {"złoto", 4}, {"ruda miedzi", 2}}},
    {"Krasnal11", {{"ruda miedzi", 4}, {"złoto", 1}, {"węgiel", 3}}},
    {"Krasnal12", {{"złoto", 3}, {"węgiel", 2}, {"ruda miedzi", 5}}},
    {"Krasnal13", {{"złoto", 5}, {"ruda miedzi", 3}, {"węgiel", 2}}},
    {"Krasnal14", {{"węgiel", 4}, {"złoto", 2}, {"ruda miedzi", 3}}},
    {"Krasnal15", {{"ruda miedzi", 2}, {"węgiel", 5}, {"złoto", 3}}},
    {"Krasnal16", {{"złoto", 1}, {"ruda miedzi", 3}, {"węgiel", 4}}},
    {"Krasnal17", {{"złoto", 4}, {"węgiel", 2}, {"ruda miedzi", 3}}},
    {"Krasnal18", {{"ruda miedzi", 5}, {"złoto", 3}, {"węgiel", 2}}},
    {"Krasnal19", {{"węgiel", 3}, {"złoto", 4}, {"ruda miedzi", 2}}},
    {"Krasnal20", {{"złoto", 2}, {"ruda miedzi", 3}, {"węgiel", 4}}}
    // Dodaj więcej krasnoludków...
};


    // Utworzenie listy złóż
    vector<Zloze> zloza = {
    {"złoto", 10},
    {"węgiel", 10},
    {"ruda miedzi", 10}
    // Dodaj więcej złóż...
};

    // Utworzenie macierzy kosztów/przychodów
    int N = krasnoludki.size();
    int M = zloza.size();
    Matrix koszty(N, vector<int>(M, 0));

    // Uzupełnienie macierzy kosztów/przychodów
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            string zloze = zloza[j].nazwa;  // Nazwa złoża
            int wartosc = 0;  // Wartość wydobycia/kosztu pracy

            if (krasnoludki[i].umiejetnosci.count(zloze) > 0) {
                wartosc = krasnoludki[i].umiejetnosci[zloze] * zloza[j].wydajnosc;
            }

            koszty[i][j] = wartosc;
        }
    }

    cout << "Macierz kosztów/przychodów:" << endl;
    displayMatrix(koszty);

    cout << "\nPrzypisanie krasnoludków do złóż:" << endl;
    Matrix assignment = hungarianAlgorithm(koszty);
    displayAssignment(assignment, krasnoludki, zloza);

    // Dodawanie krawędzi do grafu na podstawie macierzy kosztów/przychodów
    vector<vector<Edge>> graph(krasnoludki.size() + zloza.size());
    for (int i = 0; i < krasnoludki.size(); ++i) {
        for (int j = 0; j < zloza.size(); ++j) {
            Vertex krasnoludekVertex;
            krasnoludekVertex.id = i;
            krasnoludekVertex.name = krasnoludki[i].imie;

            Vertex zlozeVertex;
            zlozeVertex.id = j + krasnoludki.size();
            zlozeVertex.name = zloza[j].nazwa;

            int weight = koszty[i][j];
            addEdge(graph, krasnoludekVertex, zlozeVertex, weight);
        }
    }

    // Znalezienie minimalnego drzewa spinającego
    vector<Edge> minimumSpanningTree = findMinimumSpanningTree(graph);

    // Wyliczanie sumy wag wszystkich krawędzi w minimalnym drzewie spinającym
int totalWeight = 0;
for (const Edge& edge : minimumSpanningTree) {
    totalWeight += edge.weight;
}

    // Wyświetlenie minimalnego drzewa spinającego
    cout << endl<<"Minimalne drzewo spinajace:" << endl;
    for (const Edge& edge : minimumSpanningTree) {
        cout << edge.source.name << " -- " << edge.destination.name << " : " << edge.weight << endl;
    }

    // Wyświetlanie sumy wag
    cout << endl<<"Odleglosc, jaka musi pokonac ksiaze, aby egzekwowac zakaz spozywania jablek: " << totalWeight << endl;

    // Uzyskanie maksymalnego przepływu w grafie minimalizującego sumaryczną odległość pokonywaną przez księcia
    int maxFlow = fordFulkerson(graph, minimumSpanningTree[0].source, minimumSpanningTree[0].destination);

    cout << endl;
    cout << "Maksymalny przeplyw: " << maxFlow << endl;

    return 0;
}
