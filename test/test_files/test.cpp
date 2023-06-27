#include <gtest/gtest.h>
#include "../../projekt.cpp"
using namespace std;

TEST(DisplayMatrixTest, OutputsMatrixCorrectly) {
    // Arrange
    Matrix matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    testing::internal::CaptureStdout(); // перехватываем вывод консоли

    // Act
    displayMatrix(matrix); // вызываем функцию, которую тестируем

    // Assert
    string output = testing::internal::GetCapturedStdout(); // получаем перехваченный вывод
    string expectedOutput = "1\t2\t3\t\n4\t5\t6\t\n7\t8\t9\t\n"; // ожидаемый вывод
    EXPECT_EQ(output, expectedOutput); // сравниваем перехваченный вывод с ожидаемым выводом
}

TEST(FindMinInMatrixTest, OutputsMinCorrectly){
    // Arrange
    Matrix matrix = { {29, 11, 52}, {18, 6, 33}, {13, 45, 4} };

    // Act
    int min = findMin(matrix);

    // Assert
    ASSERT_EQ(min, 4);
}

TEST(AddEdgeTest, AddEdgesCorrectly){
    // Arrange
    Vertex V1 = {0}, V2 = {1}, V3 = {2}, V4 = {3};
    vector<vector<Edge>> graph(4);

    // Act
    addEdge(graph, V1, V4, 2);
    addEdge(graph, V1, V3,7);
    addEdge(graph, V4, V2, 6);
    addEdge(graph, V3, V4, 3);

    // Assert
    ASSERT_EQ(graph.size(), 4);
    ASSERT_EQ(graph[0].size(), 2);
    ASSERT_EQ(graph[1].size(), 1);
    ASSERT_EQ(graph[2].size(), 2);
    ASSERT_EQ(graph[3].size(), 3);
}

TEST(ReduceMatrixTest, ReduceCorrectly){
    // Arrange
    Matrix matrix = { {18, 16, 5}, {14, 12, 3}, {9, 6, 15} };

    // Act
    reduceMatrix(matrix);

    // Assert
    Matrix output = { {10, 11, 0}, {8, 9, 0}, {0, 0, 9} };
    ASSERT_EQ(matrix, output);
}




