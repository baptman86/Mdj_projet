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
