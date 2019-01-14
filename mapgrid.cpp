#include "mapgrid.h"

MapGrid::MapGrid(unsigned int size)
{
    this->size=size;
    for(int i=0;i<size;i++){
        this->data.push_back(vector<gridData> (size));
    }
}

vector<vector<gridData> > MapGrid::getData(){
    return this->data;
}

unsigned int MapGrid::getSize(){
    return this->size;
}


bool MapGrid::isObjectById(int id){
    for(pair<int, Object> pair : this->objects){
        if(pair.first=id){
            return true;
        }
    }
    return false;
}

Object MapGrid::findObjectById(int id){
    for(pair<int, Object> pair : this->objects){
        if(pair.first=id){
            return pair.second;
        }
    }
}


//id = 0 <=> pas d'objet, donc premier objet : id = 1, dans objects[0]
bool MapGrid::addObject(Object o){
    return this->addObject(o,-1,-1);
}


bool MapGrid::addObject(Object o, int x, int y){
    int id = this->data.size();
    this->objects.push_back(make_pair(id,o));

    return this->setObject(id,x,y);
}

bool MapGrid::setObject(int id, int x, int y){


    this->objects[id-1].second.setCoord(x,y);
    if(x >= 0 && y >= 0) {
        this->data[x][y].ObjId=id;
    }
    return true;
    //    for(pair<int, Object> pair : this->objects){
    //        if(pair.first=id){
    //            this->data[x][y].ObjId=id;
    //            pair.second.setCoord(x,y);
    //            return true;
    //        }
    //    }
}

//Convertit la grille en nodes exploitables par astar. Détermine les coûts.
//vector<Node> MapGrid::toNodes(){
//    const int xSize = this->data.size();
//    const int ySize = this->data[0].size();
//    vector<Node> nodes;
//    for(int i = 0; i < xSize;i++){
//        for(int j = 0; j < ySize ; j++){

//            if(this->data[i][j].ObjId!=0){
//                nodes.push_back(Node(j+ySize*i,-1));
//            } else if(this->data[i][j].GroundType==boue){
//                nodes.push_back(Node(j+ySize*i, 2));
//            }
//            nodes.push_back(Node(j+ySize*i, 1));
//        }
//    }
//}

float* MapGrid::toWeights(){
    const int xSize = this->data.size();
    const int ySize = this->data[0].size();
    float weigths[xSize*ySize];
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

vector<gridData> MapGrid::findPath(int xStart,int yStart,int xEnd,int yEnd){
    vector<pair<int,int>> paires;
    const int xSize = this->data.size();
    const int ySize = this->data[0].size();

    if(!astar(this->toWeights(),xSize,ySize,xSize*xStart+yStart,xEnd*xSize+yEnd,paires)){
        //Problème
    }

    vector<gridData> path;
    for(pair<int,int> p : paires){
        path.push_back(this->data[p.first][p.second]);
    }
    return path;
}

bool MapGrid:: isInLosAndRange( float x1, float y1, float x2, float y2, int r )
{
    //check range avec la distance de Manhattan
    if(fabs(x1-x2)+fabs(y1-y2) >=r){
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
