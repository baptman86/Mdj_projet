#include "mapgrid.h"
#include "float.h"
#include "character.h"


//Essentiellement un game state


MapGrid::MapGrid(unsigned int size,int randseed)
{
    this->size=size;
    for(int i=0;i<size;i++){
        this->data.push_back(vector<gridData> (size));
    }

    std::vector<std::vector<gridData> > fog(size,std::vector<gridData>(size));

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            fog[i][j]={-1,randomGround(),0};
        }
    }

    this->data=fog;
}

void MapGrid::enlight(int x, int y){
    if(x>=0 && y>=0 && x < size && y < size){
        data[x][y].Surbr=1;
    }
}

void MapGrid::unenlight(int x, int y){
    if(x>=0 && y>=0 && x < size && y < size){
        data[x][y].Surbr=0;
    }
}

void MapGrid::clear(){
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            data[i][j].Surbr=0;
        }
    }
}

vector<vector<gridData> > MapGrid::getData(){
    return this->data;
}

unsigned int MapGrid::getSize(){
    return this->size;
}


bool MapGrid::addObstacle(Object* o){
    return this->addObstacle(o,-1,-1);
}


bool MapGrid::addObstacle(Object* o, int x, int y){
    if(x>=0 && y>=0 && x < size && y < size){
        if(this->data[x][y].ObjId<0){
            int id = this->objects.size();
            this->objects.push_back(o);

            this->objects[id]->setCoord(x,y);
            this->data[x][y].ObjId=id;
            this->obstaclesId.push_back(id);
            return true;
        }
    }
    return false;
}

bool MapGrid::addCharacter(Object* o){
    return this->addCharacter(o,-1,-1);
}


bool MapGrid::addCharacter(Object* o, int x, int y){
    if(x>=0 && y>=0 && x < size && y < size){
        if(this->data[x][y].ObjId<0){
            int id = this->objects.size();
            this->objects.push_back(o);

            this->objects[id]->setCoord(x,y);
            this->data[x][y].ObjId=id;
            this->charactersId.push_back(id);
            return true;
        }
    }
    return false;
}

bool MapGrid::setObject(int id, int x, int y){
    if(x>=0 && y>=0 && x < size && y < size){
        if(this->data[x][y].ObjId<0){
            pair<int,int> objcoord = objects[id]->getCoord();
            if(objcoord.first != -1 && objcoord.second != -1 ){
                data[objcoord.first][objcoord.second].ObjId=-1;
            }
            this->objects[id]->setCoord(x,y);
            this->data[x][y].ObjId=id;
            return true;
        }
    }
    return false;
}

float* MapGrid::toWeights(){
    const int xSize = size;
    const int ySize = size;
    float* weigths = new float[xSize*ySize]();
    for(int i = 0; i < xSize;i++){
        for(int j = 0; j < ySize ; j++){

            if(this->data[i][j].ObjId!=0){
                weigths[j+ySize*i]=-1;
            } else if(this->data[i][j].GroundType==boue){
                weigths[j+ySize*i]=2;
            } else {
                weigths[j+ySize*i]=1;
            }
        }
    }
    return weigths;
}

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
    return lhs.fCost < rhs.fCost;
}

bool MapGrid::isValid(int x, int y, int id) { //If our Node is an obstacle it is not valid
    if (x < 0 || y < 0 || x >= size || y >= size) {
        return false;
    }
    else{
        return (data[x][y].ObjId < 0 || data[x][y].ObjId==id);
    }
}

bool isDestination(int x, int y, Node dest) {
    if (x == dest.x && y == dest.y) {
        return true;
    }
    return false;
}

double calculateH(int x, int y, Node dest) {
    double H = (sqrt((x - dest.x)*(x - dest.x)
        + (y - dest.y)*(y - dest.y)));
    return H;
}

vector<Node> MapGrid::aStar(Node player, Node dest, int id) {
   vector<Node> empty;
   if (isValid(dest.x, dest.y, id) == false) {
       cout << "Destination is an obstacle" << endl;
       return empty;
       //Destination is invalid
   }
   if (isDestination(player.x, player.y, dest)) {
       cout << "You are the destination" << endl;
       return empty;
       //You clicked on yourself
   }

   vector<vector<bool> > closedList(size,vector<bool>(size));

   //Initialize whole map
   //Node allMap[50][25];
   vector<vector<Node> > allMap(size,vector<Node>(size));
   for (int x = 0; x < size; x++) {
       for (int y = 0; y < size; y++) {
           allMap[x][y].fCost = FLT_MAX;
           allMap[x][y].gCost = FLT_MAX;
           allMap[x][y].hCost = FLT_MAX;
           allMap[x][y].parentX = -1;
           allMap[x][y].parentY = -1;
           allMap[x][y].x = x;
           allMap[x][y].y = y;

           closedList[x][y] = false;
       }
   }

   //Initialize our starting list
   int x = player.x;
   int y = player.y;
   allMap[x][y].fCost = 0.0;
   allMap[x][y].gCost = 0.0;
   allMap[x][y].hCost = 0.0;
   allMap[x][y].parentX = x;
   allMap[x][y].parentY = y;


   vector<Node> openList;
   openList.emplace_back(allMap[x][y]);
   bool destinationFound = false;


   while (!openList.empty()&&openList.size()<size*size) {
       Node node;
       do {
           //This do-while loop could be replaced with extracting the first
           //element from a set, but you'd have to make the openList a set.
           //To be completely honest, I don't remember the reason why I do
           //it with a vector, but for now it's still an option, although
           //not as good as a set performance wise.
           float temp = FLT_MAX;
           vector<Node>::iterator itNode;
           for (vector<Node>::iterator it = openList.begin();
               it != openList.end(); it = next(it)) {
               Node n = *it;
               if (n.fCost < temp) {
                   temp = n.fCost;
                   itNode = it;
               }
           }
           node = *itNode;
           openList.erase(itNode);
       } while (isValid(node.x, node.y, id) == false);


       if(node.gCost < FLT_MAX && node.gCost >((Character*)objects[id])->getMovement()){
           cout << "too far" << endl;
           return empty;
       }

       x = node.x;
       y = node.y;
       closedList[x][y] = true;


       //For each neighbour starting from North-West to South-East
       for (int newX = -1; newX <= 1; newX++) {
           for (int newY = -1; newY <= 1; newY++) {
               if(newX==0 || newY==0){
                   double gNew, hNew, fNew;
                   if (isValid(x + newX, y + newY, id)) {
                       if (isDestination(x + newX, y + newY, dest))
                       {
                           //Destination found - make path
                           allMap[x + newX][y + newY].parentX = x;
                           allMap[x + newX][y + newY].parentY = y;
                           destinationFound = true;
                           return makePath(allMap, dest);
                       }
                       else if (closedList[x + newX][y + newY] == false)
                       {
                           int cost = 1;
                           if(data[x+newX][y+newY].GroundType==boue){
                               cost++;
                           }
                           gNew = node.gCost + cost;
                           hNew = calculateH(x + newX, y + newY, dest);
                           fNew = gNew + hNew;
                           // Check if this path is better than the one already present
                           if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
                               allMap[x + newX][y + newY].fCost > fNew)
                           {
                               // Update the details of this neighbour node
                               allMap[x + newX][y + newY].fCost = fNew;
                               allMap[x + newX][y + newY].gCost = gNew;
                               allMap[x + newX][y + newY].hCost = hNew;
                               allMap[x + newX][y + newY].parentX = x;
                               allMap[x + newX][y + newY].parentY = y;
                               openList.emplace_back(allMap[x + newX][y + newY]);
                           }
                       }
                   }
               }
           }
        }
    }
    if (destinationFound == false) {
        cout << "Destination not found" << endl;
        return empty;
    }
}


vector<Node> MapGrid::makePath(vector<vector<Node> > map, Node dest) {
    try {
        cout << "Found a path" << endl;
        int x = dest.x;
        int y = dest.y;
        stack<Node> path;
        vector<Node> usablePath;

        while (!(map[x][y].parentX == x && map[x][y].parentY == y)
            && map[x][y].x != -1 && map[x][y].y != -1)
        {
            path.push(map[x][y]);
            int tempX = map[x][y].parentX;
            int tempY = map[x][y].parentY;
            x = tempX;
            y = tempY;

        }
        path.push(map[x][y]);

        while (!path.empty()) {
            Node top = path.top();
            path.pop();
            usablePath.emplace_back(top);
        }
        return usablePath;
    }
    catch(const exception& e){
        cout << e.what() << endl;
    }
}

bool MapGrid::isInLosAndRange( pair<int,int> begin, pair<int,int> end, int r ){
    return(isInLosAndRange(begin.first,begin.second,end.first,end.second,r));
}

bool MapGrid::isInLosAndRange( float x1, float y1, float x2, float y2, int r )
{
    //check range avec la distance de Manhattan
    if(fabs(x1-x2)+fabs(y1-y2) >r){
        return false;
    }

        // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);

  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int maxX = (int)x2;

  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        if(this->data[y][x].ObjId!=0){
            return false;
        }
        //SetPixel(y,x, color);
    }
    else
    {
        if(this->data[x][y].ObjId!=0){
            return false;
        }
        //SetPixel(x,y, color);
    }

    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }


  return true;
}


