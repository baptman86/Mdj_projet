#include "mapgrid.h"
#include "character.h"

MapGrid::MapGrid(unsigned int size,int randseed)
{
    this->size=size;
    for(int i=0;i<size;i++){
        this->data.push_back(vector<gridData> (size));
    }

    std::vector<std::vector<gridData> > fog(size,std::vector<gridData>(size));

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            fog[i][j]={0,randomGround(),SurbrType::rien};
        }
    }

    this->data=fog;
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
        if(this->data[x][y].ObjId<=0){
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
        if(this->data[x][y].ObjId<=0){
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
        if(this->data[x][y].ObjId<=0){

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

vector<gridData> MapGrid::findPath(int xStart,int yStart,int xEnd,int yEnd){
    vector<pair<int,int>> paires;
    const int xSize = this->data.size();
    const int ySize = this->data[0].size();

    if(!Node::astar(this->toWeights(),xSize,ySize,xSize*xStart+yStart,xEnd*xSize+yEnd,paires)){
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
