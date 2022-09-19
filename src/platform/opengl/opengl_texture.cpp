#include "opengl_texture.h"
#include "poole/rendering/image/image.h"
#include "poole/rendering/shader_loader.h"

namespace Poole::Rendering
{
	//===BOTH===

	OpenGL_Texture::~OpenGL_Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	//===OLD===

	OpenGL_Texture::OpenGL_Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
		: OpenGL_Texture(Image(imagePath), texType, slot, format, pixelType)
	{
	}

	OpenGL_Texture::OpenGL_Texture(const Image& image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	{
		m_Type = texType;
		m_Size = image.GetSize();

		//Generates an OpenGL texture object
		glGenTextures(1, &m_ID);

		//Assigns texture to the texture unit
		glActiveTexture(slot);
		glBindTexture(texType, m_ID);

		//Set minification and magnification algo (could also use GL_NEAREST or GL_LINEAR)
		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Set how the texture repeats, could also use (??????, GL_CLAMP_TO_BORDER)
		glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Only for GL_CLAMP_TO_BORDER
		//float flatColor[] = { 1,1,1,1 };
		//glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, flatColor);

		//Assign the image to the opengl texture object
		glTexImage2D(texType, 0, /*BUG?*/GL_RGBA, image.GetWidth(), image.GetHeight(), 0, format, pixelType, image.GetData());

		//Gen mipMaps
		glGenerateMipmap(texType);

		//NOTE about Mipmaps: 
		//"In OpenGL specifying the mip filter is done as part of the min filter, and uses filter names in the format GL_MIPMAP, 
		// where the first * is the min filter and the second * is the mip filter.Going by the rest of your code I would suggest 
		// that GL_NEAREST_MIPMAP_NEAREST is what you want, although you could try GL_NEAREST_MIPMAP_LINEAR for the crunchy pixel 
		// look but with less abrupt transitions between mip levels." -Some guy on a thread below
		//Source: https://community.khronos.org/t/texture-renders-as-black-square/75799/8

		//Unbinds the object to prevent accidental modification
		glBindTexture(texType, 0);
	}

	void OpenGL_Texture::SetTextureUnit(GLShader& shader, const char* uniform, GLuint unit)
	{
		GLuint texUni = glGetUniformLocation(shader.GetProgramID(), uniform);

		shader.Bind();

		glUniform1i(texUni, unit);
	}



	void OpenGL_Texture::Bind()
	{
		glBindTexture(m_Type, m_ID);
	}
	void OpenGL_Texture::Unbind()
	{
		glBindTexture(m_Type, 0);
	}

	//====NEW====


	OpenGL_Texture::OpenGL_Texture(u32 width, u32 height)
		: m_Size({width, height})
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		//glGenTextures(1, &m_ID);
		//glBindTexture(GL_TEXTURE_2D, m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGL_Texture::SetData(const void* data, u32 size)
	{
		const u32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		//assert(size == GetWidth() * GetHeight() * bpp, "Data must be an entire texture!");
		glTextureSubImage2D(m_ID, 0, 0, 0, GetWidth(), GetHeight(), m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGL_Texture::Bind(u32 slot)
	{
		glBindTextureUnit(slot, m_ID);
	}
}