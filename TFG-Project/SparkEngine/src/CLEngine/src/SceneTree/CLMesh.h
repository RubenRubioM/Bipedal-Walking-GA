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
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * @author Jose Valdés Sirvent
 * 
 */
 
 
 #pragma once

#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "CLEntity.h"

#include <memory>
#include <unordered_map>

namespace CLE {

//! Clase para crear mallas
//! Clase que permite crear mallas en nuestro motor gráfico
class CLMesh : public CLEntity {
   public:
        CLMesh();
        CLMesh(unsigned int idEntity) : CLEntity(idEntity){};
        ~CLMesh(){};
        //! Asigna una malla
        //! @param m Objecto CLResourceMesh donde tiene la malla
        void SetMesh(CLResourceMesh* m) { mesh = m; }
        //GETTERS
        //! Devuelve la malla
        //! @returns mesh Objeto CLResourceMesh almacenado
        CLResourceMesh* GetMesh() const { return mesh; }
        void Draw(GLuint shaderID);
        void DrawDepthMap(GLuint shaderID);

    private:
        CLResourceMesh* mesh = nullptr;
        CLResourceMesh* nextMesh = nullptr;
        CLResourceTexture* texture = nullptr;
};
}  // namespace CLE