#ifndef sb_gl_detail_shader_support_h
#define sb_gl_detail_shader_support_h

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

// depending on mode GL vs GLES:
#if !defined(ANDROID)
# include <GL/glew.h>
# include <string.h>  // memset()
#endif

#include <iostream>
#include <sstream>
#include <memory>


namespace sb {
namespace gl {

#if defined(USING_ALOG)
# include "alog.h"
#else
inline void alog(const std::string&, const std::string&)
{
   ;
}
#endif


inline GLuint link_program( GLuint vertex_shader, GLuint fragment_shader,  const std::vector<std::string>& attribute_name_list,
      std::string* error_string )
{
   alog( "link_program","entry");

  // Link the program
   GLuint progID = glCreateProgram();
   glAttachShader(progID, vertex_shader);
   glAttachShader(progID, fragment_shader);

   // add teh attributes
   for( GLuint i = 0; i < attribute_name_list.size(); ++i)
      glBindAttribLocation(progID, i, attribute_name_list[i].c_str());

   glLinkProgram(progID);

   // Check the program
   GLint result = GL_TRUE;
   glGetProgramiv(progID, GL_LINK_STATUS, &result);
   if( result == GL_FALSE)
   {
      glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &result);
      if ( result > 1 )
      {
         alog( "link_program","retrieving log info");

         std::unique_ptr<char[]> temp( new char[result] );
         memset(temp.get(),0,result);

         GLsizei rv_len;
         glGetProgramInfoLog(progID, result, &rv_len, temp.get());
         std::stringstream ss;
         ss << "error: " << temp.get();
         alog("link_program",ss.str());
         alog("link_program", "*** MEMORY LEAK? ***" );
         if( error_string )
            *error_string = ss.str();
      }
      alog( "link_program","error exit");
      return 0;  // remove, this is a memory leak
   }
   //   glDeleteShader(vsID);
   //   glDeleteShader(fsID);

   alog( "link_program","successful exit");
   return progID;
}


inline GLuint load_shader( const std::string& shader_code, const GLenum& shader_type, std::string* error_string)
{
   alog( "load_shader","entry");

   // Compile the shader
   GLuint shader_id = glCreateShader(shader_type);
   if( !shader_id )
      alog( "load_shader", "bad shader type");
   GLint sz = shader_code.size();
   const char* chars = shader_code.c_str();
   glShaderSource( shader_id, 1, &chars, &sz);
   glCompileShader(shader_id);

   // Check Vertex Shader
   GLint result = GL_FALSE;
   glGetShaderiv( shader_id, GL_COMPILE_STATUS, &result);
   if( result == GL_FALSE )
   {
      alog( "load_shader","retrieving log length");
      glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &result);
      if( result > 1 )
      {
         alog( "load_shader","retrieving log info");

         std::unique_ptr<char[]> temp( new char[result] );
         memset(temp.get(),0,result);
         GLsizei rv_len;
         glGetShaderInfoLog(shader_id, result, &rv_len, temp.get());
         std::stringstream ss;
         ss << "error: " << temp.get();
         alog("load_shader",ss.str());
         if(error_string)
            *error_string = ss.str();
         glDeleteShader(shader_id);
      }
      alog( "load_shader","error exit");
      return 0;
   }

   alog( "load_shader","successful exit");

   return shader_id;
}


}  // namespace gl
}  // namespace sb

#endif
