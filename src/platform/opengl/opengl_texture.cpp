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

	void OpenGL_Texture::SetupTextureParams()
	{
		//Note there's a difference glTextureParameteri(m_ID... and glTexParameteri(m_TargetType... the latter requires glBindTexture(m_TargetType, m_ID); first

		//Set minification and magnification algo (could also use GL_NEAREST or GL_LINEAR)
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Set how the texture repeats, could also use (??????, GL_CLAMP_TO_BORDER)
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Only for GL_CLAMP_TO_BORDER
		//float flatColor[] = { 1,1,1,1 };
		//glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, flatColor);
	}

	//===OLD===

	OpenGL_Texture::OpenGL_Texture(const char* imagePath, GLenum slot)
		: OpenGL_Texture(Image(imagePath), slot)
	{
	}

	OpenGL_Texture::OpenGL_Texture(const Image& image, GLenum slot)
		: m_Size(image.GetSize())
		, m_TargetType(GL_TEXTURE_2D)
		, m_ShaderFormat(GL_RGBA8)
		, m_DataFormat(image.GetNumChannels() == 3 ? GL_RGB : GL_RGBA)
		, m_PixelType(image.GetFormat() == Image::EImageFormat::Bytes ? GL_UNSIGNED_BYTE : GL_FLOAT)
	{
		//Generates an OpenGL texture object
		glCreateTextures(m_TargetType, 1, &m_ID); //Similar to: glGenTextures(1, &m_ID);
		glBindTexture(m_TargetType, m_ID);

		//Assigns texture to the texture unit
		glActiveTexture(slot);

		SetupTextureParams();

		//Assign the image to the opengl texture object
		glTexImage2D(m_TargetType, 0, m_ShaderFormat, image.GetWidth(), image.GetHeight(), 0, m_DataFormat, m_PixelType, image.GetData());

		//Gen mipMaps
		glGenerateMipmap(m_TargetType);

		//NOTE about Mipmaps: 
		//"In OpenGL specifying the mip filter is done as part of the min filter, and uses filter names in the format GL_MIPMAP, 
		// where the first * is the min filter and the second * is the mip filter.Going by the rest of your code I would suggest 
		// that GL_NEAREST_MIPMAP_NEAREST is what you want, although you could try GL_NEAREST_MIPMAP_LINEAR for the crunchy pixel 
		// look but with less abrupt transitions between mip levels." -Some guy on a thread below
		//Source: https://community.khronos.org/t/texture-renders-as-black-square/75799/8

		//Unbinds the object to prevent accidental modification
		glBindTexture(m_TargetType, 0);
	}

	void OpenGL_Texture::SetTextureUnit(GLShader& shader, const char* uniform, GLuint unit)
	{
		GLuint texUni = glGetUniformLocation(shader.GetProgramID(), uniform);

		shader.Bind();

		glUniform1i(texUni, unit);
	}



	void OpenGL_Texture::Bind()
	{
		glBindTexture(m_TargetType, m_ID);
	}
	void OpenGL_Texture::Unbind()
	{
		glBindTexture(m_TargetType, 0);
	}

	//====NEW====


	OpenGL_Texture::OpenGL_Texture(uvec2 size)
		: m_Size(size)
		, m_TargetType(GL_TEXTURE_2D)
		, m_ShaderFormat(GL_RGBA8)
		, m_DataFormat(GL_RGBA)
		, m_PixelType(GL_UNSIGNED_BYTE)
	{
		glCreateTextures(m_TargetType, 1, &m_ID); //Samilar to: glGenTextures(1, &m_ID);
		//glBindTexture not needed since all functions called refer to the m_ID not the m_TargetType
		glTextureStorage2D(m_ID, 1, m_ShaderFormat, size.x, size.y);

		SetupTextureParams();
	}

	void OpenGL_Texture::SetData(const void* data, u32 size)
	{
		const u32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ASSERT_MSG(size == GetWidth() * GetHeight() * bpp, "Data must be an entire texture!");
		glTextureSubImage2D(m_ID, 0, 0, 0, GetWidth(), GetHeight(), m_DataFormat, m_PixelType, data);
	}

	void OpenGL_Texture::Bind(u32 slot)
	{
		glBindTextureUnit(slot, m_ID);
	}

	void OpenGL_Texture::Unbind(u32 slot)
	{
		glBindTextureUnit(slot, 0); //Untested
	}
}