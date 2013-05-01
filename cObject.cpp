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
#include "material.h"

/* 
 Takes the cObject and creates and returns a pointer to a rendModel corresponding to the 
 cObject.
 */
std::shared_ptr<rendModel> cObject::addToRender()
{
    std::shared_ptr<rendModel> new_rendmodel (new rendModel(*this));
    return new_rendmodel;
}

/*
 Private function to create an identity triangle centered at (0.0, 0.0, 0.0) 
 */
void cObject::makeCTRIANGLE()
{
    vertices_.resize(3);
    
    vertices_[0] = point3(0.0, 0.5, 0.0);
    vertices_[1] = point3(-0.5, -0.5, 0.0);
    vertices_[2] = point3(0.5, -0.5, 0.0);
    
    objectTriangle triangle;
    triangle.vertices[0] = 0;
    triangle.vertices[1] = 1;
    triangle.vertices[2] = 2;
    
    triangle.vertex_normals = false;
    
    std::shared_ptr<material> default_material (new material);
    default_material->diffuse = col3(0.5, 0.5, 0.5);
    default_material->specular = default_material->emissive = default_material->reflective = default_material->transparent = col3(0.0, 0.0, 0.0);
    default_material->shininess = default_material->shininess_strength = default_material->reflectivity = 0.0;
    default_material->refractivity = 0.0;
    triangle.triangle_material = default_material;
    
    triangles_.push_back(triangle);
}

void cObject::makeCBOX()
{
    
    std::shared_ptr<material> default_material (new material);
    default_material->diffuse = col3(0.5, 0.5, 0.5);
    default_material->specular = default_material->emissive = default_material->reflective = default_material->transparent = col3(0.0, 0.0, 0.0);
    default_material->shininess = default_material->shininess_strength = default_material->reflectivity = 0.0;
    default_material->refractivity = 0.0;
    
    vertices_.resize(36);
    
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
    triangle1.triangle_material = default_material;
    
    objectTriangle triangle2;
    triangle2.vertices[0] = 3; //top right
    triangle2.vertices[1] = 4; //bottom left
    triangle2.vertices[2] = 5; //bottom right
    
    triangle2.vertex_normals = false;
    triangle2.triangle_material = default_material;
    
    //back side
    objectTriangle triangle3;
    triangle3.vertices[0] = 6; //top left
    triangle3.vertices[1] = 7; //bottom left
    triangle3.vertices[2] = 8; //top right
    
    triangle3.vertex_normals = false;
    triangle3.triangle_material = default_material;
    
    objectTriangle triangle4;
    triangle4.vertices[0] = 9; //top right
    triangle4.vertices[1] = 10; //bottom left
    triangle4.vertices[2] = 11; //bottom right
    
    triangle4.vertex_normals = false;
    triangle4.triangle_material = default_material;
    
    //bottom side
    objectTriangle triangle5;
    triangle5.vertices[0] = 12; //close left
    triangle5.vertices[1] = 13; //far left
    triangle5.vertices[2] = 14; //far right
    
    triangle5.vertex_normals = false;
    triangle5.triangle_material = default_material;
    
    objectTriangle triangle6;
    triangle6.vertices[0] = 15; //far right
    triangle6.vertices[1] = 16; //close left
    triangle6.vertices[2] = 17; //close right
    
    triangle6.vertex_normals = false;
    triangle6.triangle_material = default_material;
    
    //top side
    objectTriangle triangle7;
    triangle7.vertices[0] = 18; //close left
    triangle7.vertices[1] = 19; //far left
    triangle7.vertices[2] = 20; //far right
    
    triangle7.vertex_normals = false;
    triangle7.triangle_material = default_material;
    
    objectTriangle triangle8;
    triangle8.vertices[0] = 21; //far right
    triangle8.vertices[1] = 22; //close left
    triangle8.vertices[2] = 23; //close right
    
    triangle8.vertex_normals = false;
    triangle8.triangle_material = default_material;
    
    //left side
    objectTriangle triangle9;
    triangle9.vertices[0] = 24; //far bottom
    triangle9.vertices[1] = 25; //far top
    triangle9.vertices[2] = 26; //close top
    
    triangle9.vertex_normals = false;
    triangle9.triangle_material = default_material;
    
    objectTriangle triangle10;
    triangle10.vertices[0] = 27; //far bottom
    triangle10.vertices[1] = 28; //close top
    triangle10.vertices[2] = 29; //close bottom
    
    triangle10.vertex_normals = false;
    triangle10.triangle_material = default_material;
    
    //right side
    objectTriangle triangle11;
    triangle11.vertices[0] = 30; //far bottom
    triangle11.vertices[1] = 31; //far top
    triangle11.vertices[2] = 32; //close top
    
    triangle11.vertex_normals = false;
    triangle11.triangle_material = default_material;
    
    objectTriangle triangle12;
    triangle12.vertices[0] = 33; //far bottom
    triangle12.vertices[1] = 34; //close top
    triangle12.vertices[2] = 35; //close bottom
    
    triangle12.vertex_normals = false;
    triangle12.triangle_material = default_material;
    
    triangles_.push_back(triangle1);
    triangles_.push_back(triangle2);
    triangles_.push_back(triangle3);
    triangles_.push_back(triangle4);
    triangles_.push_back(triangle5);
    triangles_.push_back(triangle6);
    triangles_.push_back(triangle7);
    triangles_.push_back(triangle8);
    triangles_.push_back(triangle9);
    triangles_.push_back(triangle10);
    triangles_.push_back(triangle11);
    triangles_.push_back(triangle12);
}

void cObject::makeCTESTOBJECT()
{
    
    std::shared_ptr<material> default_material (new material);
    default_material->diffuse = col3(0.5, 0.5, 0.5);
    default_material->specular = default_material->emissive = default_material->reflective = default_material->transparent = col3(0.0, 0.0, 0.0);
    default_material->shininess = default_material->shininess_strength = default_material->reflectivity = 0.0;
    default_material->refractivity = 0.0;
    
    vertices_.resize(9);
	
    //left
	objectTriangle triangle1;
	vertices_[1] = point3(-0.1, 0.1, -1.3);
	vertices_[2] = point3(0.0, 0.1, -1.3);
	vertices_[0] = point3(-0.1, -0.1, -1.3);
    
    triangle1.vertices[1] = 0;
	triangle1.vertices[2] = 1;
	triangle1.vertices[0] = 2;
	
    triangle1.vertex_normals = false;
    triangle1.triangle_material = default_material;
    
	triangles_.push_back(triangle1);
	
	//right top
	objectTriangle triangle2;
	vertices_[3] = point3(-0.2, 0.1, -1.55);
	vertices_[5] = point3(0.3, 0.1, -1.50);
	vertices_[4] = point3(0.2, -0.1, -1.53);
    
    triangle2.vertices[0] = 3;
	triangle2.vertices[2] = 4;
	triangle2.vertices[1] = 5;
	
    triangle2.vertex_normals = false;
    triangle2.triangle_material = default_material;
    
    triangles_.push_back(triangle2);
    
    //right bottom
    objectTriangle triangle3;
	vertices_[6] = point3(-0.1, -0.15, -1.55);
	vertices_[8] = point3(0.4, -0.1, -1.50);
	vertices_[7] = point3(0.3, -0.3, -1.0);
    
    triangle3.vertices[0] = 6;
	triangle3.vertices[2] = 7;
	triangle3.vertices[1] =  8;
    
    triangle3.vertex_normals = false;
    triangle3.triangle_material = default_material;
	
	triangles_.push_back(triangle3);
    

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
 Creates cObject from an object/model/scene loaded from filename.
 */
cObject::cObject(std::string filename)
{
    
    translate_vector_ = vec3 (0.0, 0.0, 0.0);
    rotate_vector_ = vec3(0.0, 0.0, 0.0);
    scale_vector_ = vec3(1.0, 1.0, 1.0);
    
    std::cout << "Loading object from file [assimp]" << std::endl;

    Assimp::Importer importer;
    
    const aiScene* p_scene = importer.ReadFile(filename, aiProcess_Triangulate |aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    //std::vector<std::shared_ptr<material>> materials_;
    
    if (p_scene)
    {     
        for (unsigned int i=0; i < p_scene->mNumMeshes; i++)
        {
            const aiMesh* current_mesh = p_scene->mMeshes[i];
            
            materials_.push_back(std::shared_ptr<material>(new material));
            struct aiMaterial *ai_material = p_scene->mMaterials[current_mesh->mMaterialIndex];
            
            aiColor4D diffuse;
            if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
            {
                materials_.back()->diffuse = col3(diffuse.r, diffuse.g, diffuse.b);
                std::cout << diffuse.r << " " << diffuse.g << " " << diffuse.b << std::endl;
            }
            else
            {
                materials_.back()->diffuse = col3(0.0, 0.0, 0.0);
            }
            
            aiColor4D specular;
            if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &specular))
            {
                materials_.back()->specular = col3(specular.r, specular.g, specular.b);
                std::cout << "  " << specular.r << " " << specular.g << " " << specular.b << std::endl;
            }
            else
            {
                materials_.back()->specular = col3(0.0, 0.0, 0.0);
            }
            
            aiColor4D emissive;
            if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_EMISSIVE, &emissive))
            {
                materials_.back()->emissive = col3(emissive.r, emissive.g, emissive.b);
                std::cout << "  " << emissive.r << " " << emissive.g << " " << emissive.b << std::endl;
            }
            else
            {
                materials_.back()->emissive = col3(0.0, 0.0, 0.0);
            }
            
            aiColor4D reflective;
            if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_REFLECTIVE, &reflective))
            {
                materials_.back()->reflective= col3(reflective.r, reflective.g, reflective.b);
                std::cout << "  " << reflective.r << " " << reflective.g << " " << reflective.b << std::endl;
            }
            else
            {
                materials_.back()->reflective = col3(0.0, 0.0, 0.0);
            }
            
            aiColor4D transparent;
            if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_TRANSPARENT, &transparent))
            {
                materials_.back()->transparent = col3(transparent.r, transparent.g, transparent.b);
                std::cout << "  " << transparent.r << " " << transparent.g << " " << transparent.b << std::endl;
            }
            else
            {
                materials_.back()->transparent = col3(0.0, 0.0, 0.0);
            }
            
            float shininess = 0.0;
            unsigned int max;
            aiGetMaterialFloatArray(ai_material, AI_MATKEY_SHININESS, &shininess, &max);
            materials_.back()->shininess = shininess;
            std::cout << "  " << shininess << std::endl;
            
            float shininess_strength = 0.0;
            aiGetMaterialFloatArray(ai_material, AI_MATKEY_SHININESS_STRENGTH, &shininess_strength, &max);
            materials_.back()->shininess_strength = shininess_strength;
            std::cout << "  " << shininess_strength << std::endl;
            
            float reflectivity = 0.0;
            aiGetMaterialFloatArray(ai_material, AI_MATKEY_REFLECTIVITY, &reflectivity, &max);
            materials_.back()->reflectivity = reflectivity;
            std::cout << "  " << reflectivity << std::endl;
            
            float refractivity = 0.0;
            aiGetMaterialFloatArray(ai_material, AI_MATKEY_REFRACTI, &refractivity, &max);
            materials_.back()->refractivity = refractivity;
            std::cout << "  " << refractivity << std::endl;
            
            
            /*  we need this so we can keep the references to indices from the file
            aligning to the same vertices in our array of vertices, so we just take
            the number of vertices already in our (future) list and add this offset
            later on to keep it all aligned.
             */
            unsigned int vertex_offset = static_cast<unsigned int>(vertices_.size());
            
            for (unsigned int j=0; j < current_mesh->mNumVertices; j++)
            {
                vertices_.push_back( vec3(current_mesh->mVertices[j].x,
                                                     current_mesh->mVertices[j].y,
                                                     current_mesh->mVertices[j].z));
                
                vertex_normals_.push_back( vec3(current_mesh->mNormals[j].x,
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
                    std::cout << "face has" << face.mNumIndices << "vertices" << std::endl;
                }
                objectTriangle current_triangle;
                current_triangle.vertices[0] =
                    current_triangle.vertex_normal[0] = face.mIndices[0] + vertex_offset;
                
                current_triangle.vertices[1] =
                    current_triangle.vertex_normal[1] = face.mIndices[1] + vertex_offset;
                
                current_triangle.vertices[2] =
                    current_triangle.vertex_normal[2] = face.mIndices[2] + vertex_offset;
                
                current_triangle.vertex_normals = true;
                
                current_triangle.triangle_material = materials_[materials_.size() -1];
                
                triangles_.push_back(current_triangle);
            }
        }
        
        //now since we know how many vertices are actually in the scene/file, 
        //we can roll them in to the lists that cObject works with
                
        std::cout << "    Triangle count: " << triangles_.size() << std::endl;
        std::cout << "Finished making cObject from file [assimp]" << std::endl;
    }
    else
    {
        std::cout << "Error loading object [assimp]" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/* Set the translation vector for the object to trans_vec.
 */
void cObject::translate(vec3 trans_vec)
{
    translate_vector_ = trans_vec;
}

/* Set the rotation for the object to the rotation degrees found in rotate_vec.
 */
void cObject::rotate(vec3 rotate_vec)
{
    rotate_vector_ = rotate_vec;
}

/* Set the scale vector for the object to scale_vec.
 */
void cObject::scale(vec3 scale_vec)
{
    scale_vector_ = scale_vec;
}

/* Assigns the material referred to in new_material to all the triangles in the cObject.
 */
void cObject::assignAllMaterial(std::shared_ptr<material> new_material)
{
    for (auto &triangle : triangles_)
    {
        triangle.triangle_material = new_material;
    }
}

/* Returns the 3D point of the vertex found by the index "vertex".
 */
point3 cObject::getVertex(unsigned int vertex)
{
    return vertices_[vertex];
}

/* Returns the 3D vector normal of the vertex normal found by the index "vertex".
 */
vec3 cObject::getVertexNormal(unsigned int vertex)
{
    return vertex_normals_[vertex];
}

/* Returns the translation vector for the object.
 */
vec3 cObject::getTranslate()
{
    return translate_vector_;
}

/* Returns the rotation degrees for the object in vector format.
 */
vec3 cObject::getRotate()
{
    return rotate_vector_;
}

/* Returns the scale vector for the object.
 */
vec3 cObject::getScale()
{
    return scale_vector_;
}

/* Returns the total number of triangles found in the object.
 */
unsigned int cObject::getNumTriangle()
{
    return static_cast<unsigned int>(triangles_.size());
}