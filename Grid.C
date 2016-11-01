
#include "Grid.H"
#include <string>
// no changes in the public interface!

// data

// enum Tile { GROUND, WATER, BLOCKED };
    
// enum Direction { N, NE, E, SE, S, SW, W, NW };
    
// static const int CARDINAL_COST = 100; // Cost to move in cardinal direction
// static const int DIAGONAL_COST = 142; // Cost to move diagonally
// (>100*sqrt(2) => air distance is admissible)

// methods
bool Grid::canFloodFill(int size, int x1, int y1, int x2, int y2) const{
// Flood-fill (node, target-color, replacement-color):
//  1. Set Q to the empty queue.
//  2. If the color of node is not equal to target-color, return.
//  3. Add node to Q.
//  4. For each element N of Q:
//  5.         Set w and e equal to N.
//  6.         Move w to the west until the color of the node to the west of w no longer matches target-color.
//  7.         Move e to the east until the color of the node to the east of e no longer matches target-color.
//  8.         For each node n between w and e:
//  9.             Set the color of n to replacement-color.
// 10.             If the color of the node to the north of n is target-color, add that node to Q.
// 11.             If the color of the node to the south of n is target-color, add that node to Q.
// 12. Continue looping until Q is exhausted.
// 13. Return.
    std::vector<std::pair<int,int>> queue;
    
    Grid::Tile currentType = getTile(x1,y1);
    
    if(getTile(x1,y1) != getTile(x2, y2)){
        return false;
    }
    queue.push_back(std::make_pair(x1, y1));
    while(!queue.empty()){
        //Grab first element off stack
        std::pair<int, int> currentNode = queue.back();
        queue.pop_back();
        int x1 = currentNode.first; int y1 = currentNode.second;
        // Move tile west until a boundary is found
        int w = x1;
        while(getTile(w, y1) == currentType){
            w--;
        }
        //Move tile east until a boundary is found
        int e = x1;
        while(getTile(e, y1) == currentType){
            e++;
        }
        //Go through all nodes between east and west and add them to the queue
        for(int i=w; i<e; i++){
            //How do we set a node as visited?
            this->setTile(w, y1, BLOCKED);
            //Go north for that node
            this->canFloodFill(size, i, y1+1, x2, y2);
            
            //Go south for that node
            this->canFloodFill(size, i, y1-1, x2, y2);
        } 
    }
}


Grid::Grid(int width, int height){
    _width = width;
    _height = height;

    // Initialize all tiles to Blocked
    // It's set to blocked and not ground, for now atleast so that it ensures I
    // initialize it to something
    for(int i=0; i < _width; i++){
        tiles.push_back(std::vector<Tile>());
        for(int j=0; j < _height; j++){
            tiles[i].push_back(BLOCKED);
        }
    }
}

Grid::~Grid(){};
    
// Map properties
int Grid::getWidth() const{
    return _width;
};
int Grid::getHeight() const{
    return _height;
}

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
    std::string key = "";
    static std::map<std::string, bool> collisionCache;
    key += std::to_string(size) + std::to_string(x1) + std::to_string(y1) + std::to_string(x2) + std::to_string(y2);
    
    std::map<std::string, bool>::const_iterator iter = collisionCache.find(key);

    if(iter == collisionCache.end()){
        return collisionCache.find(key)->second;
    } else {
        bool canFF = this->canFloodFill(size, x1, y1, x2, y2);
        collisionCache.insert(std::pair<std::string, bool>(key, canFF));
        return canFF;
    };
};

// Compute a shortest path from (x1, y1) to (x2, y2) for an object with a
// given size using A*. Store the shortest path into path, and return the
// cost of that path (using CARDINAL_COST for the moves N, E, S, and W, and
// DIAGONAL_COST for the moves NE, SE, SW, and NW) or -1 if there is no
// path. Reduce initialization time by using the generation counter trick.
int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, 
                     std::vector<Direction> &path) const{
    return 0;
};
    
// sets tile type at location x y
void Grid::setTile(int x, int y, Tile tile){
    tiles[x][y] = tile;
};


