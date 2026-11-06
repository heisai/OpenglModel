#include "Texture2D.h"
#include "../Renderables/stb_image.h"
#include <algorithm>
#include <cstdio>
#include <memory>
#include <vector>
#include <QOpenGLContext>
#include <QDebug>
#include <QString>

namespace
{
	std::string pathForMessage(const std::filesystem::path& path)
	{
#if defined(_WIN32)
		const QByteArray utf8 = QString::fromStdWString(path.wstring()).toUtf8();
		return std::string(utf8.constData(), static_cast<std::size_t>(utf8.size()));
#else
		return path.string();
#endif
	}
}

Texture2D::~Texture2D()
{
	release();
}

bool Texture2D::loadFromFile(const std::filesystem::path& path, bool srgb, std::string& error)
{
	error.clear();
	QOpenGLContext* currentContext = QOpenGLContext::currentContext();
	source_path_ = path;
	last_attempt_context_ = currentContext;
	load_failed_ = true;

	if (texture_id_ != 0)
	{
		error = "Release the existing texture before loading another image";
		return false;
	}
	if (!currentContext)
	{
		error = "Cannot load a texture without a current OpenGL context";
		return false;
	}
	if (!initializeOpenGLFunctions())
	{
		error = "Failed to initialize OpenGL functions for texture loading";
		return false;
	}

	FILE* rawFile = nullptr;
#if defined(_WIN32)
	if (_wfopen_s(&rawFile, path.c_str(), L"rb") != 0)
	{
		error = "Cannot open texture image: " + pathForMessage(path);
		return false;
	}
#else
	rawFile = std::fopen(path.string().c_str(), "rb");
	if (!rawFile)
	{
		error = "Cannot open texture image: " + pathForMessage(path);
		return false;
	}
#endif
	std::unique_ptr<FILE, int(*)(FILE*)> file(rawFile, &std::fclose);

	int width = 0;
	int height = 0;
	int sourceChannels = 0;
	std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> pixels(
		stbi_load_from_file(file.get(), &width, &height, &sourceChannels, STBI_rgb_alpha),
		&stbi_image_free);
	if (!pixels)
	{
		const char* reason = stbi_failure_reason();
		error = "Cannot decode texture image " + pathForMessage(path);
		if (reason)
		{
			error += ": ";
			error += reason;
		}
		return false;
	}
	if (width <= 0 || height <= 0)
	{
		error = "Texture image has invalid dimensions: " + pathForMessage(path);
		return false;
	}
	GLint maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	if (width > maxTextureSize || height > maxTextureSize)
	{
		error = "Texture image exceeds the OpenGL size limit: " + pathForMessage(path);
		return false;
	}

	const std::size_t rowBytes = static_cast<std::size_t>(width) * STBI_rgb_alpha;
	std::vector<stbi_uc> row(rowBytes);
	for (int y = 0; y < height / 2; ++y)
	{
		stbi_uc* top = pixels.get() + static_cast<std::size_t>(y) * rowBytes;
		stbi_uc* bottom = pixels.get() + static_cast<std::size_t>(height - 1 - y) * rowBytes;
		std::copy_n(top, rowBytes, row.data());
		std::copy_n(bottom, rowBytes, top);
		std::copy(row.data(), row.data() + rowBytes, bottom);
	}

	GLint previousActiveTexture = GL_TEXTURE0;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &previousActiveTexture);
	glActiveTexture(GL_TEXTURE0);
	GLint previousTextureBinding = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTextureBinding);

	GLuint newTexture = 0;
	glGenTextures(1, &newTexture);
	if (newTexture == 0)
	{
		glActiveTexture(static_cast<GLenum>(previousActiveTexture));
		error = "OpenGL failed to allocate a texture for: " + pathForMessage(path);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, newTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8,
		width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());
	GLint allocatedWidth = 0;
	GLint allocatedHeight = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &allocatedWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &allocatedHeight);
	if (allocatedWidth != width || allocatedHeight != height)
	{
		glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previousTextureBinding));
		glDeleteTextures(1, &newTexture);
		glActiveTexture(static_cast<GLenum>(previousActiveTexture));
		error = "OpenGL failed to allocate texture storage for: " + pathForMessage(path);
		return false;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previousTextureBinding));
	glActiveTexture(static_cast<GLenum>(previousActiveTexture));

	texture_id_ = newTexture;
	context_ = currentContext;
	load_failed_ = false;
	return true;
}

bool Texture2D::release() noexcept
{
	if (texture_id_ != 0)
	{
		if (context_.isNull())
		{
			texture_id_ = 0;
		}
		else if (QOpenGLContext::currentContext() != context_.data())
		{
			qWarning() << "Texture2D must be released while its owning OpenGL context is current";
			return false;
		}
		else
		{
			glDeleteTextures(1, &texture_id_);
			texture_id_ = 0;
		}
	}

	context_.clear();
	last_attempt_context_.clear();
	source_path_.reset();
	load_failed_ = false;
	return true;
}

bool Texture2D::bind(GLenum textureUnit)
{
	if (!isLoadedForCurrentContext())
	{
		return false;
	}
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	return true;
}

bool Texture2D::isLoadedForCurrentContext() const noexcept
{
	return texture_id_ != 0 && !context_.isNull() && QOpenGLContext::currentContext() == context_.data();
}

bool Texture2D::loadFailedForCurrentContext() const noexcept
{
	return load_failed_ && last_attempt_context_.data() == QOpenGLContext::currentContext();
}

const std::optional<std::filesystem::path>& Texture2D::sourcePath() const noexcept
{
	return source_path_;
}