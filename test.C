#include <utility>
#include <queue>
#include <iostream>
using namespace std;
using Pair = std::pair< std::pair<int, int>, double>;
struct compare {
    bool operator()(const std::pair< std::pair<int, int>, double>& l, const std::pair<std::pair<int, int>, double>& r)  
    {  
        return l.second > r.second;  
    }
};

int main(){
    Pair first({1,1}, 12.0);
    Pair second({1,2}, 13.0);
    Pair third({1,3}, 11.0);

    std::priority_queue<Pair, std::vector<Pair>, compare> fScoreQueue;
    
    fScoreQueue.push(first);
    fScoreQueue.push(second);
    fScoreQueue.push(third);
    
    while(!fScoreQueue.empty()){
        cout << fScoreQueue.top().first.second << endl;
        fScoreQueue.pop();
    }
}
