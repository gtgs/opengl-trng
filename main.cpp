#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <memory.h>
#define WIDTH 800
#define HEIGHT 600
GLuint compileProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
GLuint loadShaderFromFile(const char* shader, GLuint type);
GLuint loadTexture(const char* filename, int width, int height, GLint bitsPerPixel);
GLuint loadVertexDataFromFile(const char* filename, GLfloat** outData, GLuint numberOfVertices, GLuint stride);

GLint y = 0;
GLint x = 0;
GLuint polygonMode = GL_FILL;
int main() {
	if(!glfwInit()) {
		fprintf(stderr, "problem with starting glfw");
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow( WIDTH, HEIGHT, "Render a triangle", NULL, NULL);
	if( NULL == window){
	    fprintf( stderr, "Failed to open a window\n" );
	    glfwTerminate();
	    return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glewInit();

	/** create a VAO **/
	GLuint va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	GLfloat* vertices = NULL;
	GLfloat* colors = NULL;
	GLfloat* uvs = NULL;

	loadVertexDataFromFile("./assets/cube.vertices", &vertices, 36, 3);

	return EXIT_SUCCESS;

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/** set the data pointer **/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	/** set the data pointer for color **/
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);


	GLuint vertexShader = loadShaderFromFile("./vs.glsl",GL_VERTEX_SHADER);
	if(!vertexShader){
		fprintf(stderr, "cannot load vertex shader");
		return EXIT_FAILURE;
	}

	GLuint fragmentShader = loadShaderFromFile("./fs.glsl",GL_FRAGMENT_SHADER);
	if(!vertexShader){
		fprintf(stderr, "cannot load fragment shader");
		return EXIT_FAILURE;
	}

	GLuint programId = compileProgram(vertexShader, fragmentShader);
	if(!programId) {
		fprintf(stderr, "error in generating shaders");
		return EXIT_FAILURE;

	}
	int width = 0;
	int height = 0;
	int bpp = 0;
	GLuint textureBrick = loadTexture("./assets/brick.png", width, height, bpp);
	if(textureBrick == 0){
		fprintf(stderr, "could not load texture");
		return EXIT_FAILURE;
	}


	glm::mat4 identity = glm::mat4(1.0f);


	glm::mat4 model = identity;
//	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
//	glm::mat4 model = identity * translation;


	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	do{
		if(glfwGetKey(window, GLFW_KEY_UP)) {
			y++;
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN)) {
			y--;
		}
		if(glfwGetKey(window, GLFW_KEY_LEFT)) {
			x--;
		}
		if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
			x++;
		}
		if(glfwGetKey(window, GLFW_KEY_SPACE)){
			if(GL_FILL == polygonMode){
				polygonMode = GL_LINE;
			} else if(GL_LINE == polygonMode) {
				polygonMode = GL_POINT;
			} else {
				polygonMode = GL_FILL;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		glUseProgram(programId);
	    glEnableVertexAttribArray(0);
	    glEnableVertexAttribArray(1);
	    glEnableVertexAttribArray(2);

	    glm::mat4 camera = glm::lookAt(
			glm::vec3(x, y, 10),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);

		glm::mat4 projection = glm::perspective(glm::radians(60.0f), WIDTH * 1.0f/HEIGHT, 0.1f, 1000.0f);

		glm::mat4 mvp = projection * camera * model;


	    GLuint vs_mvp = glGetUniformLocation(programId, "mvp");
	    glUniformMatrix4fv(vs_mvp, 1, GL_FALSE, &mvp[0][0]);


	    /** set the attrib pointer inside the data **/
	    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	    glBindTexture(GL_TEXTURE_2D, textureBrick);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	    glDisableVertexAttribArray(0);
	    glDisableVertexAttribArray(1);
	    glDisableVertexAttribArray(2);

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

}




GLuint loadTexture(const char* filename, int width, int height, GLint bitsPerPixel) {
	fprintf(stderr, "Loading texture from %s\n", filename);
	GLuint textureId;
	glGenTextures(1, &textureId);
	if(textureId == 0) {
			fprintf(stderr, "cannot generate texture");
			return EXIT_FAILURE;
	}
	glBindTexture(GL_TEXTURE_2D, textureId);
	unsigned char* imageData = stbi_load(filename, &width, &height, &bitsPerPixel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	return textureId;
}

GLuint loadShaderFromFile(const char* shader, GLuint type) {
	GLuint shaderId = glCreateShader(type);
	FILE * fp = fopen(shader, "r");
	if(NULL == fp){
		fprintf(stderr, "cannot read shader file - %s", shader);
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	unsigned long length = ftell(fp);
	rewind(fp);
	char* shaderSource = (char*) malloc (sizeof(char) * (length + 1));
	memset(shaderSource, 0, sizeof(char) * (length + 1));
	fread(shaderSource, length, 1, fp);

	fprintf(stderr, "\n shader source is \n--%s (%lu bytes)--\n%s\n--END-- ", shader, length, shaderSource);

	/** compile the shader **/
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	GLint compilationResult = false;
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationResult);
	if(!compilationResult) {
		GLint infoLogLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {
			char * msg = (char*) malloc(sizeof(char) * (infoLogLength + 1));
			glGetShaderInfoLog(shaderId, infoLogLength, 0, msg);
			fprintf(stderr, "\n there was an error compiling the shader - %s", msg);
			return 0;
		}
	}
	return shaderId;
}

GLuint compileProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	/** link the program **/
	GLint linkStatus = false;
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
	if(!linkStatus){
		GLint infoLogLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {
			char * msg = ((char*) malloc(sizeof(char) * (infoLogLength + 1)));
			glGetProgramInfoLog(programId, infoLogLength, 0, msg);
			fprintf(stderr, "there was an error linking the shader - %s", msg);
			return 0;
		}
	}
	return programId;
}

GLuint loadVertexDataFromFile(const char* filename, GLfloat** outData, GLuint numberOfVertices, GLuint stride) {
	FILE * fp = fopen(filename, "r");
	if(NULL == fp){
		fprintf(stderr, "cannot read file - %s\n", filename);
		return 0;
	}
	fprintf(stderr, "reading file - %s\n", filename);
	if(NULL == *outData) {
		fprintf(stderr, "allocating memory for %d bytes \n", sizeof(GLfloat) * numberOfVertices * stride);
		*outData = (GLfloat*) malloc(sizeof(GLfloat) * numberOfVertices * stride);
	}
	GLuint index = 0;
	while(index < (numberOfVertices * stride)){
		fprintf(stderr, "%d", index);
		//fscanf(fp, "%f,%f,%f", *outData[index++], *outData[index++], *outData[index++]);

		fprintf(stderr, "read %f, %f, %f\n", *outData[index], *outData[index + 1], *outData[index + 2]);
	}

	fclose(fp);
	return 0;
}
