#include "objloader.h"

ObjLoader::ObjLoader()
{

}

bool ObjLoader::loadOBJ(const char * path, vector < QVector3D > & out_vertices, vector < QVector2D > & out_uvs, vector < QVector3D > & out_normals){
    vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    vector< QVector3D > temp_vertices;
    vector< QVector2D > temp_uvs;
    vector< QVector3D > temp_normals;
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }
    while( 1 ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            float x;
            float y;
            float z;
            fscanf(file, "%f %f %f\n", &x, &y, &z );
            QVector3D vertex = QVector3D(x,y,z);
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            float x;
            float y;
            fscanf(file, "%f %f\n", &x, &y );
            QVector2D uv = QVector2D(x,y);
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            float x;
            float y;
            float z;
            fscanf(file, "%f %f %f\n", &x, &y, &z );
            QVector3D normal = QVector3D(x,y,z);
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    // For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        unsigned int vertexIndex = vertexIndices[i];
        QVector3D vertex = temp_vertices[ vertexIndex-1 ];
        out_vertices.push_back(vertex);
    }

    for( unsigned int i=0; i<uvIndices.size(); i++ ){
        unsigned int uvIndex = uvIndices[i];
        QVector2D uv = temp_uvs[ uvIndex-1 ];
        out_uvs.push_back(uv);
    }

    for( unsigned int i=0; i<normalIndices.size(); i++ ){
        unsigned int normalIndex = normalIndices[i];
        QVector3D normal = temp_normals[ normalIndex-1 ];
        out_normals.push_back(normal);
    }
    return true;
}
