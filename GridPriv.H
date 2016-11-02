// list private Grid members here
#include <string>
#include <map>
#include <vector>
int _width;
int _height;
mutable int _tileGroup;
mutable std::map<std::string, int> travelCache;
std::vector<std::vector<Grid::Tile> > tiles;
void floodFillInit(int size, int x1, int y1) const;
void floodFill(const int size,const int x1,const int y1,const int group,const Tile tile) const;
double rectDistance(int x1, int y1, int x2, int y2) const;
std::vector<Direction> reconstructPath(std::map<std::pair<int, int>, std::pair<int, int> >, std::pair<int, int> ) const;
Direction getDirection(const std::pair<int, int> current, const std::pair<int, int> next) const;
int calculateCost(const std::vector<Direction>) const;
bool canFit(int size, int x, int y) const;
