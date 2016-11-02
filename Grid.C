
#include "Grid.H"
////////////////////////////////////////////////////////////////////////////////
// Aliases
///////////////////////////////////////////////////////////////////////////////
using Position = std::pair<int, int>;
using fScore = std::pair< Position, double>;

///////////////////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////////////////

// A helper function for a priority queue
struct fScoreCompare {
    bool operator()(const fScore& l, const fScore& r)  
    {  
        return l.second > r.second;  
    } 
};

// A generator to create all neighbour positions at request time
struct GenNeighbour{
    GenNeighbour(const Position &pos){
        storedPos = Position(pos.first, pos.second);
        
        // Neighbour (+1, +0)
        lastGenerated = Position(pos.first, pos.second);
    }

    Position next(){
        if(lastGenerated.first == storedPos.first && lastGenerated.second == storedPos.second){
            lastGenerated.first = storedPos.first + 1;
            return lastGenerated;
        }else if(lastGenerated.first == storedPos.first+1 && lastGenerated.second == storedPos.second){
            // Neighbour (+1, 0) -> Neighbour(+1, +1)
            lastGenerated.second += 1;
            return lastGenerated;
        } else if(lastGenerated.first == storedPos.first+1 && lastGenerated.second == storedPos.second + 1){
            // Neighbour (+1, +1) -> Neighbour(+1, -1)
            lastGenerated.second = storedPos.second -1;
            return lastGenerated;
        } else if(lastGenerated.first == storedPos.first+1 && lastGenerated.second == storedPos.second - 1){
            // Neighbour (+1, -1) -> Neighbour(0, -1)
             lastGenerated.first = storedPos.first;
             return lastGenerated;
        } else if(lastGenerated.first == storedPos.first && lastGenerated.second == storedPos.second - 1){
            // Neighbour (0, -1) -> Neighbour(0, +1)
             lastGenerated.second = storedPos.second + 1;
             return lastGenerated;
        } else if(lastGenerated.first == storedPos.first && lastGenerated.second == storedPos.second + 1){
            // Neighbour (0, +1) -> Neighbour(-1, +1)
             lastGenerated.first = storedPos.first - 1;
             return lastGenerated;
        } else if(lastGenerated.first == storedPos.first-1 && lastGenerated.second == storedPos.second + 1){
            // Neighbour (-1, +1) -> Neighbour(-1, 0)
             lastGenerated.second = storedPos.second;
             return lastGenerated;
        }else if(lastGenerated.first == storedPos.first-1 && lastGenerated.second == storedPos.second){
            // Neighbour (-1, 0) -> Neighbour(-1, -1)
             lastGenerated.second = storedPos.second -1;
             return lastGenerated;
        } else {
            throw "Can't generate anymore neighbours";
        }
    }
private:
    Position lastGenerated;
    Position storedPos;
};

std::string makeKey(int size, int x, int y){
    std::string key = std::to_string(size) + "x" + std::to_string(x) + "y" + std::to_string(y);
    return key;
}

///////////////////////////////////////////////////////////////////////////////
// Class implementation
///////////////////////////////////////////////////////////////////////////////

Grid::Grid(int width, int height){
    _width = width;
    _height = height;
    _tileGroup = 0;

    // Initialize all tiles to Blocked
    // It's set to blocked and not ground, for now atleast so that it ensures I
    // // initialize it to something
    for(int i=0; i < _width; i++){
        tiles.push_back(std::vector<Tile>());
        for(int j=0; j < _height; j++){
            tiles[i].push_back(BLOCKED);
        }
    }
}

Grid::~Grid(){
    // Don't need to clean up, using standard C++ structs
};

// Map properties
int Grid::getWidth() const{
    return _width;
};
int Grid::getHeight() const{
    return _height;
}

// sets tile type at location x y
void Grid::setTile(int x, int y, Tile tile){
    tiles[x][y] = tile;
};
Grid::Tile Grid::getTile(int x, int y) const{
  return tiles[x][y];  
};

// Pathfinding operations

// Return true iff object with a given size can reside on (x1, y1)
// and move from there to (x2, y2) while staying at the same tile
// type without colliding. 
//
// This should execute faster than calling findShortestPath().
// 
// Also, if the map hasn't changed, subsequent calls with the same
// x1,y1,x2,y2 coordinates SHOULD BE MUCH FASTER. Hint: flood fill + caching
bool Grid::isConnected(int size, int x1, int y1, int x2, int y2)const{
    // Build the key
    std::string key1 = makeKey(size, x1, y1);
    
    std::string key2 = makeKey(size, x2, y2);
    
    std::map<std::string, int>::const_iterator key1Iter = travelCache.find(key1);
    if(key1Iter == travelCache.end()){
        floodFillInit(size, x1, y1);
    }
    
    key1Iter = travelCache.find(key1);
    std::map<std::string, int>::const_iterator key2Iter = travelCache.find(key2);
    
    if(key1Iter != travelCache.end() && key2Iter != travelCache.end()){
        return key1Iter->second == key2Iter->second;
    } else{
        return false;
    }
};


void Grid::floodFillInit(int size, int x, int y) const{
    // Generate key for map
    std::string key = makeKey(size, x, y);
    std::map<std::string, int>::const_iterator groupIter = travelCache.find(key);   
  
    if(x < 0 || x >= this->_width){
        // Bounds check for x
        return;
    } else if(y < 0 || y >= this->_height){
        // Bounds check for y
        return;
    } else if(groupIter != travelCache.end()){
        return;
    } else if(!canFit(size, x, y)){
        return;
    }else {
        _tileGroup += 1;
        int newGroup =  _tileGroup;
        Tile tile = getTile(x, y);
        floodFill(size, x, y, newGroup, tile);
    }
}

void Grid::floodFill(const int size, const int x, const int y, const int group, 
                     const Tile tile) const{
    if(x < 0 || x >= this->_width){
        // Bounds check for x
        return;
    } else if(y < 0 || y >= this->_height){
        // Bounds check for y
        return;
    } else if(!canFit(size, x, y)){
        return;
    }

    std::string key = makeKey(size, x, y);
    std::map<std::string, int>::const_iterator groupIter = travelCache.find(key);

    if(groupIter != travelCache.end()){
        return;
    } else if(getTile(x, y) != tile){
        return;
    } else{
        travelCache[key] = group;
        // North
        floodFill(size, x, y+1, group, tile);
        // South 
        floodFill(size, x, y-1, group, tile);
        // East
        floodFill(size, x+1, y, group, tile);
        // West
        floodFill(size, x-1, y, group, tile);
    }

}    

// Compute a shortest path from (x1, y1) to (x2, y2) for an object with a
// given size using A*. Store the shortest path into path, and return the
// cost of that path (using CARDINAL_COST for the moves N, E, S, and W, and
// DIAGONAL_COST for the moves NE, SE, SW, and NW) or -1 if there is no
// path. Reduce initialization time by using the generation counter trick.
int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, 
                     std::vector<Direction> &path) const{
// The implementation of this A* was using the algorithm outlined on Wiki
// https://en.wikipedia.org/wiki/A*_search_algorithm#psuedocode
    int totalCost = -1;
    // Starting node
    Position key = Position(x1, y1);
    
    // Set of all visited nodes
    std::set< Position > visited;
    // std::set< Position > unvisited;
    // Collection of nodes to visit
    std::priority_queue< fScore, std::vector<fScore>, fScoreCompare> fScoreQueue;
    fScoreQueue.push( fScore(key, this->pythDistance(x1, y1, x2, y2)));
    //cameFrom
    std::map< Position, Position > cameFrom;
    // map of cost to go from start node to key node
    std::map< Position, double> gScore;
    gScore[key] =  0;
    
    while(!fScoreQueue.empty()){
        fScore topNode = fScoreQueue.top();

        Position pos = topNode.first;
        if(pos.first == x2 && pos.second == y2){
            // If we find our current Node is the Goal node return
            std::vector<Direction> _path = reconstructPath(cameFrom, pos);
            path = _path;
            totalCost = calculateCost(path);

            return totalCost;
        }
        fScoreQueue.pop();
        visited.insert(pos);
        //Generate all neighbours
        GenNeighbour generator(pos);
        for(int i=0; i< 8; i++){
            Position neighbour = generator.next();
            // If we've all ready visited this neighbour, skip it
            if(visited.count(neighbour) > 0){
                continue;
            }

            // Make sure my position and my neighbours positions are within bounds
            if(pos.first<0 || pos.first>= this->_width){
                break;
            }
            if(pos.second<0 || pos.second>= this->_height){
                break;
            }
            if(neighbour.first<0 || neighbour.first>= this->_width){
                break;
            }
            if(neighbour.second<0 || neighbour.second>= this->_height){
                break;
            }

            // If the two tile are not the same skip
            if(getTile(neighbour.first, neighbour.second) != getTile(pos.first, pos.second)){
                continue;
            }

            
            double neighbourGScore = gScore[pos] + 
                this->pythDistance(pos.first, pos.second, neighbour.first, neighbour.second );
           
            if(gScore.count(neighbour) == 0){
                // If we haven't encountered this before don't then neighbourGscore
                // is currently greatest
            } else if(neighbourGScore >= gScore[neighbour]){
                // This is not the best path so go to next name
                continue;
            }
            if(!canFit(size, neighbour.first, neighbour.second)){
                // shape can't fit in neighbour so go to next neighbour
                continue;
            }
            // We made it through a gauntlet of ifs...
            // Record this new best path
            cameFrom[neighbour] = pos;
            gScore[neighbour] = neighbourGScore;
            fScoreQueue.push(fScore(neighbour, neighbourGScore + 
                                    this->pythDistance(neighbour.first, 
                                                       neighbour.second, x2, y2)));
        }
    }
    return totalCost;
};
    

double Grid::pythDistance(int x1, int y1, int x2, int y2) const{
    return sqrt((pow((x1 - x2),2 ) + pow((y1 - y2), 2)));
}

std::vector<Grid::Direction> Grid::reconstructPath(std::map< Position, Position > 
                                                   cameFrom, Position current) const{
    
    std::vector<Grid::Direction> path;
    
    std::vector< Position > nodeTransitions;
    // Check to make sure cameFrom is bigger than zero
    if(cameFrom.size() == 0){
        return path;
    }
    // I was encountering problems generating directions otherwise
    // so I split off grabbing the correct node transitions from goal to start
    // and the directions we need to return
    nodeTransitions.push_back(current);
    while(cameFrom.count(current) > 0){
        current = cameFrom[current];
        nodeTransitions.push_back(current);
    }

    // Generate allow directions from start to goal.
    std::vector< Position >::reverse_iterator rBegin = nodeTransitions.rbegin(); 
    std::vector< Position >::reverse_iterator rEnd = nodeTransitions.rend();
    for(; rBegin+1 != rEnd; rBegin++){
        path.push_back( getDirection(*rBegin, *(rBegin+1)));
    }
    // Return path
    return path;
}

Grid::Direction Grid::getDirection(const Position current,const Position next) const{
    // Function to hide the messiness of determining direction
    if(next.first == current.first+1 && next.second == current.second){
        return E; 
    } else if(next.first == current.first+1 && next.second == current.second + 1){
        return SE;
    } else if(next.first == current.first+1 && next.second == current.second - 1){
        return NE;
    } else if(next.first == current.first && next.second == current.second - 1){
        return N;
    } else if(next.first == current.first && next.second == current.second + 1){
        return S;
    } else if(next.first == current.first-1 && next.second == current.second + 1){
        return SW;
    }else if(next.first == current.first-1 && next.second == current.second-1){
        return NW;
    }else if(next.first == current.first-1 && next.second == current.second){
        return W;   
    } else {
        throw "Can't find direction";
    }
}

int Grid::calculateCost(const std::vector<Direction> path) const{
    int cost = 0;
    for(int a: path){
        // For all directions in path, if the path is cardinal add cardinal cost
        // else if direction is diagonal add diagonal cost
        // else throw error
        if(a == N || a == S || a == W || a == E){
            cost += CARDINAL_COST;
        } else if( a == NE || a == NW || a == SE || a == SW){
            cost += DIAGONAL_COST;
        } else {
            throw "Direction not found";
        }
    }
    return cost;
}

bool Grid::canFit(int size, int x, int y) const{
    Tile mainTile = getTile(x,y);
    // make sure the entire shape is within bounds
    if(x+size>= this->_width || x<0){
        return false;
    }  
    if(y+size>= this->_height || y<0){
        return false;
    }
    
    // Check to make sure each tile is the same as x+0, y+0
    for(int i=0; i<= size; i++){
        for(int j=0; j <= size; j++){
            if(getTile(x+i, y+j) != mainTile){
                // If they aren't the same tile return false
                return false;
            } 
        }
    }
    // If we make it through the for loop return true
    return true;
}
