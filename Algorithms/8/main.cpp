#include<iostream>
#include<vector>
#include<climits>

using namespace std;
const int infinity = INT_MAX;

struct Edge {
    int adjNode;
    int cNode;
    int cPrice;
    int adjPrice;
};

int DFS(const vector<vector<Edge> > &cities,
        vector<int> &visit,
        int node, int n);

int main() {
    int roadNum, cityNum;
    cin >> cityNum >> roadNum;

    // read edges
    vector<vector<Edge> > cities(cityNum + 1, vector<Edge>());
    for (int i = 0; i < roadNum; ++i) {
        int a, b, c, p, r;
        cin >> a >> b >> c >> p >> r;
        Edge temp;
        temp.adjNode = b;
        temp.cNode = c;
        temp.cPrice = p;
        temp.adjPrice = r;
        cities[a].push_back(temp);
    }

    // DFS
    vector<int> visit(cityNum + 1, 0);
    int result = DFS(cities, visit, 1, cityNum);
    if(result == -1)
        cout << "impossible" << endl;
    else
        cout <<  result << endl;

    return 0;
}

int DFS(const vector<vector<Edge> > &cities,
        vector<int> &visit,
        int node, int n ) {

    if (node == n)
        return 0;
    if (visit[node] >= 5)
        return -1;

    ++visit[node];

    int minSum = INT_MAX, sum, add;
    bool valid = false;

    for (unsigned i = 0; i < cities[node].size(); ++i) {
        if(visit[cities[node][i].cNode])
            sum = cities[node][i].cPrice;
        else
            sum = cities[node][i].adjPrice;

        if((add = DFS(cities, visit, cities[node][i].adjNode, n)) != -1){
            sum += add;
            valid = true;
            if(sum < minSum)
                minSum = sum;
        }
    }

    --visit[node];
    if(valid)
        return minSum;
    else
        return -1;
}
