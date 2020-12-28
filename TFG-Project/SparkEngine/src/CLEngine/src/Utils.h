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
 
 
#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include <GLM/vec3.hpp>

using namespace glm;
using namespace std;
using namespace std::chrono;

class Utils {
   public:

    static int64_t getMicrosSinceEpoch() {
        return std::chrono::duration_cast<std::chrono::microseconds>(system_clock::now().time_since_epoch()).count();
    }

    static int64_t getMillisSinceEpoch() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    static float DegToRad(float deg) {
        return deg * 3.1415 / 180.0;
    }

    static float RadToDeg(float rad) {
        uint32_t auxDeg = rad * (180.0 / 3.1415);
        return (auxDeg + 360) % 360;
    }

    static float getRandomFloat(float LO, float HI) {
        if(LO > HI)
            std::swap(LO, HI);
        int divisor = HI - LO;
        if(divisor != 0)
            return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(divisor)));
        else {
            //cout << "OJO el random ha dado divisor cero, devolvemos LO" << endl;
            return LO;        
        }
    }


    static int getRandomInt(int LO, int HI) {
        if(LO > HI)
            std::swap(LO, HI);
        int divisor = (HI+1)-LO;
        if(divisor != 0)
            return LO + rand() /(RAND_MAX/divisor);
        else {
            //cout << "OJO el random ha dado divisor cero, devolvemos LO" << endl;
            return LO;        
        }
    }
};

    