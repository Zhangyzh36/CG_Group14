#ifndef OCEAN_H
#define OCEAN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <complex>
#include <glad/glad.h>


class Ocean
{
public:
    Ocean(glm::vec2 wind, int resolution_N, int resolution_M, float amplitude, int x, int z, float height);
    ~Ocean();

    // Given current time, generate wave
    void generateWave(float time);

    // The texture used to store selected heights
    unsigned int heightMap, normalMap;
    // The 3*N*N array to store final vertices position and indice information
    int vertexCount;
    float *vertices;
    int indexCount;
    unsigned int *indices;
    // The flag to control generating method
    bool useFFT;
private:
	// center of the ocean
	int center_x, center_z;

    float g;
    float PI;
    float unitWidth;
    // Choppiness of the wave
    float choppy;
    // Resolution
    int N, M;
    // Water Length
    int Lx, Lz;
    // Wave height amplitude parameter
    float A;
    // Wind direction and speed in one vector
    glm::vec2 v_wind;
    // the buffer to store computed results
    std::complex<float> *hBuffer;
    glm::vec2 *kBuffer;
    std::complex<float> *epsilonBufferx;
    std::complex<float> *epsilonBuffery;
    std::complex<float> *displacementBufferx;
    std::complex<float> *displacementBuffery;

    float *heightMapBuffer;
    float *normalMapBuffer;

    // Returns height
    float H(float x, float z, float t);

    std::complex<float> h(glm::vec2 k, float t);
    std::complex<float> h0(glm::vec2 k);
    inline float normalRandom();
    inline float Ph(glm::vec2 k);
    inline float omega(glm::vec2 k);
    glm::vec3 epsilon(float x, float z, float t);

    // Returns displacement vector
    glm::vec3 D(float x, float z, float t);
};


#endif
