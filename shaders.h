//////////////////////////////////////////////////////////////////////
// (c) Janusz Ganczarski
// http://www.januszg.hg.pl
// JanuszG@enter.net.pl
//////////////////////////////////////////////////////////////////////

#ifndef __SHADERS__H__
#define __SHADERS__H__
#include <GL\glew.h>


//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera z pliku i jego kompilacja,
// rodzaj shadera okre�la parametr type; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadShader( const GLenum type, const char *name );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera i jego kompilacja,
// rodzaj shadera okre�la parametr type; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadShader( const GLenum type, const char **src );

//////////////////////////////////////////////////////////////////////
// wy��czenie obs�ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera kontroli teselacji z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessControlShader( const char *name );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera kontroli teselacji
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessControlShader( const char **src );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera ewaluacji teselacji z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessEvaluationShader( const char *name );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera ewaluacji teselacji
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessEvaluationShader( const char **src );

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera wierzcho�k�w z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadVertexShader( const char *name );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera wierzcho�k�w
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadVertexShader( const char **src );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera fragment�w z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadFragmentShader( const char *name );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera fragment�w
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadFragmentShader( const char **src );

//////////////////////////////////////////////////////////////////////
// wy��czenie obs�ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera geometrii z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadGeometryShader( const char *name );

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera geometrii
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadGeometryShader( const char **src );

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// powi�zanie obiektu shadera z obiektem programu; 
// w razie potrzeby tworzony jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachShader( GLuint &program, const GLuint shader );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera wierzcho�k�w, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachVertexShader( GLuint &program, const char *name );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera wierzcho�k�w, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachVertexShader( GLuint &program, const char **src );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera fragment�w, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachFragmentShader( GLuint &program, const char *name );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera fragment�w, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachFragmentShader( GLuint &program, const char **src );

//////////////////////////////////////////////////////////////////////
// wy��czenie obs�ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera geometrii, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachGeometryShader( GLuint &program, const char *name );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera geometrii, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachGeometryShader( GLuint &program, const char **src );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera kontroli teselacji, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessControlShader( GLuint &program, const char *name );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera kontroli teselacji, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessControlShader( GLuint &program, const char **src );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera ewaluacji teselacji, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessEvaluationShader( GLuint &program, const char *name );

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera ewaluacji teselacji, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessEvaluationShader( GLuint &program, const char **src );

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// konsolidacja i walidacja obiektu programu
//////////////////////////////////////////////////////////////////////
void LinkProgram( GLuint program );

#endif // __SHADERS__H__
