#include "precomp.h"
#include "ShaderObj.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

using namespace Engine;

ShaderObj::ShaderObj()
: isSet(false), initialized(false), geometryShaderSource(NULL)
{
}

ShaderObj::~ShaderObj()
{
}

bool ShaderObj::setShader(const CL_String &fileName)
{
	if(isSet)
		return false;

	int vSize, fSize, gSize;

	// Allocate memory to hold the source of our shaders.
	vSize = shaderSize(fileName, V_SHADER);
	fSize = shaderSize(fileName, F_SHADER);
	gSize = shaderSize(fileName, G_SHADER);

	if((vSize == -1) || (fSize == -1))
		throw CL_Exception(cl_format("Cannot determine size of the shader %1", fileName));

	vertexShaderSource = (GLchar *) malloc(vSize);
	fragmentShaderSource = (GLchar *) malloc(fSize);

	// Read the source code
	if(!readShader(fileName, V_SHADER, vertexShaderSource, vSize))
		throw CL_Exception(cl_format("Cannot read the file %1.vs", fileName));

	if(!readShader(fileName, F_SHADER, fragmentShaderSource, fSize))
		throw CL_Exception(cl_format("Cannot read the file %1.fs", fileName));

	if(gSize > -1)
	{
		geometryShaderSource = (GLchar *) malloc(gSize);
		if(!readShader(fileName, G_SHADER, geometryShaderSource, gSize))
			throw CL_Exception(cl_format("Cannot read the file %1.gs", fileName));
	}

	isSet = true;
	return true;
}

bool ShaderObj::initShader()
{
	if(initialized)
		return false;

	if(!isShaderSet())
		setShader("res/shaders/minimal");

	GLint vertCompiled, fragCompiled, geomCompiled;    // status values
	GLint linked;
	int infologLength = 0;
	int charsWritten  = 0;
	GLchar *infoLog;

	// Create a vertex shader object and a fragment shader object
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vertS = vertexShaderSource;
	const char *fragS = fragmentShaderSource;
	const char *geomS = geometryShaderSource;

	// Load source code strings into shaders
	glShaderSource(vs, 1, &vertS, NULL);
	glShaderSource(fs, 1, &fragS, NULL);

	if(geometryShaderSource)
	{
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, &geomS, NULL);
	}

	// Compile the vertex shader, and print out
	// the compiler log file.
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vertCompiled);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
			throw CL_Exception(cl_format("Could not allocate InfoLog buffer"));

		glGetShaderInfoLog(vs, infologLength, &charsWritten, infoLog);
		if(charsWritten > 0)
			throw CL_Exception(cl_format("VertexShader InfoLog: %1", infoLog));

		free(infoLog);
	}

	// Compile the fragment shader, and print out
	// the compiler log file.

	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fragCompiled);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
			throw CL_Exception(cl_format("Could not allocate InfoLog buffer"));

		glGetShaderInfoLog(fs, infologLength, &charsWritten, infoLog);
		if(charsWritten > 0)
			throw CL_Exception(cl_format("FragmentShader InfoLog: %1", infoLog));

		free(infoLog);
	}

	// Compile the geometry shader, and print out
	// the compiler log file.
	if(geometryShaderSource)
	{
		glCompileShader(gs);
		glGetShaderiv(gs, GL_COMPILE_STATUS, &geomCompiled);
		glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0)
		{
			infoLog = (GLchar *)malloc(infologLength);
			if (infoLog == NULL)
				throw CL_Exception(cl_format("Could not allocate InfoLog buffer"));

			glGetShaderInfoLog(gs, infologLength, &charsWritten, infoLog);
			if(charsWritten > 0)
				throw CL_Exception(cl_format("GeometryShader InfoLog: %1", infoLog));

			free(infoLog);
		}

		if(!geomCompiled)
			return false;
	}

	if (!vertCompiled || !fragCompiled)
		return false;

	// Create a program object and attach the two compiled shaders
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	if(geometryShaderSource)
	{
		glAttachShader(prog, gs);

		glProgramParameteriEXT(prog,GL_GEOMETRY_INPUT_TYPE,GL_POINTS);
		glProgramParameteriEXT(prog,GL_GEOMETRY_OUTPUT_TYPE,GL_LINES);

		int temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES,&temp);
		glProgramParameteriEXT(prog,GL_GEOMETRY_VERTICES_OUT,temp);
	}

	// Link the program object and print out the info log
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
			throw CL_Exception(cl_format("Could not allocate InfoLog buffer"));

		glGetProgramInfoLog(prog, infologLength, &charsWritten, infoLog);
		if(charsWritten > 0)
			throw CL_Exception(cl_format("Program InfoLog: %1", infoLog));

		free(infoLog);
	}
	if (!linked)
		return false;

	initialized = true;
	return true;
}

void ShaderObj::enableShader()
{
	glUseProgram(prog);
}

void ShaderObj::disableShader()
{
	glUseProgram(0);
}

int ShaderObj::shaderSize(const CL_String &fileName, ShaderType shaderType)
{
  // Returns the size in bytes of the shader fileName.
  // If an error occurred, it returns -1.
  //
  // File name convention:
  //
  // <fileName>.vert
  // <fileName>.frag
  //
  int fd;
  char name[100];
  int count = -1;

  strcpy(name, fileName.c_str());

  switch(shaderType)
  {
    case V_SHADER:
		strcat(name, ".vs");
		break;
    case F_SHADER:
		strcat(name, ".fs");
		break;
	case G_SHADER:
		strcat(name, ".gs");
		break;
    default:
		throw CL_Exception(cl_format("Unknown shader file type"));
  }

  // Open the file, seek to the end to find its length (WIN32)
  fd = _open(name, _O_RDONLY);
  if (fd != -1)
  {
    count = _lseek(fd, 0, SEEK_END) + 1;
    _close(fd);
  }
  return count;
}

int ShaderObj::readShader(const CL_String &fileName, ShaderType shaderType, char *shaderText, int size)
{
  // Reads a shader from the supplied file and returns the shader in the
  // arrays passed in. Returns 1 if successful, 0 if an error occurred.
  // The parameter size is an upper limit of the amount of bytes to read.
  // It is ok for it to be too big.
  FILE *fh;
  char name[100];
  int count;

  strcpy(name, fileName.c_str());

  switch (shaderType) 
  {
    case V_SHADER:
      strcat(name, ".vs");
      break;
    case F_SHADER:
      strcat(name, ".fs");
      break;
	case G_SHADER:
      strcat(name, ".gs");
      break;
    default:
      throw CL_Exception(cl_format("Unknown shader file type"));
  }

  // Open the file
  fh = fopen(name, "r");
  if (!fh)
    return -1;

  // Get the shader from a file.
  fseek(fh, 0, SEEK_SET);
  count = (int) fread(shaderText, 1, size, fh);
  shaderText[count] = '\0';

  if (ferror(fh))
      count = 0;

  fclose(fh);
  return count;
}
