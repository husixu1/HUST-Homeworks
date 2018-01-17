#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<algorithm>
#include<climits>

using namespace std;
typedef pair<int, int> Stick;

bool compareF(const Stick &a, const Stick &b){return a.first < b.first;}
bool compareS(const Stick &a, const Stick &b){return a.second < b.second;}

int main(){
    unsigned caseNum, stickNum;
    cin >> caseNum;

    while(caseNum--){
        cin >> stickNum;
        vector<Stick> sticks;
        int result = 0;
        vector<int> curMaxList;

        int length, weight;

        // read inputs
        while(stickNum--){
            cin >> length >> weight;
            sticks.push_back(pair<int, int>(length, weight));
        }

        stable_sort(sticks.begin(), sticks.end(), &compareS);
        stable_sort(sticks.begin(), sticks.end(), &compareF);

        for(unsigned i=0; i < sticks.size(); ++i){
            unsigned j=0;
            for(j=0; j < curMaxList.size(); ++j)
                if(sticks[i].second >= curMaxList[j]){
                    curMaxList[j] = sticks[i].second;
                    break;
                }
            if(j == curMaxList.size()){
                curMaxList.push_back(sticks[i].second);
                ++result;
            }
        }
        cout << result << endl;
    }
    return 0;
}
