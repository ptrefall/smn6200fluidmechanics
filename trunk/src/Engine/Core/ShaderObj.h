#pragma once

namespace Engine
{
	enum ShaderType 
	{
		V_SHADER, 
		F_SHADER,
		G_SHADER
	};

	class ShaderObj
	{
	public:
		ShaderObj();
		~ShaderObj();

		bool isShaderSet() const { return isSet; }
		bool isShaderInitialized() const { return initialized; }

		bool setShader(const CL_String &filename);
		bool initShader();

		void enableShader();
		void disableShader();
		
		unsigned int getShaderProg() const { return prog; }

	protected:
		int shaderSize(const CL_String &fileName, ShaderType shaderType);
		int readShader(const CL_String &fileName, ShaderType shaderType, char *shaderText, int size);
		
		bool isSet, initialized;
		unsigned int vs, fs, gs, prog;
		char *vertexShaderSource, *fragmentShaderSource, *geometryShaderSource;
	};
}
