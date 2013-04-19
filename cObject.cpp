//
//  cObject.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <assert.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "rendModel.h"
#include "cObject.h"

/* 
 Takes the cObject object and turns it in to a returned rendModel 
 to be rendered.
*/
rendModel cObject::addToRender()
{
    return rendModel (this);
}

/*
 Private function to create an identity triangle centered at (0.0, 0.0, 0.0) 
 */
void cObject::makeCTRIANGLE()
{
    vertices_ = new point3[3];
    vertices_count_ = 3;
    vertex_normals_ = new vec3[1];
    
    triangles_ = new objectTriangle[1];
    triangle_count_ = 1;
    
    vertices_[0] = point3(0.0, 0.5, 0.0);
    vertices_[1] = point3(-0.5, -0.5, 0.0);
    vertices_[2] = point3(0.5, -0.5, 0.0);
    
    objectTriangle triangle;
    triangle.vertices[0] = 0;
    triangle.vertices[1] = 1;
    triangle.vertices[2] = 2;
    
    triangle.vertex_normals = false;
    
    triangles_[0] = triangle;
}

void cObject::makeCBOX()
{
    triangles_ = new objectTriangle[12];
    triangle_count_ = 12;
    
    vertices_ = new point3[36];
    vertices_count_ = 36;
    vertex_normals_ = new vec3[6];
    
    vertices_[0] = point3(-0.5, 0.5, 0.5);
    vertices_[1] = point3(-0.5, -0.5, 0.5);
    vertices_[2] = point3(0.5, 0.5, 0.5);
    
    vertices_[3] = point3(0.5, 0.5, 0.5);
    vertices_[4] = point3(-0.5, -0.5, 0.5);
    vertices_[5] = point3(0.5, -0.5, 0.5);
    
    vertices_[6] = point3(0.5, 0.5, -0.5);
    vertices_[7] = point3(-0.5, -0.5, -0.5);
    vertices_[8] = point3(-0.5, 0.5, -0.5);
    
    vertices_[9] = point3(0.5, 0.5, -0.5);
    vertices_[10] = point3(0.5, -0.5, -0.5);
    vertices_[11] = point3(-0.5, -0.5, -0.5);
    
    vertices_[12] = point3(-0.5, -0.5, 0.5);
    vertices_[13] = point3(-0.5, -0.5, -0.5);
    vertices_[14] = point3(0.5, -0.5, -0.5);
    
    vertices_[15] = point3(0.5, -0.5, -0.5);
    vertices_[16] = point3(-0.5, -0.5, 0.5);
    vertices_[17] = point3(0.5, -0.5, 0.5);
    
    vertices_[18] = point3(-0.5, 0.5, 0.5);
    vertices_[19] = point3(-0.5, 0.5, -0.5);
    vertices_[20] = point3(0.5, 0.5, -0.5);
    
    vertices_[21] = point3(0.5, 0.5, -0.5);
    vertices_[22] = point3(-0.5, 0.5, 0.5);
    vertices_[23] = point3(0.5, 0.5, 0.5);
    
    vertices_[24] = point3(-0.5, -0.5, -0.5);
    vertices_[25] = point3(-0.5, 0.5, -0.5);
    vertices_[26] = point3(-0.5, 0.5, 0.5);
    
    vertices_[27] = point3(-0.5, -0.5, -0.5);
    vertices_[28] = point3(-0.5, 0.5, 0.5);
    vertices_[29] = point3(-0.5, -0.5, 0.5);
    
    vertices_[30] = point3(0.5, -0.5, -0.5);
    vertices_[31] = point3(0.5, 0.5, -0.5);
    vertices_[32] = point3(0.5, 0.5, 0.5);
    
    vertices_[33] = point3(0.5, -0.5, -0.5);
    vertices_[34] = point3(0.5, 0.5, 0.5);
    vertices_[35] = point3(0.5, -0.5, 0.5);
    
    //front side
    objectTriangle triangle1;
    triangle1.vertices[0] = 0; //top left
    triangle1.vertices[1] = 1; //bottom left
    triangle1.vertices[2] = 2; //top right
    
    triangle1.vertex_normals = false;
    
    objectTriangle triangle2;
    triangle2.vertices[0] = 3; //top right
    triangle2.vertices[1] = 4; //bottom left
    triangle2.vertices[2] = 5; //bottom right
    
    triangle2.vertex_normals = false;
    
    //back side
    objectTriangle triangle3;
    triangle3.vertices[0] = 6; //top left
    triangle3.vertices[1] = 7; //bottom left
    triangle3.vertices[2] = 8; //top right
    
    triangle3.vertex_normals = false;
    
    objectTriangle triangle4;
    triangle4.vertices[0] = 9; //top right
    triangle4.vertices[1] = 10; //bottom left
    triangle4.vertices[2] = 11; //bottom right
    
    triangle4.vertex_normals = false;
    
    //bottom side
    objectTriangle triangle5;
    triangle5.vertices[0] = 12; //close left
    triangle5.vertices[1] = 13; //far left
    triangle5.vertices[2] = 14; //far right
    
    triangle5.vertex_normals = false;
    
    objectTriangle triangle6;
    triangle6.vertices[0] = 15; //far right
    triangle6.vertices[1] = 16; //close left
    triangle6.vertices[2] = 17; //close right
    
    triangle6.vertex_normals = false;
    
    //top side
    objectTriangle triangle7;
    triangle7.vertices[0] = 18; //close left
    triangle7.vertices[1] = 19; //far left
    triangle7.vertices[2] = 20; //far right
    
    triangle7.vertex_normals = false;
    
    objectTriangle triangle8;
    triangle8.vertices[0] = 21; //far right
    triangle8.vertices[1] = 22; //close left
    triangle8.vertices[2] = 23; //close right
    
    triangle8.vertex_normals = false;
    
    //left side
    objectTriangle triangle9;
    triangle9.vertices[0] = 24; //far bottom
    triangle9.vertices[1] = 25; //far top
    triangle9.vertices[2] = 26; //close top
    
    triangle9.vertex_normals = false;
    
    objectTriangle triangle10;
    triangle10.vertices[0] = 27; //far bottom
    triangle10.vertices[1] = 28; //close top
    triangle10.vertices[2] = 29; //close bottom
    
    triangle10.vertex_normals = false;
    
    //right side
    objectTriangle triangle11;
    triangle11.vertices[0] = 30; //far bottom
    triangle11.vertices[1] = 31; //far top
    triangle11.vertices[2] = 32; //close top
    
    triangle11.vertex_normals = false;
    
    objectTriangle triangle12;
    triangle12.vertices[0] = 33; //far bottom
    triangle12.vertices[1] = 34; //close top
    triangle12.vertices[2] = 35; //close bottom
    
    triangle12.vertex_normals = false;
    
    triangles_[0] = triangle1;
    triangles_[1] = triangle2;
    triangles_[2] = triangle3;
    triangles_[3] = triangle4;
    triangles_[4] = triangle5;
    triangles_[5] = triangle6;
    triangles_[6] = triangle7;
    triangles_[7] = triangle8;
    triangles_[8] = triangle9;
    triangles_[9] = triangle10;
    triangles_[10] = triangle11;
    triangles_[11] = triangle12;
}

void cObject::makeCTESTOBJECT()
{
    triangles_ = new objectTriangle[3];
	triangle_count_ = 3;
    
    vertices_ = new point3[9];
    vertices_count_ = 9;
    vertex_normals_ = NULL;
	
    //left
	objectTriangle triangle1;
	vertices_[1] = point3(-0.1, 0.1, -1.3);
	vertices_[2] = point3(0.0, 0.1, -1.3);
	vertices_[0] = point3(-0.1, -0.1, -1.3);
    
    triangle1.vertices[1] = 0;
	triangle1.vertices[2] = 1;
	triangle1.vertices[0] = 2;
	
    triangle1.vertex_normals = false;
    
	triangles_[0] = triangle1;
	
	//right top
	objectTriangle triangle2;
	vertices_[3] = point3(-0.2, 0.1, -1.55);
	vertices_[5] = point3(0.3, 0.1, -1.50);
	vertices_[4] = point3(0.2, -0.1, -1.53);
    
    triangle2.vertices[0] = 3;
	triangle2.vertices[2] = 4;
	triangle2.vertices[1] = 5;
	
    triangle2.vertex_normals = false;
    
	triangles_[1] = triangle2;
    
    //right bottom
    objectTriangle triangle3;
	vertices_[6] = point3(-0.1, -0.15, -1.55);
	vertices_[8] = point3(0.4, -0.1, -1.50);
	vertices_[7] = point3(0.3, -0.3, -1.0);
    
    triangle3.vertices[0] = 6;
	triangle3.vertices[2] = 7;
	triangle3.vertices[1] =  8;
    
    triangle3.vertex_normals = false;
	
	triangles_[2] = triangle3;
    

}

/*
 Create an object in the scene from the provided objects.
 */
cObject::cObject(int option)
{
    translate_vector_ = vec3 (0.0, 0.0, 0.0);
    rotate_vector_ = vec3(0.0, 0.0, 0.0);
    scale_vector_ = vec3(1.0, 1.0, 1.0);
    
    switch (option)
    {
        case CTRIANGLE:
            makeCTRIANGLE();
            break;
        case CBOX:
            makeCBOX();
            break;
        case CTESTOBJECT:
            makeCTESTOBJECT();
            break;
    }
}

/*
 Load object from filename.
 */
cObject::cObject(const char *filename)
{
    
    translate_vector_ = vec3 (0.0, 0.0, 0.0);
    rotate_vector_ = vec3(0.0, 0.0, 0.0);
    scale_vector_ = vec3(1.0, 1.0, 1.0);
    
    printf("Loading object from file [assimp]\n");

    Assimp::Importer importer;
    
    const aiScene* p_scene = importer.ReadFile(filename, aiProcess_Triangulate |aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (p_scene)
    {
        /* These three vectors are for holding all the vertices in the scene from the file 
         together in one place. Since the scene can be split up in to a number of meshes 
         in the file, we can't really easily tell how many vertices are in the scene in 
         total, and cObject doesn't have support for multiple meshes in one cObject nor will 
         it probably ever.         
         */
        std::vector<objectTriangle> temp_triangles;
        
        std::vector<point3> temp_vertices_vector;
        std::vector<point3> temp_vertex_normal_vector;
        
        for (unsigned int i=0; i < p_scene->mNumMeshes; i++)
        {
            const aiMesh* current_mesh = p_scene->mMeshes[i];
            
            /*  we need this so we can keep the references to indices from the file
            aligning to the same vertices in our array of vertices, so we just take
            the number of vertices already in our (future) list and add this offset
            later on to keep it all aligned.
             */
            unsigned int vertex_offset = temp_vertices_vector.size();
            
            for (unsigned int j=0; j < current_mesh->mNumVertices; j++)
            {
                temp_vertices_vector.push_back( vec3(current_mesh->mVertices[j].x,
                                                     current_mesh->mVertices[j].y,
                                                     current_mesh->mVertices[j].z));
                
                temp_vertex_normal_vector.push_back( vec3(current_mesh->mNormals[j].x,
                                                          current_mesh->mNormals[j].y,
                                                          current_mesh->mNormals[j].z));
            }            
            
            for (unsigned int j=0; j < current_mesh->mNumFaces; j++)
            {
                const aiFace &face = current_mesh->mFaces[j];
                if (face.mNumIndices != 3)
                {
                    //this shouldn't be reachable for a while since assimp is given the
                    //aiProcess_Triangulate flag, so it builds triangles for us
                    printf("face has %i vertices\n", face.mNumIndices);
                }
                objectTriangle current_triangle;
                current_triangle.vertices[0] =
                    current_triangle.vertex_normal[0] = face.mIndices[0] + vertex_offset;
                
                current_triangle.vertices[1] =
                    current_triangle.vertex_normal[1] = face.mIndices[1] + vertex_offset;
                
                current_triangle.vertices[2] =
                    current_triangle.vertex_normal[2] = face.mIndices[2] + vertex_offset;
                
                current_triangle.vertex_normals = true;
                
                temp_triangles.push_back(current_triangle);
            }
        }
        
        //now since we know how many vertices are actually in the scene/file, 
        //we can roll them in to the lists that cObject works with
        
        vertices_count_ = temp_vertices_vector.size();
        
        vertices_ = new point3[vertices_count_];
        vertex_normals_ = new vec3[vertices_count_];
        
        for (unsigned int i = 0; i < temp_vertices_vector.size(); i++)
        {
            vertices_[i] = temp_vertices_vector[i];
            vertex_normals_[i] = temp_vertex_normal_vector[i];
        }
        
        triangle_count_ = temp_triangles.size();
        triangles_ = new objectTriangle[triangle_count_];
        
        for (unsigned int i = 0; i < temp_triangles.size(); i++)
        {
            triangles_[i] = temp_triangles[i];
        }
        
        printf("    Triangle count: %d\n", triangle_count_);
        printf("Finished making cObject from file [assimp]\n");
    }
    else
    {
        printf("Error loading object [assimp]");
        exit(EXIT_FAILURE);
    }
}
void cObject::translate(vec3 trans_vec)
{
    translate_vector_ = trans_vec;
}
void cObject::rotate(vec3 rotate_vec)
{
    rotate_vector_ = rotate_vec;
}
void cObject::scale(vec3 scale_vec)
{
    scale_vector_ = scale_vec;
}

void cObject::assignMaterial()
{
    
}

point3 cObject::getVertex(unsigned int vertex)
{
    assert(vertex < vertices_count_);
    return vertices_[vertex];    
}

vec3 cObject::getVertexNormal(unsigned int vertex)
{
    assert(vertex < vertices_count_);
    return vertex_normals_[vertex];
}

vec3 cObject::getTranslate()
{
    return translate_vector_;
}

vec3 cObject::getRotate()
{
    return rotate_vector_;
}

vec3 cObject::getScale()
{
    return scale_vector_;
}

unsigned int cObject::getNumTriangle()
{
    return triangle_count_;
}