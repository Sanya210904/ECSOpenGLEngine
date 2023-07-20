#include <utils/Debug.hpp>

#include <iostream>
#include <assert.h>

std::string GLErrorToString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR:                      return "GL_NO_ERROR";
	case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
	case 0x8031: /* not core */            return "GL_TABLE_TOO_LARGE_EXT";
	case 0x8065: /* not core */            return "GL_TEXTURE_TOO_LARGE_EXT";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default:
		assert(!"Unhandled GL error code");
		return NULL;
	}
}

std::string GLFrameBufferStatus(GLenum error)
{
	switch (error)
	{
	case GL_FRAMEBUFFER_UNDEFINED:                      return "GL_FRAMEBUFFER_UNDEFINED";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:          return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
	case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
	default:
		assert(!"Unhandled GL error code");
		return NULL;
	}
}

GLenum checkError_(const char* file, int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "0x" << std::hex << error << " - " << GLErrorToString(error) << " | " << file << " (" << std::dec << line << ")" << std::endl;
	return error;
}

void glDebugOutput(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei lenght,
	const char* message,
	const void* userParam)
{
	if (id == 131169
		|| id == 131185
		|| id == 131218
		|| id == 131204)
		return;

	std::cout << "==============================" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	std::cout << "Source: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				std::cout << "API";				break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		std::cout << "Window System";	break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	std::cout << "Shader Compiler";	break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		std::cout << "Third Party";		break;
	case GL_DEBUG_SOURCE_APPLICATION:		std::cout << "Application";		break;
	case GL_DEBUG_SOURCE_OTHER:				std::cout << "Other";			break;
	}
	std::cout << std::endl;

	std::cout << "Type: ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				std::cout << "Error";				break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	std::cout << "Deprecated Behavior";	break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	std::cout << "Undefined Behavior";	break;
	case GL_DEBUG_TYPE_PORTABILITY:			std::cout << "Portability";			break;
	case GL_DEBUG_TYPE_PERFORMANCE:			std::cout << "Performance";			break;
	case GL_DEBUG_TYPE_MARKER:				std::cout << "Marker";				break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			std::cout << "Push Group";			break;
	case GL_DEBUG_TYPE_POP_GROUP:			std::cout << "Pop Group";			break;
	case GL_DEBUG_TYPE_OTHER:				std::cout << "Other";				break;
	}
	std::cout << std::endl;
	
	std::cout << "Severity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			std::cout << "High";			break;
	case GL_DEBUG_SEVERITY_MEDIUM:			std::cout << "Medium";			break;
	case GL_DEBUG_SEVERITY_LOW:				std::cout << "Low";				break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	std::cout << "Notification";	break;
	}
	std::cout << std::endl << std::endl;
}
