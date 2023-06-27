#ifndef ALGORYTMY_II_PROJECT_H
#define ALGORYTMY_II_PROJECT_H
#include <vector>
#include <string>
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

void displayMatrix(const Matrix& matrix);   // Funkcja pomocnicza do wyświetlania macierzy
void displayAssignment(const Matrix& assignment, const vector<Krasnoludek>& krasnoludki, const vector<Zloze>& zloza);   // Funkcja pomocnicza do wyświetlania przypisań krasnoludków do złóż
void addEdge(vector<vector<Edge>>& graph, const Vertex& source, const Vertex& destination, int weight);     // Funkcja pomocnicza do dodawania krawędzi do grafu
vector<Edge> findMinimumSpanningTree(const vector<vector<Edge>>& graph);    // Implementacja algorytmu Prima
int findMin(const Matrix& matrix);      // Funkcja znajdująca minimalny element w macierzy
void reduceMatrix(Matrix& matrix);      // Funkcja redukująca wiersze i kolumny macierzy
bool isColumnAssigned(const Matrix& assignment, int col);   // Funkcja sprawdzająca, czy dana kolumna ma już przypisanie
int findAssignmentForColumn(const Matrix& matrix, Matrix& assignment, int col);     // Funkcja znajdująca przypisanie dla danej kolumny
Matrix hungarianAlgorithm(const Matrix& matrix);        // Funkcja wykonująca algorytm węgierski
int fordFulkerson(vector<vector<Edge>>& graph, const Vertex& source, const Vertex& sink);   // Implementacja algorytmu Forda-Fulkersona



#endif //ALGORYTMY_II_PROJECT_H
