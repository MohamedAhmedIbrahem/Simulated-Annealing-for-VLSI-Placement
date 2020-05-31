#include <iostream>
#include<bits/stdc++.h>
using namespace std;
const int grid_size = 5;
const int number_of_cells = 20;
int grid[grid_size][grid_size];
map<int, pair<int, int>> mp;

double calculate_cost(vector<vector<int>> connections) {
    double cost = 0;
    for(int i = 0; i < number_of_cells; i++)
    {
        int x1 = mp[i+1].first;
        int y1 = mp[i+1].second;
        for(int j = 0; j < connections[i].size(); j++)
        {
            int x2 = mp[connections[i][j]].first;
            int y2 = mp[connections[i][j]].second;
            cost += sqrt(pow(x2 - x1, 2) * 1.0 + pow(y2 - y1, 2) * 1.0); ;
        }
    }
    return cost;
}

void swap_two_cells(int cell_1, int cell_2)
{
    int x1 = mp[cell_1].first;
    int y1 = mp[cell_1].second;
    int x2 = mp[cell_2].first;
    int y2 = mp[cell_2].second;
    mp[cell_1].first = x2;
    mp[cell_1].second = y2;
    mp[cell_2].first = x1;
    mp[cell_2].second = y1;

    int temp = grid[x1][y1];
    grid[x1][y1] = grid[x2][y2];
    grid[x2][y2] = temp;
}

void move_cell_down(int cell) {
    int x = mp[cell].first;
    int y = mp[cell].second;
    if (x < grid_size - 1 && grid[x + 1][y] == 0)
    {
        mp[cell].first = x + 1;
        grid[x][y] = 0;
        grid[x + 1][y] = cell;
    }
}

void move_cell_up(int cell) {
    int x = mp[cell].first;
    int y = mp[cell].second;
    if(x > 0 && grid[x - 1][y] == 0)
    {
        mp[cell].first = x - 1;
        grid[x][y] = 0;
        grid[x - 1][y] = cell;
    }
}

void move_cell_right(int cell) {
    int x = mp[cell].first;
    int y = mp[cell].second;
    if(y < grid_size - 1 && grid[x][y + 1] == 0)
    {
        mp[cell].second = y + 1;
        grid[x][y] = 0;
        grid[x][y + 1] = cell;
    }
}

void move_cell_left(int cell) {
    int x = mp[cell].first;
    int y = mp[cell].second;
    if(y > 0 && grid[x][y - 1] == 0)
    {
        mp[cell].second = y - 1;
        grid[x][y] = 0;
        grid[x][y - 1] = cell;
    }
}
bool probability (double diff, double T)
{
    double prob = exp(diff/T);
    if(prob < rand())
        return true;
    return false;
}
void simulated_annealing(vector<vector<int>> connections) {
    double T = 1;
    double Tmin = 0.0001;
    double alpha = 0.9;
    int number_of_iterations = 10;
    int number_of_swaps = 0;
    int number_of_moves = 0;
    while (T > Tmin) {
        for (int i = 0; i < number_of_iterations; i++){
            int min_diff = INT_MIN, min_diff_index = -1;
            double curr_cost = calculate_cost(connections);
            int index = rand() % number_of_cells + 1;
            int index_2 = rand() % number_of_cells + 1;
            move_cell_left(index);
            double new_cost = calculate_cost(connections);
            double diff_left = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_left && diff_left != 0) ? 0 : min_diff_index;
            min_diff = (min_diff < diff_left && diff_left != 0) ? diff_left : min_diff;
            if(diff_left != 0)
                move_cell_right(index);

            move_cell_right(index);
            new_cost = calculate_cost(connections);
            double diff_right = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_right && diff_right != 0) ? 1 : min_diff_index;
            min_diff = (min_diff < diff_right && diff_right != 0) ? diff_right : min_diff;
            if(diff_right != 0)
                move_cell_left(index);

            move_cell_up(index);
            new_cost = calculate_cost(connections);
            double diff_up = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_up && diff_up != 0) ? 2 : min_diff_index;
            min_diff = (min_diff < diff_up &&  diff_up != 0) ? diff_up : min_diff;
            if(diff_up != 0)
                move_cell_down(index);

            move_cell_down(index);
            new_cost = calculate_cost(connections);
            double diff_down = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_down && diff_down != 0) ? 3 : min_diff_index;
            min_diff = (min_diff < diff_down && diff_down != 0) ? diff_down : min_diff;
            if(diff_down != 0)
                move_cell_up(index);

            swap_two_cells(index, index_2);
            new_cost = calculate_cost(connections);
            double diff_swap = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_swap && diff_swap != 0) ? 4 : min_diff_index;
            min_diff = (min_diff < diff_swap && diff_swap != 0) ? diff_swap : min_diff;
            if(diff_swap != 0)
                swap_two_cells(index_2, index);

            switch(min_diff_index){
                case 0:
                    move_cell_left(index);
                    cout<<"Moving "<<index<<" to the left"<<endl;
                    number_of_moves++;
                    break;
                case 1:
                    move_cell_right(index);
                    cout<<"Moving "<<index<<" to the right"<<endl;
                    number_of_moves++;
                    break;
                case 2:
                    move_cell_up(index);
                    cout<<"Moving "<<index<<" up"<<endl;
                    number_of_moves++;
                    break;
                case 3:
                    move_cell_down(index);
                    cout<<"Moving "<<index<<" down"<<endl;
                    number_of_moves++;
                    break;
                case 4:
                    swap_two_cells(index, index_2);
                    cout<<"Swapping "<<index<<" with "<<index_2<<endl;
                    number_of_swaps++;
                    break;
            }
            if(min_diff < 0)
            {
                if(probability(min_diff, T))
                {
                    switch(min_diff_index) {
                        case 0:
                            move_cell_right(index);
                            cout << "Reverting left move"<<endl;
                            number_of_moves--;
                            break;
                        case 1:
                            move_cell_left(index);
                            cout << "Reverting right move"<<endl;
                            number_of_moves--;
                            break;
                        case 2:
                            move_cell_down(index);
                            cout << "Reverting up move"<<endl;
                            number_of_moves--;
                            break;
                        case 3:
                            move_cell_up(index);
                            cout << "Reverting down move"<<endl;
                            number_of_moves--;
                            break;
                        case 4:
                            swap_two_cells(index_2, index);
                            cout << "Reverting swap"<<endl;
                            number_of_swaps--;
                            break;
                    }
                }
            }

        }
        T *= alpha;
    }
    cout<<"Total number of swaps = "<<number_of_swaps<<endl;
    cout<<"Total number of moves = "<<number_of_moves<<endl;

}

int main() {
    vector<vector<int>> connections={{2, 2, 3}, {1, 1, 3}, {1, 2}, {5}, {4,6}, {5}, {8}, {7}, {},
                                     {},{12, 13}, {11, 13}, {11, 12, 15}, {15, 16, 19, 20},
                                     {13, 14, 16, 19, 20}, {14, 15, 19, 20}, {}, {},
                                     {14, 15, 16, 20}, {14, 15, 16, 19}};
//    vector<vector<int>> connections={{2, 2, 3}, {1, 1, 3}, {1, 2}, {5}, {4,6}, {5}, {8}, {7}};
    for(int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
        {
            cin >> grid[i][j];
            mp[grid[i][j]] = make_pair(i, j);
        }
    double cost = calculate_cost(connections);
    cout<<"Intial cost = "<<cost<<endl;
    simulated_annealing(connections);
    cost = calculate_cost(connections);
    cout<<"Cost after Simulated annealing = "<<cost<<endl;
    cout<<"Optimal grid :"<<endl;
    for(int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
            cout<<grid[i][j]<<" ";
        cout<<endl;
    }
    return 0;
}
