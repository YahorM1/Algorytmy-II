#include "project.h"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <queue>
using namespace std;

int main() {


    // Utworzenie listy krasnoludków
    vector<Krasnoludek> krasnoludki = {
            {"Krasnal1", {{"zloto", 2}, {"wegiel", 5}, {"ruda miedzi", 3}}},
            {"Krasnal2", {{"wegiel", 4}, {"zloto", 1}, {"ruda miedzi", 2}}},
            {"Krasnal3", {{"ruda miedzi", 3}, {"wegiel", 2}, {"zloto", 4}}},
            {"Krasnal4", {{"zloto", 3}, {"wegiel", 5}, {"ruda miedzi", 2}}},
            {"Krasnal5", {{"zloto", 1}, {"ruda miedzi", 4}, {"wegiel", 3}}},
            {"Krasnal6", {{"wegiel", 4}, {"ruda miedzi", 2}, {"zloto", 5}}},
            {"Krasnal7", {{"ruda miedzi", 3}, {"zloto", 2}, {"wegiel", 4}}},
            {"Krasnal8", {{"zloto", 4}, {"wegiel", 3}, {"ruda miedzi", 1}}},
            {"Krasnal9", {{"zloto", 2}, {"ruda miedzi", 5}, {"wegiel", 4}}},
            {"Krasnal10", {{"wegiel", 3}, {"zloto", 4}, {"ruda miedzi", 2}}},
            {"Krasnal11", {{"ruda miedzi", 4}, {"zloto", 1}, {"wegiel", 3}}},
            {"Krasnal12", {{"zloto", 3}, {"wegiel", 2}, {"ruda miedzi", 5}}},
            {"Krasnal13", {{"zloto", 5}, {"ruda miedzi", 3}, {"wegiel", 2}}},
            {"Krasnal14", {{"wegiel", 4}, {"zloto", 2}, {"ruda miedzi", 3}}},
            {"Krasnal15", {{"ruda miedzi", 2}, {"wegiel", 5}, {"zloto", 3}}},
            {"Krasnal16", {{"zloto", 1}, {"ruda miedzi", 3}, {"wegiel", 4}}},
            {"Krasnal17", {{"zloto", 4}, {"wegiel", 2}, {"ruda miedzi", 3}}},
            {"Krasnal18", {{"ruda miedzi", 5}, {"zloto", 3}, {"wegiel", 2}}},
            {"Krasnal19", {{"wegiel", 3}, {"zloto", 4}, {"ruda miedzi", 2}}},
            {"Krasnal20", {{"zloto", 2}, {"ruda miedzi", 3}, {"wegiel", 4}}}
            // Dodaj więcej krasnoludków...
    };


    // Utworzenie listy złóż
    vector<Zloze> zloza = {
            {"zloto", 10},
            {"wegiel", 10},
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

    cout << "Macierz kosztow/przychodow:" << endl;
    displayMatrix(koszty);

    cout << "\nPrzypisanie krasnoludkow do zloz:" << endl;
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
