#include "Ocean.h"

#include <random>
#include <iostream>
#include <vector>

#include <chrono>

static const float PI = 3.1415926f;

static void bitReverseCopy(const std::vector<std::complex<float>> &a,
                           std::vector<std::complex<float>> &A)
{
    static int n = -1;
    static int *rev = nullptr;
    // Initialize rev[n] so that there is no need to recompute it later
    if (n != (int)A.size()) {
        n = (int)A.size();
        delete[] rev;
        rev = new int[n];

        for (int i = 0; i < n; ++i) {
            // 0b001 -> 0b100
            int revi = i;
            int len = 1;
            for (int j = 0; j < 32; ++j) {
                if ((1 << j) == n) {
                    len = j;
                    break;
                }
            }
            for (int j = 0; j < len / 2; ++j) {
                int tmp1 = (revi >> j) & 0x1;
                int tmp2 = (revi >> (len - 1 - j)) & 0x1;
                revi = revi & ~(1 << j);
                revi = revi | (tmp2 << j);
                revi = revi & ~(1 << (len - 1 - j));
                revi = revi | (tmp1 << (len - 1 - j));
            }
            rev[i] = revi;
        }
    }

    for (int i = 0; i < n; ++i) {
        A[rev[i]] = a[i];
    }
}

void iterativeFFT(const std::vector<std::complex<float>> &a,
                  std::vector<std::complex<float>> &A)
{
    using namespace std;
    auto n = a.size();
    bitReverseCopy(a, A);
    for (int s = 1; (1 << s) <= n; ++s) {
        auto m = (1 << s);
        auto wm = complex<float>(cos(2*PI/m), sin(2*PI/m));
        for (int k = 0; k < n; k += m) {
            complex<float> w = 1.0;
            for (int j = 0; j < m/2; ++j) {
                auto t = w * A[k + j + m/2];
                auto u = A[k + j];
                A[k + j] = u + t;
                A[k + j + m/2] = u - t;
                w = w * wm;
            }
        }
    }
}


Ocean::Ocean(glm::vec2 wind, int resolution_N, int resolution_M, float amplitude, int x, int z, float height)
        : v_wind(wind), N(resolution_N),M(resolution_M), A(amplitude), center_x(x), center_z(z)
{
    // Precompute indices and vertices
    vertexCount = 3 * N * M;
    indexCount  = 6 * N * M;
    vertices = new float[vertexCount];
    indices  = new unsigned int[indexCount];
    for (unsigned int i = 0; i < N - 1; ++i) {
        for (unsigned int j = 0; j < M - 1; ++j) {
            indices[6 * (i * M + j)    ] = (i * M + j);
            indices[6 * (i * M + j) + 1] = (i * M + j + 1);
            indices[6 * (i * M + j) + 2] = ((i + 1) * M + j);
            indices[6 * (i * M + j) + 3] = (i * M + j + 1);
            indices[6 * (i * M + j) + 4] = ((i + 1) * M + j);
            indices[6 * (i * M + j) + 5] = ((i + 1) * M + j + 1);
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int pos = 3 * (i * M + j);
            vertices[pos + 0] = ((i - N / 2) + center_x) * 8;
            vertices[pos + 1] = height;
            vertices[pos + 2] = ((j - M / 2) + center_z) * 8;
        }
    }
    // Initialize ocean wave related data
    g  = 9.8f;
    PI = 3.1415926f;

    Lx = N / 8;
	Lz = M / 8;
	
    unitWidth = 3.0f;
    choppy = 0.0f;
    hBuffer            = new std::complex<float>[N * M];
    kBuffer            = new glm::vec2[N * M];
    epsilonBufferx     = new std::complex<float>[N * M];
    epsilonBuffery     = new std::complex<float>[N * M];
    displacementBufferx = new std::complex<float>[N * M];
    displacementBuffery = new std::complex<float>[N * M];

    heightMapBuffer = new float[3 * N * M];
    normalMapBuffer = new float[3 * N * M];

    // Compute k buffer
    for (int n = -N / 2; n < N / 2; n++) {
        float kx = 2.0f * PI * n / Lx;
        for (int m = -M / 2; m < M / 2; m++) {
			float ky = 2.0f * PI * m / Lz;
            glm::vec2 k = glm::vec2(kx, ky);

            int bufferIndex = ((n + N/2) * M) + (m + M/2);
            kBuffer[bufferIndex] = k;
        }
    }

    // Setup height map and normal map
    glGenTextures(1, &heightMap);
    glBindTexture(GL_TEXTURE_2D, heightMap);
    // Set default texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Setup height map and normal map
    glGenTextures(1, &normalMap);
    glBindTexture(GL_TEXTURE_2D, normalMap);
    // Set default texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Ocean::~Ocean()
{
    delete[] vertices;
    delete[] indices;
    delete[] hBuffer;
    delete[] kBuffer;
    delete[] epsilonBufferx;
    delete[] epsilonBuffery;
    delete[] displacementBufferx;
    delete[] displacementBuffery;
    delete[] heightMapBuffer;
    delete[] normalMapBuffer;
}

void Ocean::generateWave(float time)
{
    // Eliminate inital status when time accumulate from 0
    time += 10000;
    time /= 2;
    using namespace std;
    // Compute buffers
    for (int n = -N / 2; n < N / 2; ++n) {
        for (int m = -M / 2; m < M / 2; ++m) {
            int bufferIndex = ((n + N/2) * M) + (m + M/2); // get the index of the array 

			// calculate the set of h(k, t)
            hBuffer[bufferIndex] = h(kBuffer[bufferIndex], time);

            epsilonBufferx[bufferIndex] = hBuffer[bufferIndex] * complex<float>(0.0f, kBuffer[bufferIndex].x);
            epsilonBuffery[bufferIndex] = hBuffer[bufferIndex] * complex<float>(0.0f, kBuffer[bufferIndex].y);

            auto k = kBuffer[bufferIndex];
            float k_len = sqrt(pow(k.x, 2) + pow(k.y, 2));
            if (k_len < 0.00001) {
                displacementBufferx[bufferIndex] = 0;
                displacementBuffery[bufferIndex] = 0;
            } else {
                displacementBufferx[bufferIndex] = -(epsilonBufferx[bufferIndex] / k_len);
                displacementBuffery[bufferIndex] = -(epsilonBuffery[bufferIndex] / k_len);
            }
        }
    }

		// Set Wave vertices and normals seperately
        auto *HBuffer = new std::complex<float>[N * M];
		
        // First round of FFT on rows
        for (int i = 0; i < N; ++i) {
            vector<complex<float>> a(M), buf(M);

            for (int j = 0; j < M; ++j)
                a[j] = hBuffer[i * M + j];
            iterativeFFT(a, buf);
            for (int j = 0; j < M; ++j)
                HBuffer[i * M + j] = buf[j];

            vector<complex<float>> b(M), c(M), buf2(M), buf3(M);
            for (int j = 0; j < M; ++j) {
                b[j] = epsilonBufferx[i * M + j];
                c[j] = epsilonBuffery[i * M + j];
            }
            iterativeFFT(b, buf2);
            iterativeFFT(c, buf3);
            for (int j = 0; j < M; ++j) {
                epsilonBufferx[i * M + j] = buf2[j];
                epsilonBuffery[i * M + j] = buf3[j];
            }

            for (int j = 0; j < M; ++j) {
                buf2[j] = buf3[j] = 0;
            }
            for (int j = 0; j < M; ++j) {
                b[j] = displacementBufferx[i * M + j];
                c[j] = displacementBuffery[i * M + j];
            }
            iterativeFFT(b, buf2);
            iterativeFFT(c, buf3);
            for (int j = 0; j < M; ++j) {
                displacementBufferx[i * M + j] = buf2[j];
                displacementBuffery[i * M + j] = buf3[j];
            }
        }

        // Second round of FFT on columns
        for (int i = 0; i < M; ++i) {
            vector<complex<float>> a(N), buf(N);
            for (int j = 0; j < N; ++j)
                a[j] = HBuffer[j * M + i];
            iterativeFFT(a, buf);
            for (int j = 0; j < N; ++j) {
                if ((i + j) % 2 == 0)
                    HBuffer[j * M + i] = buf[j];
                else
                    HBuffer[j * M + i] = -buf[j];
            }

            vector<complex<float>> b(N), c(N), buf2(N), buf3(N);
            for (int j = 0; j < N; ++j) {
                b[j] = epsilonBufferx[j * M + i];
                c[j] = epsilonBuffery[j * M + i];
            }
            iterativeFFT(b, buf2);
            iterativeFFT(c, buf3);
            for (int j = 0; j < N; ++j) {
                if ((i + j) % 2 == 0) {
                    epsilonBufferx[j * M + i] = buf2[j];
                    epsilonBuffery[j * M + i] = buf3[j];
                } else {
                    epsilonBufferx[j * M + i] = -buf2[j];
                    epsilonBuffery[j * M + i] = -buf3[j];
                }
            }

            for (int j = 0; j < N; ++j) {
                buf2[j] = buf3[j] = 0;
            }
            for (int j = 0; j < N; ++j) {
                b[j] = displacementBufferx[j * M + i];
                c[j] = displacementBuffery[j * M + i];
            }
            iterativeFFT(b, buf2);
            iterativeFFT(c, buf3);
            for (int j = 0; j < N; ++j) {
                if ((i + j) % 2 == 0) {
                    displacementBufferx[j * M + i] = buf2[j];
                    displacementBuffery[j * M + i] = buf3[j];
                } else {
                    displacementBufferx[j * M + i] = -buf2[j];
                    displacementBuffery[j * M + i] = -buf3[j];
                }
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                int index = i * M + j;
                int pos = 3 * (i * M + j);

                glm::vec3 heightVector = glm::vec3(-displacementBufferx[index].real(),
                                                    HBuffer[i * M + j].real(),
                                                   -displacementBuffery[index].real());

                heightVector = heightVector / 5.0f + glm::vec3(0.5f);
                heightMapBuffer[pos + 0] = heightVector.x;
                heightMapBuffer[pos + 1] = heightVector.y;
                heightMapBuffer[pos + 2] = heightVector.z;
                if (heightVector.x > 1.0 || heightVector.y > 1.0 || heightVector.z > 1.0
                        || heightVector.x < 0.0 || heightVector.y < 0.0 || heightVector.z < 0.0) {
                    std::cout << "Warning" << std::endl;
                }

                glm::vec3 normal = glm::vec3(-epsilonBufferx[index].real(),
                                              1.0f,
                                             -epsilonBuffery[index].real());
                normal = glm::normalize(normal) / 2.0f + glm::vec3(0.5f);

                normalMapBuffer[pos + 0] = normal.x;
                normalMapBuffer[pos + 1] = normal.y;
                normalMapBuffer[pos + 2] = normal.z;
            }
        }
        delete[] HBuffer;

        // Setup height map and normal map
        glBindTexture(GL_TEXTURE_2D, heightMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, N, M,
                     0, GL_RGB, GL_FLOAT, heightMapBuffer);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, N, M,
                     0, GL_RGB, GL_FLOAT, normalMapBuffer);
}


// ================================= h(k, t) ============================
// calculate omega(k)
float Ocean::omega(glm::vec2 k)
{
	return sqrt(g * glm::length(k));
}

// calculate h0(k)
std::complex<float> Ocean::h0(glm::vec2 k)
{
	using std::complex;
	float xi1 = normalRandom(), 
		xi2 = normalRandom();
	return (1.0f / sqrt(2.0f)) * complex<float>(xi1, xi2) * sqrt(Ph(k));
}

// create a normal distribuited random variable
float Ocean::normalRandom()
{
	static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static std::default_random_engine generator((unsigned)seed);
	static std::normal_distribution<float> dist(0.5, 0.1);
	return dist(generator);
}


// calculate Ph(k)
float Ocean::Ph(glm::vec2 k)
{
	if (glm::length(k) < 0.001f)	// avoid the situation that denominator = 0
		return 0.0f;

	float k_len = glm::length(k),
		L = pow(glm::length(v_wind), 2) / g;

	float result = A;
	result *= exp(-1.0f / pow((k_len*L), 2)) / pow(k_len, 4);
	result *= pow(glm::dot(glm::normalize(k), glm::normalize(v_wind)), 2);
	return result;
}

// calculate h(k, t)
std::complex<float> Ocean::h(glm::vec2 k, float t)
{
	using std::complex;
	float omega_k = omega(k);
	float coswt = cos(omega_k * t), 
		sinwt = sin(omega_k * t);
	complex<float> result = h0(k) * complex<float>(coswt, sinwt) + std::conj(h0(-k)) * complex<float>(coswt, -sinwt);
	return result;
}
// ======================================================================


float Ocean::H(float x, float z, float t)
{
    using std::complex;
    complex<float> result(0.0f, 0.0f);
    for (int n = -N / 2; n < N / 2; ++n) {
        for (int m = -M / 2; m < M / 2; ++m) {
            int bufferIndex = (n + N/2) * M + m + M/2;
            glm::vec2 k = kBuffer[bufferIndex];
            float k_dot_x = glm::dot(k, glm::vec2(x, z));

            result += hBuffer[bufferIndex] * complex<float>(cos(k_dot_x), sin(k_dot_x));
        }
    }
    return result.real();
}



glm::vec3 Ocean::epsilon(float x, float z, float t)
{
    using std::complex;
    glm::vec3 result(0.0f, 0.0f, 0.0f);
    for (int n = -N / 2; n < N / 2; ++n) {
        for (int m = -M / 2; m < M / 2; ++m) {
            int bufferIndex = (n + N/2) * M + m + M/2;
            glm::vec2 k = kBuffer[bufferIndex];
            float k_dot_x = glm::dot(glm::vec2(x, z), k);

            complex<float> tmp = hBuffer[bufferIndex]
                                 * complex<float>(cos(k_dot_x), sin(k_dot_x));

            glm::vec2 v = -tmp.imag() * k;
            result.x += v.x;
            result.z += v.y;
        }
    }
    return result;
}

glm::vec3 Ocean::D(float x, float z, float t)
{
    using std::complex;
    glm::vec3 result(0.0f, 0.0f, 0.0f);
    for (int n = -N / 2; n < N / 2; ++n) {
        for (int m = -M / 2; m < M / 2; ++m) {
            int bufferIndex = (n + N/2) * M + m + M/2;
            glm::vec2 k = kBuffer[bufferIndex];
            float k_dot_x = glm::dot(glm::vec2(x, z), k);

            complex<float> tmp = hBuffer[bufferIndex]
                                 * complex<float>(sin(k_dot_x), -cos(k_dot_x));

            glm::vec2 v = (tmp.real()/glm::length(k)) * k;
            result.x += v.x;
            result.z += v.y;
        }
    }
    return result;
}








