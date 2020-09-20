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
 
#include "CLResourceTexture.h"

using namespace CLE;

bool CLResourceTexture::LoadFile(string file, bool vertically){

    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    
    //stbi_set_flip_vertically_on_load(false);
    if(vertically) {
        //stbi_set_flip_vertically_on_load(true);
    }
    unsigned char *data = SOIL_load_image(file.c_str(), &width, &height, &nrComponents, 0);
    widthText = width;
    heightText = height;
    
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            return false;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); 

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << file << std::endl;
        SOIL_free_image_data(data);
        return false;
    }
    return true;
}

void CLResourceTexture::Draw(GLuint shaderID){
    glBindTexture(GL_TEXTURE_2D, textureID);
}

CLResourceTexture::~CLResourceTexture() {
    //cout << "Entra en el destructor de Textura\n";
    glDeleteTextures(1, &textureID);
}