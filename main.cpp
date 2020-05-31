#include <iostream>
#include<bits/stdc++.h>
using namespace std;
const int grid_size = 5;
const int number_of_cells = 20;
int grid[grid_size][grid_size];
map<int, pair<int, int>> mp;

float calculate_cost(vector<vector<int>> connections) {
    float cost = 0;
    for(int i = 0; i < number_of_cells; i++)
    {
        int x1 = mp[i+1].first;
        int y1 = mp[i+1].second;
        for(int j = 0; j < connections[i].size(); j++)
        {
            int x2 = mp[connections[i][j]].first;
            int y2 = mp[connections[i][j]].second;
            cost += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0); ;
        }
    }
    return cost;
}
void simulated_annealing(vector<vector<int>> connections) {
    double T = 1;
    double Tmin = 0.0001;
    double alpha = 0.9;
    int number_of_iterations = 100;
    while (T > Tmin) {
        for (int i = 0; i < number_of_iterations; i++){
            // TODO: Make a change and test if it improves cost
        }
        T *= alpha;
    }
    return;
}
int main() {
    vector<vector<int>> connections={{2,3}, {1,3}, {1,2}, {5}, {4,6}, {5}, {8}, {7}, {}, {},
                                     {12, 13}, {11, 13}, {11, 12, 15}, {15, 16, 19, 20},
                                     {13, 14, 16, 19, 20}, {14, 15, 19, 20}, {}, {}, {14, 15, 16, 20}, {14, 15, 16, 19}};

    for(int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
        {
            cin >> grid[i][j];
            mp[grid[i][j]] = make_pair(i, j);
        }
    float cost = calculate_cost(connections);
    cout<<"Intial cost = "<<cost<<endl;
    return 0;
}
