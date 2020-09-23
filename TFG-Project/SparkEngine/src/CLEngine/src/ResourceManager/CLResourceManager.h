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
 * @author Judith Mula Molina <https://www.linkedin.com/in/judith-mm-18099215a/> 
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * 
 */
 
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "CLResourceMesh.h"
#include "CLResourceShader.h"
#include "CLResourceTexture.h"

using namespace std;
namespace CLE {

    //! Clase para gestionar la lectura de recursos
    //! Esta clase engloba todos los metodos para leer diferentes tipos de archivos
    //! @note Es singleton
    class CLResourceManager {
        public:
            //! Constructor singleton
            static CLResourceManager* GetResourceManager(){
                if(!instance){
                    instance = new CLResourceManager();
                }
                return instance;
            }
            //! Destructor
            ~CLResourceManager(){delete instance;};
            //! Metodo para leer mallas
            //! @param file Ruta al fichero
            //! @param flipUV Bool para invertir los UV's al leer la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            CLResourceMesh* GetResourceMesh(const std::string file, bool flipUV);
            //! Metodo para leer mallas
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            CLResourceMesh* GetResourceMesh(const std::string file);
            //! Metodo para leer texturas
            //! @param file Ruta al fichero
            //! @param vertically Bool para poner en vertical la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceTexture
            CLResourceTexture* GetResourceTexture(const std::string file, bool vertically);
            //! Metodo para leer texturas
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceTexture
            CLResourceTexture* GetResourceTexture(const std::string file);
            //! Metodo para leer shaders
            //! @param vertex Ruta al vertex shader
            //! @param fragment Ruta al fragment shader
            //! @returns Puntero a un CLE::CLResource::CLResourceShader
            CLResourceShader* GetResourceShader(const std::string vertex, const std::string fragment);
            //! Metodo para leer shaders
            //! @param vertex Ruta al vertex shader
            //! @param fragment Ruta al fragment shader
            //! @param geometry Ruta al geometry shader
            //! @returns Puntero a un CLE::CLResource::CLResourceShader
            CLResourceShader* GetResourceShader(const std::string vertex, const std::string fragment, const std::string geometry);
            //! Libera la memoria de una textura
            //! @param file
            //! @returns bool
            bool DeleteResourceTexture(const std::string file);
            //! Libera la memoria de una malla
            //! @param file
            //! @returns bool
            bool DeleteResourceMesh(const std::string file);
            //! Devuelve las mallas
            //! @returns mallas
            vector<shared_ptr<CLResourceMesh>> GetMeshes() {return meshes;}
        private:
            CLResourceManager();
            inline static CLResourceManager* instance { nullptr };
            vector<shared_ptr<CLResourceMesh>> meshes;
            vector<shared_ptr<CLResourceTexture>> textures;
            vector<shared_ptr<CLResourceShader>> shaders;
    };

}