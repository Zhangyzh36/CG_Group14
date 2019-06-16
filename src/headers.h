#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game_object/car.h"
#include "game_object/City.h"
#include "game_object/plane.h"

#include "camera/camera.h"
#include "stb_image/stb_image.h"
#include "renderers/RendererManager.h"
#include "./model_loader/loader.h"
#include "./particle//ParticleGenerator.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "shaders/shader.h"
#include "renderers/SkyboxRenderer.h"
#include "renderers/entityRenderer.h"
#include "renderers/RendererManager.h"
