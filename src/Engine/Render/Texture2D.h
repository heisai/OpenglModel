#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <QtOpenGL/QOpenGLFunctions_4_5_Core>
#include <QPointer>

class QOpenGLContext;

class Texture2D : protected QOpenGLFunctions_4_5_Core
{
public:
	Texture2D() = default;
	~Texture2D();

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	bool loadFromFile(const std::filesystem::path& path, bool srgb, std::string& error);
	bool release() noexcept;
	bool bind(GLenum textureUnit);
	bool isLoadedForCurrentContext() const noexcept;
	bool loadFailedForCurrentContext() const noexcept;
	const std::optional<std::filesystem::path>& sourcePath() const noexcept;

private:
	GLuint texture_id_ = 0;
	QPointer<QOpenGLContext> context_;
	QPointer<QOpenGLContext> last_attempt_context_;
	std::optional<std::filesystem::path> source_path_;
	bool load_failed_ = false;
};