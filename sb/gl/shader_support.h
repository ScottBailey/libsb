#ifndef sb_gl_shader_support_h
#define sb_gl_shader_support_h

/*
   Copyright (c) 2014, Scott Bailey
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of the <organization> nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL SCOTT BAILEY BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if defined(ANDROID)
# include <GLES2/gl2.h>
//# include <GLES3/gl3.h>
#else
# include <GL/gl.h>
#endif

#include <vector>
#include <string>


namespace sb {
namespace gl {

/// load a shader and return it's id
/// @param shader_code  The shader's source code as a single string
/// @param shader_type  The type of shader to create (i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// @param error_string  Pointer to storage for the error string that will be populated if an error occurs
/// @return 0 on error, otherwise the shader id of the loaded shader
GLuint load_shader(const std::string& shader_code, const GLenum& shader_type, std::string* error_string);

/// create a program, attach shader's, bind attribute names, and perform the link
/// @param vertex_shader  The vertex shader to use
/// @param fragment_shader  The fragment shader to use
/// @param attribute_name_list  The
/// @param error_string  Pointer to storage for the error string that will be populated if an error occurs
/// @return 0 on error, otherwise the program id of the linked program
GLuint link_program( GLuint vertex_shader, GLuint fragment_shader,
      const std::vector<std::string>& attribute_name_list, std::string* error_string );

} // namespace gl
} // namespace sb

#include <sb/gl/detail/shader_support.ipp>

#endif
