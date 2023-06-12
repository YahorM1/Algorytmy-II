#include <iostream>
#include <vector>

struct Krasnoludek {
    std::string imie;
    std::string preferowanyMineral;
    int odlegloscDoKopalni;
    std::string przypisanaKopalnia;
};

struct Kopalnia {
    std::string nazwa;
    std::string rodzajMineralu;
    int wydajnosc;
    int liczbaPracownikow;
};

std::vector<std::vector<int>> generateCostMatrix(const std::vector<Krasnoludek>& krasnoludki, const std::vector<Kopalnia>& kopalnie)
{
    int numKrasnoludki = krasnoludki.size();
    int numKopalnie = kopalnie.size();

    std::vector<std::vector<int>> costMatrix(numKrasnoludki, std::vector<int>(numKopalnie, 0));

    for (int i = 0; i < numKrasnoludki; i++) {
        for (int j = 0; j < numKopalnie; j++) {
            costMatrix[i][j] = krasnoludki[i].odlegloscDoKopalni * (krasnoludki[i].preferowanyMineral != kopalnie[j].rodzajMineralu);
        }
    }

    return costMatrix;
}

void hungarianAlgorithm(std::vector<std::vector<int>>& costMatrix)
{
    int rows = costMatrix.size();
    int cols = costMatrix[0].size();

    std::vector<bool> rowCovered(rows, false);
    std::vector<bool> colCovered(cols, false);
    std::vector<int> rowAssignments(rows, -1);
    std::vector<int> colAssignments(cols, -1);

    // Redukcja macierzy
    for (int i = 0; i < rows; i++) {
        int minCost = costMatrix[i][0];
        for (int j = 1; j < cols; j++) {
            if (costMatrix[i][j] < minCost) {
                minCost = costMatrix[i][j];
            }
        }
        for (int j = 0; j < cols; j++) {
            costMatrix[i][j] -= minCost;
        }
    }

    // Przypisanie zerowym linii i kolumnom
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (costMatrix[i][j] == 0 && rowAssignments[i] == -1 && colAssignments[j] == -1) {
                rowAssignments[i] = j;
                colAssignments[j] = i;
                break;
            }
        }
    }

    // Uaktualnienie przypisań w przypadku konfliktów
    bool solutionFound = false;
    while (!solutionFound) {
        std::vector<bool> markedRows(rows, false);
        std::vector<bool> markedCols(cols, false);
        int row = -1, col = -1;

        // Sprawdzenie czy istnieje oznaczona kolumna
        for (int j = 0; j < cols; j++) {
            if (colAssignments[j] == -1) {
                col = j;
                break;
            }
        }

        if (col == -1) {
            solutionFound = true;
        } else {
            while (true) {
                // Szukanie zaznaczonej komórki w tej kolumnie
                int i = colAssignments[col];
                markedRows[i] = true;

                // Sprawdzanie czy istnieje oznaczona kolumna w tej linii
                bool markedColFound = false;
                for (int j = 0; j < cols; j++) {
                    if (markedCols[j]) {
                        continue;
                    }
                    if (costMatrix[i][j] == 0) {
                        col = j;
                        markedColFound = true;
                        break;
                    }
                }

                if (markedColFound) {
                    markedCols[col] = true;
                    row = colAssignments[col];
                    if (row == -1) {
                        // Przypisanie zerowej komórki
                        while (col != -1) {
                            row = colAssignments[col];
                            colAssignments[col] = row;
                            int nextCol = rowAssignments[row];
                            rowAssignments[row] = col;
                            col = nextCol;
                        }
                        break;
                    }
                } else {
                    // Powiększenie oznaczonych kolumn
                    std::vector<bool> visited(rows, false);
                    visited[i] = true;

                    while (true) {
                        int nextCol = rowAssignments[row];
                        markedCols[col] = true;
                        rowAssignments[row] = col;
                        colAssignments[col] = row;
                        if (nextCol == -1) {
                            break;
                        }
                        row = nextCol;
                        col = rowAssignments[row];
                        visited[row] = true;
                    }

                    // Zaznaczenie wolnych linii i nieoznaczonych kolumn
                    for (int j = 0; j < cols; j++) {
                        if (markedCols[j]) {
                            continue;
                        }
                        for (int i = 0; i < rows; i++) {
                            if (visited[i]) {
                                continue;
                            }
                            if (costMatrix[i][j] == 0) {
                                markedRows[i] = true;
                                markedCols[j] = true;
                                break;
                            }
                        }
                    }

                    // Przejście do etapu sprawdzania zaznaczonych linii
                    break;
                }
            }
        }
    }

    // Wynik algorytmu - przyporządkowanie krasnoludków do kopalń
    for (int i = 0; i < rows; i++) {
        std::cout << "Krasnal " << i + 1 << " -> Kopalnia " << rowAssignments[i] + 1 << std::endl;
    }
}

int main()
{
    std::vector<Krasnoludek> listaKrasnoludkow = {
        {"Krasnal 1", "Zloto", 5, ""},
        {"Krasnal 2", "Wegiel", 10, ""},
        {"Krasnal 3", "Ruda miedzi", 8, ""},
        {"Krasnal 4", "Srebro", 7, ""},
        {"Krasnal 5", "Zloto", 9, ""},
        {"Krasnal 6", "Ruda miedzi", 6, ""},
        {"Krasnal 7", "Srebro", 12, ""},
    };

    std::vector<Kopalnia> listaKopalni = {
        {"Kopalnia 1", "Zloto", 100, 50},
        {"Kopalnia 2", "Wegiel", 80, 60},
        {"Kopalnia 3", "Ruda miedzi", 90, 40},
        {"Kopalnia 4", "Srebro", 70, 70},
    };

    std::vector<std::vector<int>> costMatrix = generateCostMatrix(listaKrasnoludkow, listaKopalni);
    hungarianAlgorithm(costMatrix);

    return 0;
}
