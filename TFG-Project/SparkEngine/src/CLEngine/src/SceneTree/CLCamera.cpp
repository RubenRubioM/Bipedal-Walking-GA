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
 
#include "CLCamera.h"

using namespace CLE;

CLCamera::CLCamera(unsigned int idEntity) : CLEntity(idEntity) {

}

/**
 * Cambia los valores de configuracion de la camara.
 */
void CLCamera::SetCameraConfig(GLfloat n, GLfloat f, GLfloat r, GLfloat l, GLfloat t, GLfloat b) {
    m_near = n; m_far = f; right = r; left = l; top = t; bottom = b;
    CalculateProjectionMatrix();
}

/**
 * Calcula la matriz de proyeccion segun si usamos perspectiva o ortografico.
 */
glm::mat4 CLCamera::CalculateProjectionMatrix() {
    glm::mat4 projection = glm::mat4(0.0f);
    if (perspective) {
        //TO-DO: Poner variable el spect
        projection = glm::perspective(glm::radians(fov), aspect, m_near, m_far);
    }
    else {
        projection = glm::ortho(left, right, bottom, top, m_near, m_far);
    }

    return projection;
}

void CLCamera::Draw(GLuint shaderID) {

}

void CLCamera::DrawDepthMap (GLuint shaderID){}
