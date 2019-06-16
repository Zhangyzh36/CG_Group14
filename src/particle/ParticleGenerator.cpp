

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader &shader, Texture2D &texture, GLuint amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->init();
}

GLfloat diffuseParticle = 3500;
GLfloat upParticle = 0.008;
void ParticleGenerator::Update(GLfloat dt, Entity &object, GLuint newParticles, glm::vec3 offset)
{
 	// Add new particles 
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}

	
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= (dt * 2.0); // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position -= p.Velocity * dt;
			GLfloat randomX = ((rand() % 100) - 50) / diffuseParticle;
			GLfloat randomY = ((rand() % 100) - 50) / diffuseParticle + upParticle;
			GLfloat randomZ = ((rand() % 100) - 50) / diffuseParticle;
			p.Position += glm::vec3(randomX, randomY, randomZ);
			p.Color.a = p.Life;
		}
	}
	
}

// Render all particles
void ParticleGenerator::Draw(glm::mat4 view, glm::mat4 projection)
{
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	int i = 0;
	
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->shader.use();
			this->shader.setVec3("offset", particle.Position);
			this->shader.setVec4("color", particle.Color);
			this->shader.setMat4("view", view);
			this->shader.setMat4("projection", projection);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			
		}
	} 

	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Draw(Shader &shader, Texture2D &texture, glm::mat4 view, glm::mat4 projection) {
	
	GLuint VBO;
	GLuint VAO;
	GLfloat particle_quad[] = {
		-0.5f, 0.3f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.3f, -0.5f, 1.0f, 0.0f,
		-0.5f, 0.3f, -0.5f, 0.0f, 0.0f,

		-0.5f, 0.3f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.3f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.3f, -0.5f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	int i = 0;

	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			shader.setVec3("offset", particle.Position);
			shader.setVec4("color", particle.Color);
			texture.Bind();
			//glBindVertexArray(this->VAO);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			std::cout << i++ << std::endl;
		}
	}

	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*
	GLuint VBO;
	GLuint VAO;
	GLfloat particle_quad[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	debugShader.use();
	debugShader.setMat4("projection", projection);
	debugShader.setVec3("offset", glm::vec3(0.0, -0.5, 0.0));
	debugShader.setVec4("color", glm::vec4(1.0, 0.0, 0.0, 1.0));
	debugShader.setMat4("view", view);

	texture.Bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	*/
}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	/*
	GLfloat particle_quad[] = {
		0.0f, 0.1f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.1f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.1f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.1f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.1f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.1f, 0.0f, 1.0f, 0.0f
	};
	*/
	GLfloat particle_quad[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f,

		0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);

	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

GLfloat particleIntegrate = 700;
void ParticleGenerator::respawnParticle(Particle &particle, Entity &object, glm::vec3 offset)
{
	GLfloat randomX = ((rand() % 100) - 50) / particleIntegrate;
	GLfloat randomY = ((rand() % 100) - 50) / particleIntegrate;
	GLfloat randomZ = ((rand() % 100) - 50) / particleIntegrate;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f) / 2;
	particle.Position = object.position + glm::vec3(randomX, randomY, randomZ) +offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.velocity * 0.25f;
}
