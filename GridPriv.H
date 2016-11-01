// list private Grid members here
#include <string>
#include <map>
#include <vector>
int _width;
int _height;
std::vector<std::vector<Tile> > tiles;
bool canFloodFill(int size, int x1, int y1, int x2, int y2) const;
double rectDistance(int x1, int y1, int x2, int y2) const;
std::vector<Direction> reconstructPath(std::map<std::pair<int, int>, std::pair<int, int> >, std::pair<int, int> ) const;
Direction getDirection(const std::pair<int, int> current, const std::pair<int, int> next) const;
int calculateCost(const std::vector<Direction>) const;
