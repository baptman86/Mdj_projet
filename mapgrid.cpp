#include "mapgrid.h"
#include "character.h"

MapGrid::MapGrid(unsigned int size)
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

bool MapGrid::isInLosAndRange( pair<int, int> begin, pair<int,int> end, int r ){
    return this->isInLosAndRange(begin.first, begin.second, end.first, end.second, r);
}

bool MapGrid::isInLosAndRange( int x1, int y1, int x2, int y2, int r )
{
    //check range avec la distance de Manhattan
    if(fabs(x1-x2)+fabs(y1-y2) >=r){
        return false;
    }

    return this->plotLine(x1,y1,x2,y2);

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

bool MapGrid::isValid(int x, int y, int id, int targetId) { //If our Node is an obstacle it is not valid
    if (x < 0 || y < 0 || x >= size || y >= size) {
        return false;
    }
    else{
        return (data[x][y].ObjId < 0 || data[x][y].ObjId==id || (targetId >=0 && data[x][y].ObjId==targetId));
    }
}

vector<Node> MapGrid::aStar(Node player, Node dest, int id, int targetId, bool usemouvement) {
   vector<Node> empty;
   if (isValid(dest.x, dest.y, id,targetId) == false) {
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
       } while (isValid(node.x, node.y, id, targetId) == false);


       if(usemouvement){
           if(node.gCost < FLT_MAX && node.gCost >((Character*)objects[id])->getMovement()){
               //cout << "too far" << endl;
               return empty;
           }
       }

       x = node.x;
       y = node.y;
       closedList[x][y] = true;


       //For each neighbour starting from North-West to South-East
       for (int newX = -1; newX <= 1; newX++) {
           for (int newY = -1; newY <= 1; newY++) {
               if(newX==0 || newY==0){
                   double gNew, hNew, fNew;
                   if (isValid(x + newX, y + newY, id,targetId)) {
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
        //cout << "Destination not found" << endl;
        return empty;
    }
}


vector<Node> MapGrid::makePath(vector<vector<Node> > map, Node dest) {
    try {
        //cout << "Found a path" << endl;
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

bool MapGrid::plotLineLow(int x0,int y0,int x1,int y1){
    cout << "On entre dans low";
  int dx = x1 - x0;
  int dy = y1 - y0;
  int yi = 1;
  if (dy < 0){
    yi = -1;
    dy = -dy;
  }
  int D = 2*dy - dx;
  int y = y0;

  for(int x = x0; x < x1 ; x++){
      this->enlight(x,y);
    if(this->data[x][y].ObjId>=0 && (!(x0==x && y0 ==y))&& (!(x1==x && y1==y))){
        return false;

    }
    if (D > 0){
       y = y + yi;
       D = D - 2*dx;
    }

   D = D + 2*dy;

  }
  return true;
}

bool MapGrid::plotLineHigh(int x0,int y0,int x1,int y1){

  int dx = x1 - x0;


  int dy = y1 - y0;



  int xi = 1;
  if (dx < 0){
    xi = -1;
    dx = -dx;
  }
  int D = 2*dx - dy;

  int x = x0;


  for(int y = y0; y < y1 ; y++){


//      this->enlight(x,y);
    if(this->data[x][y].ObjId>=0 && (!(x0==x && y0 ==y)) && (!(x1==x && y1==y))){
        return false;

    }

    if (D > 0){
       x = x + xi;
       D = D - 2*dy;
    }
    D = D + 2*dx;

  }
  return true;
}

bool MapGrid::plotLine(int x0,int y0,int x1,int y1){
    if (abs(y1 - y0) < abs(x1 - x0)){
        if (x0 > x1) {
          return plotLineLow(x1, y1, x0, y0);
        }
        else {
          return plotLineLow(x0, y0, x1, y1);
        }
    }
      else {
        if (y0 > y1) {
          return plotLineHigh(x1, y1, x0, y0);
        }
        else {
          return plotLineHigh(x0, y0, x1, y1);
        }
     }
}
