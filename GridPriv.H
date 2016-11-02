// list private Grid members here
int _width;
int _height;
// Counter for amount of unique groups
mutable int _tileGroup;
// Cache of all spaces travelled to on the map
mutable std::map<std::string, int> travelCache;

// Vector of vector of tiles
std::vector<std::vector<Grid::Tile> > tiles;

void floodFillInit(int size, int x1, int y1) const;

void floodFill(const int size,const int x1,const int y1,const int group,
               const Tile tile) const;

// Calculate distance from one point to another
double pythDistance(int x1, int y1, int x2, int y2) const;

// Generate a vector of direction givens a current node and traces back through a
// set of nodes to a starting node
std::vector<Direction> reconstructPath(std::map<std::pair<int, int>, 
                                       std::pair<int, int> >, std::pair<int, int> ) const;

// Get the direction to go from current node to next node
Direction getDirection(const std::pair<int, int> current, const std::pair<int, int> next) const;

// Calculate the cost of a direction vector
int calculateCost(const std::vector<Direction>) const;

// Determine if shape can fit on the tiles
bool canFit(int size, int x, int y) const;
