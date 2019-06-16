#include "headers.h"

using namespace std;
// -----------------------------
// window params
float SCR_WIDTH = 1600;
float SCR_HEIGHT = 800;
const float SKYBOX_SIZE = 800.0f;
// -------------------------------
// shadow params
unsigned int depthMapFBO;
unsigned int intermediateDepthMapFBO;

unsigned int quadVAO;
unsigned int quadVBO;
unsigned int framebuffer;
unsigned int textureColorBufferMultiSampled;
unsigned int intermediateFBO;
unsigned int screenTexture;


float SHADOW_WIDTH = 1000, SHADOW_HEIGHT = 1000;
float sleft = -10, sright = 10, sbottom = -10, stop = 10, sfar_plane = 77.f, snear_plane = 1.0;
// -------------------------------
// control params
float xPos = 0.0f;
float yPos = 0.0f;
bool firstMouse = true;
bool autoRotate = false;
bool autoScale = false;
bool autoTranslate = false;
bool keys[1024];
double updateTime = 0.0;
float scale = 1.0f;
float scaleDir = 1.0f;
float movementx = 0.0f;
float moveDir = 1.0f;
float rotation = 0.0f;
// -------------------------------
// camera
// camera 

float  particleScale= 2.35;
 
float preAngle = 0; 
float relativeDirection = 18; 
float relativeHeight = 5; 

glm::vec3 particleOffset = glm::vec3(0.0, 0.4, 0.0);

Camera camera(glm::vec3(relativeDirection, -0.5 + relativeHeight, 0)); 
// -------------------------------
// game objs
vector<Entity*> objs;
// ------------------------------
// projection
glm::mat4 lightSpaceMatrix;
glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 2800.0f);

GLFWwindow* openGLallInit();
void initImGui(GLFWwindow* window);
void renderImgui(bool menu);
void renderScene(Shader* shader);
void shadowMapping(Shader& simpleDepthShader, glm::mat4 lightSpaceMatrix);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void initShadow();
void mouseCallback(GLFWwindow*, double xpos, double ypos);
void move(GLfloat dtime, Car&);

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
void initTextShader(Shader& shader);


void initFrameBuffer();
void initIntermediateBuffer();
void renderScreenQuad(Shader &shader);

void blitBufferColor();
void blitBufferDepth();



int main() {
	GLfloat curFrame = 0.0f, lastFrame = 0.0f;
	camera.yaw = 190;
	camera.pitch = 2.5;
	camera.updateCamera();
	GLFWwindow* window = openGLallInit();
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetCursorPosCallback(window, mouseCallback);
	// glfwSetKeyCallback(window, keyCallback);
	initImGui(window);
	if (window == NULL)
		return -1;
	
	 std::vector<std::string> skyboxTextures = {
		 "res/sky/sky_rt.jpg",

		 "res/sky/sky_lf.jpg",

		 "res/sky/sky_up.jpg",

		 "res/sky/sky_dn.jpg",

		 "res/sky/sky_bk.jpg",

		 "res/sky/sky_ft.jpg"
	 };
	// // init renderer
	RendererManager::init();
	 SkyboxRenderer skybox(skyboxTextures, SKYBOX_SIZE);
	// init shadow
	initShadow();
	initFrameBuffer();
	initIntermediateBuffer();

	//float near_plane = 1.0f, far_plane = 77.5f;
	
	// ----------------------------------
	// init game object
	Car car("res/car/newcar2/Avent.obj");
	City city("res/Unity/3dsmax3.FBX");
	Plane plane;
	objs.push_back(&plane);
	objs.push_back(&car);
	objs.push_back(&city);
	// ----------------------------------
	// shader
	Shader shadowShader("shaders/glsl/shadow_depth.vs", "shaders/glsl/shadow_depth.fs");
	Shader entityShader("shaders/glsl/shadow_mapping.vs", "shaders/glsl/shadow_mapping.fs");
	entityShader.use();
	entityShader.setInt("shadowMap", 100);

	Shader textShader("shaders/glsl/text.vs", "shaders/glsl/text.fs");
	initTextShader(textShader);

	Shader carParticleShader("shaders/glsl/car_particle.vs", "shaders/glsl/car_particle.fs");
	Texture2D carParticleTexture("res/particle/gas2.jpg", GL_FALSE);

	ParticleGenerator *Particles = new ParticleGenerator(carParticleShader, carParticleTexture, 700);
	carParticleShader.setInt("sprite", 0);

	Shader screenShader("shaders/glsl/screen.vs", "shaders/glsl/screen.fs");
	screenShader.setInt("screenTexture", 0);

	while (!glfwWindowShouldClose(window)) {
		glm::mat4 lightProjection = glm::ortho(sleft, sright, sbottom, stop, snear_plane, sfar_plane);
		glm::mat4 lightView = glm::lookAt(
			glm::vec3(RendererManager::headlight.position),
			car.position,
			glm::vec3(0, 1, 0));
		lightSpaceMatrix = lightProjection * lightView;
		curFrame = glfwGetTime();
		// ----------------------------------
		// process config
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		// ----------------------------------
		// render shadow
		shadowMapping(shadowShader, lightSpaceMatrix);
		// ----------------------------------
		// render text
		std::string t = "speed" + std::to_string(car.speed);
		
		// ----------------------------------
		// render sky box
		//skybox.render(camera.getViewMat(), projection);
		// ----------------------------------
		// render scene
		glActiveTexture(GL_TEXTURE0 + 100);
		glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//
		renderScene(&entityShader);
		RenderText(textShader, t, 12.0f, 12.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		GLfloat dt = curFrame - lastFrame;
		Particles->Update(dt, car, 11, glm::vec3(-car.direction.x * particleScale, particleOffset.y - car.direction.y, -car.direction.z * particleScale));
		Particles->Draw(camera.getViewMat(), projection);
		// render imgui
		renderImgui(true);
		blitBufferColor();
		// ----------------------------------
		

		move(curFrame - lastFrame, car);
		lastFrame = curFrame;

		renderScreenQuad(screenShader);
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete Particles;
	return 0;
}

void renderScene(Shader* shader) {
	for (int i = 0; i < objs.size(); ++i) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, objs[i]->position);
		if (i != 0) {
			model = objs[i]->getModelMat();
		}
		EntityRenderer::render(
			shader,
			objs[i],
			objs[i]->useVertColor,
			projection,
			camera.getViewMat(),
			model,
			lightSpaceMatrix,
			RendererManager::headlight.position,
			camera.position,
			objs[i]->vertColor
		);
	}
}

void shadowMapping(Shader& simpleDepthShader, glm::mat4 lightSpaceMatrix) {
	//Shader simpleDepthShader("shaders/shadow_depth.vs", "shaders/shadow_depth.fs");
	simpleDepthShader.use();
	simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(&simpleDepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void initShadow() {
	// ---------------------------------------------
	// frame depth map
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &RendererManager::depthMap);
	glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, RendererManager::depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		autoRotate = !autoRotate;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		autoTranslate = !autoTranslate;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		autoScale = !autoScale;
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}


void mouseCallback(GLFWwindow*, double xpos, double ypos) {
	if (firstMouse) {
		xPos = xpos;
		yPos = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - xPos;
	float yoffset = yPos - ypos;
	//camera.mouseMoveHandler(-xoffset, -yoffset);
	xPos = xpos;
	yPos = ypos;
}

void move(GLfloat dtime, Car& car) {
	if (keys[GLFW_KEY_W]) {
		car.speedup(dtime);
	}
	else if (keys[GLFW_KEY_S]) {
		car.speeddown(dtime);
	}
	else {
		car.friction(dtime);
	}
	if (car.speed != 0)
	{
		car.move(dtime);
		 glm::vec3 relativePosition = glm::vec3(-relativeDirection * car.direction.x, relativeHeight, -relativeDirection * car.direction.z);
		 camera.position = car.position + relativePosition;
		 camera.yaw -= car.angle - preAngle;
		 preAngle = car.angle;
		 camera.updateCamera();
	}
	if (keys[GLFW_KEY_A]) {
		car.rotate(Car::turnAngle);
	}
	if (keys[GLFW_KEY_D]) {
		car.rotate(-Car::turnAngle);
	}

	if (keys[GLFW_KEY_E])
	{
		car.brake();
	}

	RendererManager::headlight.position.x = car.position.x + 3;
	RendererManager::headlight.position.z = car.position.z + 3;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void initImGui(GLFWwindow* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 330";
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}


GLFWwindow* openGLallInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "firWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_MULTISAMPLE);
	return window;
}


void renderImgui(bool menu) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Menu", &menu, ImGuiWindowFlags_MenuBar);
	ImGui::SliderFloat("yaw", &camera.yaw, -360, 360);
	ImGui::SliderFloat("pitch", &camera.pitch, -89, 89);
	ImGui::SliderFloat("c x", &camera.position.x, -10, 100);
	ImGui::SliderFloat("c y", &camera.position.y, -10, 100);
	ImGui::SliderFloat("c z", &camera.position.z, -10, 100);

	ImGui::SliderFloat("l", &sleft, -3600, 3600);
	ImGui::SliderFloat("r", &sright, -3600, 3600);
	ImGui::SliderFloat("b", &sbottom, -3600, 3600);
	ImGui::SliderFloat("t", &stop, -3600, 3600);
	ImGui::SliderFloat("f", &sfar_plane, 0, 10000);
	ImGui::SliderFloat("w", &SHADOW_WIDTH, 1000, 3600);
	ImGui::SliderFloat("h", &SHADOW_HEIGHT, 1000, 10000);
	ImGui::Text("particle offset");
	ImGui::SliderFloat("offset y", &particleOffset.y, 0, 5);
	ImGui::SliderFloat("particleScale", &particleScale, 1, 10);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint textVAO, textVBO;

void initTextShader(Shader& shader)
{
	// Compile and setup the shader
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	shader.use();
	shader.setMat4("projection", projection);
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void initFrameBuffer()
{
	float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							   // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a multisampled color attachment texture
	
	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initIntermediateBuffer()
{
	
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	// create a color attachment texture
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderScreenQuad(Shader & shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// draw Screen quad
	shader.use();
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture); // use the now resolved color attachment as the quad's texture
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void blitBufferColor()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void blitBufferDepth()
{
}


void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	shader.use();
	shader.setVec3("textColor", glm::vec3(color.x, color.y, color.z));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}