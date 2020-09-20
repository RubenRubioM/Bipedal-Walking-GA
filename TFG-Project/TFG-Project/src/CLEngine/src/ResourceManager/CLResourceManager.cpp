/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
#include "CLResourceManager.h"
#include "../Constants.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    meshes.reserve(300);
    textures.reserve(300);
}

CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file){
    return GetResourceMesh(file,false);
}

CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file, bool flipUV) {
    shared_ptr<CLResourceMesh> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<meshes.size() && search; ++ i) {
        if (!file.compare(meshes[i]->GetName())) {
            resource = meshes[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceMesh>();
        resource->SetName(file);
        if (resource->LoadFile(file, flipUV)) {
            meshes.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file){
    return GetResourceTexture(file,false);
}

CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file, bool vertically) {
    shared_ptr<CLResourceTexture> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<textures.size() && search; ++ i) {
        if (!file.compare(textures[i]->GetName())) {
            resource = textures[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceTexture>();
        resource->SetName(file);
        if (resource->LoadFile(file, vertically)) {
            textures.push_back(resource);
        }
    }

    

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string vertex, const std::string fragment) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!vertex.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        //cout << "Creo el shader: " << vertex <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(vertex);
        //resource->SetShaderType(type);
        if (resource->LoadFile(vertex,fragment)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string vertex, const std::string fragment, const std::string geometry) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!vertex.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        //cout << "Creo el shader: " << vertex <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(vertex);
        //resource->SetShaderType(type);
        if (resource->LoadFile(vertex,fragment,geometry)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

bool CLResourceManager::DeleteResourceTexture(const std::string file){

    for (unsigned int i=0; i<textures.size(); ++ i) {
        if (!file.compare(textures[i]->GetName())) {
            textures.erase(textures.begin()+i);
            return true;
        }
    }

    return false;
}

bool CLResourceManager::DeleteResourceMesh(const std::string file){
    // cout << meshes.size() << endl;
    for (unsigned int i=0; i<meshes.size(); ++ i) {
        if (!file.compare(meshes[i]->GetName())) {
            meshes.erase(meshes.begin()+i);
            // cout << meshes.size() << endl;

            return true;
        }
    }
    return false;
}
