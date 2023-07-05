#pragma once

#include<iostream>
#include<vector>
#include<tuple>
#include<limits>

#include "HungarianClass/types.h"

float find_shortest_cycle(std::vector<mine_t>& ores)
{
	float sum = 0;
	int counter = 0;
	int i = 0, j = 0;
	int ores_no = ores.size();
	float min = std::numeric_limits<float>::max();
	std::vector<int> route(ores_no + 1);
	std::vector<bool> visited(ores_no, false);

	std::vector<std::vector<float>> distances(ores_no, std::vector<float>(ores_no));
	for (int i = 0; i < ores_no; ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			float dx = ores[i].position.first - ores[j].position.first;
			float dy = ores[i].position.second - ores[j].position.second;
			distances[i][j] = dx * dx + dy * dy;
			distances[j][i] = distances[i][j];
		}
	}
	while (i < ores_no && j < ores_no)
	{
		if (counter >= ores_no) break;

		if (j != i && !visited[j])
			if (distances[i][j] < min)
			{
				min = distances[i][j];
				route[counter] = j;
			}
		++j;

		if (j == ores_no)
		{
			sum += min;
			min = std::numeric_limits<float>::max();
			j = 0;
			i = route[counter];
			visited[route[counter]] = true;
			++counter;
		}
	}
	i = route[counter];

	for (j = 0; j < ores_no; ++j)
	{
		if ((i != j) && distances[i][j] < min)
		{
			min = distances[i][j];
			route[counter] = j;
		}
	}
	sum += min;

	for (auto& e : route)
		std::cout << e << " position: (" << ores[e].position.first << ", " <<  ores[e].position.second <<")\n";

	return sum;
}