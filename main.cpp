#include<bits/stdc++.h>
#include<stdio.h>
#include<time.h>
using namespace std;

map<int, pair<int, int>> mp;

int number_of_cells,grid_size;
vector<vector<int>>grid;
vector<int>visited;
map<int,pair<int,int>>closest;

int dx[] = { 1,0,-1,0,1,-1,-1,1 };
int dy[] = { 0,1,0,-1,1,1,-1,-1 };

vector<vector<int>> connections={{2,3}, {1,3}, {1,2}, {5}, {4,6}, {5}, {8}, {7}, {}, {},
                                     {12, 13}, {11, 13}, {11, 12, 15}, {15, 16, 19, 20},
                                     {13, 14, 16, 19, 20}, {14, 15, 19, 20}, {}, {}, {14, 15, 16, 20}, {14, 15, 16, 19}};

void size_grid()
{
    for(int i=0;i<grid_size;i++)
        grid[i].clear();
    grid.clear();
    
    visited.resize(number_of_cells+1);

    for(int i=1;i*i<=number_of_cells;i++)   
        grid_size = i;
    grid_size++;

    grid.resize(grid_size);
    for(int i=0;i<grid_size;i++)
        grid[i].resize(grid_size);
}

void random_connections(int n)
{
    connections.clear();
    connections.resize(n);
    srand(time(NULL));
    for(int i=0;i<n;i++)
    {
        int number_of_connections = rand()%n;
        for(int j=0;j<number_of_connections;j++)
        {
            int module = rand()%n;
            if(module==i)
                continue;
            connections[i].push_back(module+1);
            connections[module].push_back(i+1);
        }
    }
}

void random_grid(int n)
{
    size_grid();
    int grid_total = grid_size*grid_size;
    vector<int>arr(grid_total);
    for(int i=0;i<n;i++)    
        arr[i]=i+1;
    random_shuffle(arr.begin(),arr.end());
    int cur = 0;
    for(int i=0;i<grid_size;i++)
        for(int j=0;j<grid_size;j++)
            grid[i][j] = arr[cur++];
}
bool cmp(const pair<int,int>&a,const pair<int,int>&b)
{
    if(a.first == b.first)  
        return a.second<b.second;
    return a.first>b.first;
}
vector<vector<pair<int,int>>>WCG;
vector<int>CNG;
void create_WCG()
{
    vector<map<int,int>>graph(number_of_cells+1);
    WCG.resize(number_of_cells+1);
    for(int i=0;i<number_of_cells;i++)
    {
        for(int j=0;j<connections[i].size();j++)
            graph[i+1][connections[i][j]]++;
    }
    for(int i=1;i<=number_of_cells;i++)
    {
        for(auto x:graph[i])
            WCG[i].push_back({x.second,x.first});
        sort(WCG[i].begin(),WCG[i].end(),cmp);
    }
    /*
    for(int i=1;i<=number_of_cells;i++)
    {
        cout<<"connecte to "<<i<<endl;
        for(int j=0;j<WCG[i].size();j++)
            cout<<WCG[i][j].second<<" "<<WCG[i][j].first<<endl;
    }
    */
}
void create_CNG()
{
    CNG.resize(number_of_cells+1);
    for(int i=1;i<=number_of_cells;i++)
    {
        if(WCG[i].size())
            CNG[i] = WCG[i][0].second;
        else 
            CNG[i] = i;
    }
    /*
    for(int i=1;i<CNG.size();i++)
        cout<<i<<" "<<CNG[i]<<endl;
    */
}

bool valid(int i,int j)
{
    return (i>=0 && i<grid_size && j>=0 && j<grid_size);
}

pair<int,int> get_closest(int i,int j)
{
    queue<pair<int,int>>q;

    vector<vector<int>>vis(number_of_cells+1);
    for(int i=0;i<vis.size();i++)
        vis[i].resize(number_of_cells+1);

    q.push({i,j});
    while(!q.empty())
    {
        pair<int,int>cur = q.front();
        q.pop();
        for(int s=0;s<8;s++)
        {
            int xx = cur.first+dx[s], yy = cur.second+dy[s];
            if(valid(xx,yy) && !vis[xx][yy])
            {
                if(!grid[xx][yy])
                    return {xx,yy};
                vis[xx][yy]=1;
                q.push({xx,yy});
            }
        }
    }
}

void dfs(int i,int j,int module)
{
    visited[module] = 1;
    grid[i][j] = module;
    closest[module] = {i,j};

    int cur = 0;
    for(auto new_module : WCG[module])
    {
        if(visited[new_module.second])
            continue;
        
        for(int x = cur;x < 8;x++)
        {
            int xx = i+dx[x] , yy = j+dy[x];
            if(valid(xx,yy) && !grid[xx][yy])
            {
                dfs(xx,yy,new_module.second);
                break;
            }
        }
    }
}
void initial_placement()
{
    create_WCG();
    create_CNG();
    size_grid();

    vector<pair<int,int>>order(number_of_cells+1);
    for(int i=1;i<=number_of_cells;i++)
    {
        order[CNG[i]].first++;
        order[i].second = i;
    }
    order[0].first = 1e9;
    sort(order.rbegin(),order.rend());

    for(int i=1;i<=number_of_cells;i++)
    {
        if(!visited[order[i].second])
        {
            if(!grid[grid_size/2][grid_size/2])
                dfs(grid_size/2,grid_size/2,order[i].second);
            else 
            {
                pair<int,int> start = {grid_size/2,grid_size/2};
                for(auto module : WCG[order[i].second])
                {
                    if(closest.count(module.second))
                    {
                        start = closest[module.second];
                        break;
                    }
                }
                start = get_closest(start.first,start.second);
                dfs(start.first,start.second,order[i].second);
            }
        }
    }
}

double calculate_cost() {
    double cost = 0;
    for(int i = 0; i < number_of_cells; i++)
    {
        int x1 = mp[i+1].first;
        int y1 = mp[i+1].second;
        for(int j = 0; j < connections[i].size(); j++)
        {
            int x2 = mp[connections[i][j]].first;
            int y2 = mp[connections[i][j]].second;
            cost += abs(x2-x1)+abs(y2-y1);
            // cost += sqrt(pow(x2 - x1, 2) * 1.0 + pow(y2 - y1, 2) * 1.0);
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
void simulated_annealing() {
    double T = 1;
    double Tmin = 0.0001;
    double alpha = 0.9;
    int number_of_iterations = 10;
    int number_of_swaps = 0;
    int number_of_moves = 0;
    while (T > Tmin) {
        for (int i = 0; i < number_of_iterations; i++){
            int min_diff = INT_MIN, min_diff_index = -1;
            double curr_cost = calculate_cost();
            int index = rand() % number_of_cells + 1;
            int index_2 = rand() % number_of_cells + 1;
            move_cell_left(index);
            double new_cost = calculate_cost();
            double diff_left = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_left && diff_left != 0) ? 0 : min_diff_index;
            min_diff = (min_diff < diff_left && diff_left != 0) ? diff_left : min_diff;
            if(diff_left != 0)
                move_cell_right(index);

            move_cell_right(index);
            new_cost = calculate_cost();
            double diff_right = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_right && diff_right != 0) ? 1 : min_diff_index;
            min_diff = (min_diff < diff_right && diff_right != 0) ? diff_right : min_diff;
            if(diff_right != 0)
                move_cell_left(index);

            move_cell_up(index);
            new_cost = calculate_cost();
            double diff_up = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_up && diff_up != 0) ? 2 : min_diff_index;
            min_diff = (min_diff < diff_up &&  diff_up != 0) ? diff_up : min_diff;
            if(diff_up != 0)
                move_cell_down(index);

            move_cell_down(index);
            new_cost = calculate_cost();
            double diff_down = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_down && diff_down != 0) ? 3 : min_diff_index;
            min_diff = (min_diff < diff_down && diff_down != 0) ? diff_down : min_diff;
            if(diff_down != 0)
                move_cell_up(index);

            swap_two_cells(index, index_2);
            new_cost = calculate_cost();
            double diff_swap = curr_cost - new_cost;
            min_diff_index = (min_diff < diff_swap && diff_swap != 0) ? 4 : min_diff_index;
            min_diff = (min_diff < diff_swap && diff_swap != 0) ? diff_swap : min_diff;
            if(diff_swap != 0)
                swap_two_cells(index_2, index);

            switch(min_diff_index){
                case 0:
                    move_cell_left(index);
                    // cout<<"Moving "<<index<<" to the left"<<endl;
                    number_of_moves++;
                    break;
                case 1:
                    move_cell_right(index);
                    // cout<<"Moving "<<index<<" to the right"<<endl;
                    number_of_moves++;
                    break;
                case 2:
                    move_cell_up(index);
                    // cout<<"Moving "<<index<<" up"<<endl;
                    number_of_moves++;
                    break;
                case 3:
                    move_cell_down(index);
                    // cout<<"Moving "<<index<<" down"<<endl;
                    number_of_moves++;
                    break;
                case 4:
                    swap_two_cells(index, index_2);
                    // cout<<"Swapping "<<index<<" with "<<index_2<<endl;
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
                            // cout << "Reverting left move"<<endl;
                            number_of_moves--;
                            break;
                        case 1:
                            move_cell_left(index);
                            // cout << "Reverting right move"<<endl;
                            number_of_moves--;
                            break;
                        case 2:
                            move_cell_down(index);
                            // cout << "Reverting up move"<<endl;
                            number_of_moves--;
                            break;
                        case 3:
                            move_cell_up(index);
                            // cout << "Reverting down move"<<endl;
                            number_of_moves--;
                            break;
                        case 4:
                            swap_two_cells(index_2, index);
                            // cout << "Reverting swap"<<endl;
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

void print_grid()
{
    cout<<"grid size "<<grid_size<<endl;
    for(int i=0;i<grid_size;i++)
    {
        for(int j=0;j<grid_size;j++)
            cout<<grid[i][j]<<" ";
        cout<<endl;
    }
    cout<<"------------------------------------------------------"<<endl;
}
void fill_map()
{
    mp.clear();
    for(int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
            mp[grid[i][j]] = make_pair(i, j);
}
int main() {
    cout<<"enter number of modules"<<endl;
    cin >> number_of_cells;

    random_connections(number_of_cells);

    random_grid(number_of_cells);
    fill_map();
    cout<<"Random grid with cost : "<<calculate_cost()<<endl;
    print_grid();

    initial_placement();
    fill_map();
    cout<<"initial grid with cost :"<<calculate_cost()<<endl;
    print_grid();

    simulated_annealing();
    cout<<"optimal grid after Simulated annealing with cost : "<<calculate_cost()<<endl;
    print_grid();

    return 0;
}
