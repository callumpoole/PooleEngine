#include "opengl_texture.h"
#include "stb/stb_image.h"
#include "poole/rendering/shader_loader.h"

namespace Poole::Rendering
{
	OpenGL_Texture::OpenGL_Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	{
		type = texType;

		int width = -1, height = -1, numChannels = -1;
		stbi_set_flip_vertically_on_load(true); //Stb reads top to bottom, openGL is the opposite, so change stb mode.
		unsigned char* bytes = stbi_load(image, &width, &height, &numChannels, 0);

		if (width == -1 || height == -1 || numChannels == -1)
		{
			LOG_ERROR("Failed to load image: {}", image);
			ID = 0;
			return;
		}
		else
		{
			LOG("Loaded: {}, W: {}, H: {}, Channels: {}", image, width, height, numChannels);
		}

		//Generates an OpenGL texture object
		glGenTextures(1, &ID);

		//Assigns texture to the texture unit
		glActiveTexture(slot);
		glBindTexture(texType, ID);

		//Set minification and magnification algo (could also use GL_????)
		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Set how the texture repeats, could also use (??????, GL_CLAMP_TO_BORDER)
		glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Only for GL_CLAMP_TO_BORDER
		//float flatColor[] = { 1,1,1,1 };
		//glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, flatColor);

		//Assign the image to the opengl texture object
		glTexImage2D(texType, 0, /*BUG?*/GL_RGBA, width, height, 0, format, pixelType, bytes);

		//Gen mipMaps
		glGenerateMipmap(texType);

		//NOTE about Mipmaps: 
		//"In OpenGL specifying the mip filter is done as part of the min filter, and uses filter names in the format GL_MIPMAP, 
		// where the first * is the min filter and the second * is the mip filter.Going by the rest of your code I would suggest 
		// that GL_NEAREST_MIPMAP_NEAREST is what you want, although you could try GL_NEAREST_MIPMAP_LINEAR for the crunchy pixel 
		// look but with less abrupt transitions between mip levels." -Some guy on a thread below
		//Source: https://community.khronos.org/t/texture-renders-as-black-square/75799/8

		//Deletes the data since openGL has it now
		stbi_image_free(bytes);

		//Unbinds the object to prevent accidental modification
		glBindTexture(texType, 0);
	}

	void OpenGL_Texture::SetTextureUnit(GLShader& shader, const char* uniform, GLuint unit)
	{
		GLuint texUni = glGetUniformLocation(shader.GetProgramID(), uniform);

		shader.Bind();

		glUniform1i(texUni, unit);
	}

	OpenGL_Texture::~OpenGL_Texture()
	{
		glDeleteTextures(1, &ID);
	}

	void OpenGL_Texture::Bind()
	{
		glBindTexture(type, ID);
	}
	void OpenGL_Texture::Unbind()
	{
		glBindTexture(type, 0);
	}
}