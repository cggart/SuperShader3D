/*	Copyright (C) 2011 by Eddy Luten

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
	http://openglbook.com/the-book/chapter-4-entering-the-third-dimension/
*/
#include "Utils.h"
#include "wavefront_loader.h"
#include "freeimage\FreeImage.h"
#include "super_gui.h"

float latPosOffset = 0;
float posOffset = 0;

#define WINDOW_TITLE_PREFIX "Super 3D"
int tricount;
int CurrentWidth = 800,
	CurrentHeight = 600,
	WindowHandle = 0;

unsigned FrameCount = 0;

GLuint
	ProjectionMatrixUniformLocation,
	ViewMatrixUniformLocation,
	ModelMatrixUniformLocation,
	BufferIds[3] = { 0 },
	ShaderIds[3] = { 0 };

Matrix
	ProjectionMatrix,
	ViewMatrix,
	ModelMatrix;

float CubeRotation = 0;
clock_t LastTime = 0;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void createMesh(void);
void DestroyCube(void);
void DrawCube(void);
void keyboard(unsigned char, int, int);
void setupLines(void);
void drawLines(void); 

	GLuint lineShdr_PrgmID;
	GLuint linesVBOid;
	GLuint vaoBufferid;
	GLuint lineShdr_vertID;
	GLuint lineShdr_fragID;

	bool dl = 0;

int  main(int argc, char* argv[])
{
	Initialize(argc, argv);

	glutMainLoop();
	
	exit(EXIT_SUCCESS);
}
void Initialize(int argc, char* argv[])
{
	initGui();
	GLenum GlewInitResult;

	InitWindow(argc, argv);
	
	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	glGetError();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

	ModelMatrix = IDENTITY_MATRIX;
	ProjectionMatrix = IDENTITY_MATRIX;
	ViewMatrix = IDENTITY_MATRIX;


	TranslateMatrix(&ViewMatrix, 0, 0, -2);

	createMesh();
	setupLines();
}
void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
	GLUT_ACTION_ON_WINDOW_CLOSE,
	GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);
	
	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if(WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);
	glutCloseFunc(DestroyCube);
	glutKeyboardFunc(keyboard);
}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
	ProjectionMatrix =
		CreateProjectionMatrix(
			60,
			(float)CurrentWidth / CurrentHeight,
			1.0f,
			100.0f
		);

		glUseProgram(ShaderIds[0]);
		glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m);
		glUseProgram(0);
		glUseProgram(lineShdr_PrgmID);
		glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m);
		glUseProgram(0);
}

void RenderFunction(void)
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawCube();
	if(dl)
	drawLines();

	
	glutSwapBuffers();
	glutPostRedisplay();
}
void IdleFunction(void)
{
	glutPostRedisplay();
}
void TimerFunction(int Value)
{
	if (0 != Value) {
		char* TempString = (char*)
			malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
		);

		glutSetWindowTitle(TempString);
		free(TempString);
	}

	FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}
void drawLines(void)
{
	float CubeAngle;
	clock_t Now = clock();

	if (LastTime == 0)
		LastTime = Now;
	ViewMatrix = IDENTITY_MATRIX;
	TranslateMatrix(&ViewMatrix, 0+latPosOffset, 0, -2+posOffset);
	CubeRotation += 45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC);
	CubeAngle = DegreesToRadians(CubeRotation);
	LastTime = Now;

	ModelMatrix = IDENTITY_MATRIX;
	RotateAboutY(&ModelMatrix, CubeAngle);


	//glUseProgram(ShaderIds[0]);
	glUseProgram(lineShdr_PrgmID);
	ExitOnGLError("ERROR: Could not use the shader program");

	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.m);
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.m);
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(vaoBufferid);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawArrays(GL_LINES, 0, 36);

	ExitOnGLError("ERROR: Could not draw the cube");

	//not using a vertex definition any more
	glBindVertexArray(0);
	//noting using a shader anymore
	glUseProgram(0); 

}
void setupLines()
{	

float myLines[] = {
  // X      Y     Z     
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f,
	-0.8f, -0.1f, 13.0f,
	-0.5f,  0.5f, 0.0f,
	  1,	2,		3,
	  1,    1,      1,
	  4,   0.8,     2,
	  0,    0,      1
};

	//generate some space for our new GLSL program & grab
	//said space's id
	lineShdr_PrgmID = glCreateProgram();
		ExitOnGLError("ERROR: Could not create the shader program");
	//load from disk & compile our GLSL code and store it on the GPU
	lineShdr_fragID = LoadShader("shaders/lineShader.fragment.glsl", GL_FRAGMENT_SHADER);
	lineShdr_vertID = LoadShader("shaders/lineShader.vertex.glsl", GL_VERTEX_SHADER);

	//Attach our compiled GLSL
	glAttachShader(lineShdr_PrgmID, lineShdr_fragID);
	glAttachShader(lineShdr_PrgmID, lineShdr_vertID);

	//merge it all together and compile a GLSL program
	//into an executable
	glLinkProgram(lineShdr_PrgmID);
	ExitOnGLError("ERROR: Could not link the shader program");

	//Here we grab the IDs of the matricies with our GLSL program so that we can
	//set them equal to our c++ controlled matricies when we are ready to render.

	ModelMatrixUniformLocation = glGetUniformLocation(lineShdr_PrgmID, "ModelMatrix");
	ViewMatrixUniformLocation = glGetUniformLocation(lineShdr_PrgmID, "ViewMatrix");
	ProjectionMatrixUniformLocation = glGetUniformLocation(lineShdr_PrgmID, "ProjectionMatrix");
	ExitOnGLError("ERROR: Could not get shader uniform locations");


	glGenVertexArrays(1, &vaoBufferid);
	ExitOnGLError("ERROR: Could not generate the VAO");
	
	//Make our VBO Current VBO
	glBindVertexArray(vaoBufferid);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glEnableVertexAttribArray(0);
	ExitOnGLError("ERROR: Could not enable vertex attributes");

	//glGenBuffer grabs a slot buffer slot from the GPU
	//that isn't in use.  If we use one that was in use
	//it would cause everything to go foobar.
	//We tell it how manu slots to tell us about, and a space to store the results

	linesVBOid = 0;
	glGenBuffers(1, &linesVBOid);
	ExitOnGLError("ERROR: Could not generate the buffer objects");


	//Now we dump our raw data into our gpu , and free up
	//anything on the heap.  In this case its all on the stack so
	//no worries there.

	glBindBuffer(GL_ARRAY_BUFFER, linesVBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myLines), &myLines[0], GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	//attribute index  0 | 3 members (pos) | They Are floats (GL_FLOAT) | Do Not Normalize | stride is sizeof(vertex) |Source of Data (0 Means current VBO)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pos3), (GLvoid*)0);

	glBindVertexArray(0);
}
void createMesh()
{
	gModel_3d* theModel = load_wavefront_obj("cube.obj");
	
		ShaderIds[0] = glCreateProgram();
		ExitOnGLError("ERROR: Could not create the shader program");

	ShaderIds[1] = LoadShader("shaders/SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
	ShaderIds[2] = LoadShader("shaders/SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
	glAttachShader(ShaderIds[0], ShaderIds[1]);
	glAttachShader(ShaderIds[0], ShaderIds[2]);
	glLinkProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not link the shader program");


	ModelMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
	ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
	ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
	ExitOnGLError("ERROR: Could not get shader uniform locations");

	glGenVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	ExitOnGLError("ERROR: Could not enable vertex attributes");
	
		glGenBuffers(2, &BufferIds[1]);
	ExitOnGLError("ERROR: Could not generate the buffer objects");

		int size = theModel->theMesh.pos.size() * sizeof(theModel->theMesh.pos[0]) ;

	glBindBuffer(GL_ARRAY_BUFFER, 1);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");
	glBufferData(GL_ARRAY_BUFFER, size,  &theModel->theMesh.pos[0], GL_STATIC_DRAW);
	
	//defing the VAO
	int posSize= sizeof(theModel->theMesh.pos[0].position);
	int normalSize = sizeof(theModel->theMesh.pos[0].normal);
	int uvSize = sizeof(theModel->theMesh.pos[0].uv);
	//attribute index  0 | 3 members (pos) | They Are floats (GL_FLOAT) | Do Not Normalize | stride is sizeof(vertex) |Source of Data (0 Means current VBO)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(theModel->theMesh.pos[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(theModel->theMesh.pos[0]), (GLvoid*)posSize);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(theModel->theMesh.pos[0]), (GLvoid*)(posSize+normalSize));

	ExitOnGLError("ERROR: Could not set VAO attributes");

	static GLuint texture = 0;

FIBITMAP* bitmap = FreeImage_Load(
        FreeImage_GetFileType("tex.bmp", 0),
        "tex.bmp");

	FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
	0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(pImage);

	int indiceSize = theModel->theMesh.indices.size() * sizeof(theModel->theMesh.indices[0]);
	//int test = sizeof(INDICES);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize, &theModel->theMesh.indices[0], GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");
	tricount = theModel->theMesh.indices.size();
	glBindVertexArray(0);
}
void createNormals(gModel_3d* model)
{
	int size = model->theMesh.pos.size();

	std::vector<pos3> normal_indices;
	for(int i=0;i<size;i++)
	{
		model->theMesh.pos[0].normal;
		//Add The Origin
		pos3 origin = model->theMesh.pos[0].position;
		pos3 normal = model->theMesh.pos[0].normal;
		normal_indices.push_back(origin);
		pos3 pos;
		pos.x = normal.x + origin.x;
		pos.y = normal.y + origin.y;
		pos.z = normal.z + origin.z;
	}
	
}
void DestroyCube()
{
	glDetachShader(ShaderIds[0], ShaderIds[1]);
	glDetachShader(ShaderIds[0], ShaderIds[2]);
	glDeleteShader(ShaderIds[1]);
	glDeleteShader(ShaderIds[2]);
	glDeleteProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not destroy the shaders");

	glDeleteBuffers(2, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}
void DrawCube(void)
{
	float CubeAngle;
	clock_t Now = clock();
	if (LastTime == 0)
		LastTime = Now;
	ViewMatrix = IDENTITY_MATRIX;
	TranslateMatrix(&ViewMatrix, 0+latPosOffset, 0, -2+posOffset);
	CubeRotation += 45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC);
	CubeAngle = DegreesToRadians(CubeRotation);
	LastTime = Now;

	ModelMatrix = IDENTITY_MATRIX;
	RotateAboutY(&ModelMatrix, CubeAngle);
	//RotateAboutX(&ModelMatrix, CubeAngle);

	glUseProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.m);
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.m);
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawElements(GL_TRIANGLES, tricount, GL_UNSIGNED_INT, (GLvoid*)0);
	ExitOnGLError("ERROR: Could not draw the cube");

	//not using a vertex definition any more
	glBindVertexArray(0);
	//noting using a shader anymore
	glUseProgram(0); 
}
void keyboard(unsigned char key, int x, int y)
{
	/* This time the controls are:
	
	  "a": move left
	  "d": move right
	  "w": move forward
	  "s": move back
	  "t": toggle depth-testing

	*/
	switch (key)
	{
	case 'a':
	case 'A':
		latPosOffset=latPosOffset+0.05;
		break;
	case 'd':
	case 'D':
		latPosOffset=latPosOffset-0.05;
		break;
	case 'f':
	case 'F':
		if(dl){dl = 0;}else{dl=1;}
		break;
	case 's':
	case 'S':
		posOffset=posOffset-0.05;
		break;
	case 'w':
	case 'W':
		posOffset=posOffset+0.05;
		break;
	}
}