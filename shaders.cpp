//////////////////////////////////////////////////////////////////////
// (c) Janusz Ganczarski
// http://www.januszg.hg.pl
// JanuszG@enter.net.pl
//////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "shaders.h"

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera z pliku i jego kompilacja,
// rodzaj shadera okreœla parametr type; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadShader( const GLenum type, const char *name )
{
    // odczyt kodu shadera do bufora
    std::ifstream file;
    file.open( name, std::ios::binary );

    // sprawdzenie poprawnoœci odczytu pliku
    if( file.bad() )
    {
        std::cout << "Niepoprawny odczyt pliku shadera " << name << std::endl;
        exit( 0 );
    }

    // sprawdzenie d³ugoœci plinu
    file.seekg( 0, std::ios::end );
    int len = static_cast<int>( file.tellg() );
    if( len <= 0 )
    {
        std::cout << "Niepoprawny odczyt pliku shadera " << name << std::endl;
        exit( 0 );
    }

    // w³aœciwy odczyt pliku
    file.seekg( 0, std::ios::beg );
    GLchar *srcBuf = new GLchar[(len + 1) * sizeof( GLchar )];
    file.read( srcBuf, len );
    srcBuf[len] = '\0';
    file.close();

    // utworzenie obiektu shadera
    GLuint shader = glCreateShader( type );

    // za³adowanie kodu do obiektu shadera
    glShaderSource( shader, 1, const_cast<const GLchar**>( &srcBuf ), NULL );

    // porz¹dki
    delete[] srcBuf;

    // kompilacja shadera
    glCompileShader( shader );

    // sprawdzenie statusu kompilacji shadera
    GLint status;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        std::cout << "Niepoprawna kompilacja shadera " << name << std::endl;

        // pobranie i wyœwietlenie komunikatu b³êdu
        GLint logLength;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetShaderInfoLog( shader, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }

    // zwrócenie identyfikatora obiektu shadera
    return shader;
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera i jego kompilacja,
// rodzaj shadera okreœla parametr type; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadShader( const GLenum type, const char **src )
{
    // utworzenie obiektu shadera
    GLuint shader = glCreateShader( type );

    // za³adowanie kodu do obiektu shadera
    glShaderSource( shader, 1, src, NULL );

    // kompilacja shadera
    glCompileShader( shader );

    // sprawdzenie statusu kompilacji shadera
    GLint status;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        std::cout << "Niepoprawna kompilacja shadera" << std::endl;

        // pobranie i wyœwietlenie komunikatu b³êdu
        GLint logLength;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetShaderInfoLog( shader, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }

    // zwrócenie identyfikatora obiektu shadera
    return shader;
}

//////////////////////////////////////////////////////////////////////
// wy³¹czenie obs³ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera kontroli teselacji z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessControlShader( const char *name )
{
    return LoadShader( GL_TESS_CONTROL_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera kontroli teselacji
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessControlShader( const char **src )
{
    return LoadShader( GL_TESS_CONTROL_SHADER, src );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera ewaluacji teselacji z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessEvaluationShader( const char *name )
{
    return LoadShader( GL_TESS_EVALUATION_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera ewaluacji teselacji
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessEvaluationShader( const char **src )
{
    return LoadShader( GL_TESS_EVALUATION_SHADER, src );
}

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera wierzcho³ków z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadVertexShader( const char *name )
{
    return LoadShader( GL_VERTEX_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera wierzcho³ków
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadVertexShader( const char **src )
{
    return LoadShader( GL_VERTEX_SHADER, src );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera fragmentów z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadFragmentShader( const char *name )
{
    return LoadShader( GL_FRAGMENT_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera fragmentów
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadFragmentShader( const char **src )
{
    return LoadShader( GL_FRAGMENT_SHADER, src );
}

//////////////////////////////////////////////////////////////////////
// wy³¹czenie obs³ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera geometrii z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadGeometryShader( const char *name )
{
    return LoadShader( GL_GEOMETRY_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za³adowanie kodu Ÿród³owego shadera geometrii
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadGeometryShader( const char **src )
{
    return LoadShader( GL_GEOMETRY_SHADER, src );
}

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// powi¹zanie obiektu shadera z obiektem programu;
// w razie potrzeby tworzony jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachShader( GLuint &program, const GLuint shader )
{
    // w razie potrzeby tworzymy obiekt programu
    if( !glIsProgram( program ) )
        program = glCreateProgram();

    // do³¹czenie obiektu shadera do obiektu programu
    glAttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera wierzcho³ków, reprezentowanego przez kod
// Ÿród³owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachVertexShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadVertexShader( name );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera wierzcho³ków, reprezentowanego przez kod
// Ÿród³owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachVertexShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadVertexShader( src );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera fragmentów, reprezentowanego przez kod
// Ÿród³owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachFragmentShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadFragmentShader( name );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera fragmentów, reprezentowanego przez kod
// Ÿród³owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachFragmentShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadFragmentShader( src );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// wy³¹czenie obs³ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera geometrii, reprezentowanego przez kod
// Ÿród³owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachGeometryShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadGeometryShader( name );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera geometrii, reprezentowanego przez kod
// Ÿród³owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachGeometryShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadGeometryShader( src );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera kontroli teselacji, reprezentowanego przez kod
// Ÿród³owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessControlShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadTessControlShader( name );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera kontroli teselacji, reprezentowanego przez kod
// Ÿród³owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessControlShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadTessControlShader( src );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera ewaluacji teselacji, reprezentowanego przez kod
// Ÿród³owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessEvaluationShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadTessEvaluationShader( name );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi¹zanie shadera ewaluacji teselacji, reprezentowanego przez kod
// Ÿród³owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessEvaluationShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadTessEvaluationShader( src );

    // do³¹czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// konsolidacja i walidacja obiektu programu
//////////////////////////////////////////////////////////////////////
void LinkProgram( GLuint program )
{
    // konsolidacja shaderów
    glLinkProgram( program );

    // sprawdzenie poprawnoœci konsolidacji obiektu programu
    GLint status;
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( status == GL_FALSE )
    {
        std::cout << "Niepoprawna konsolidacja programu" << std::endl;

        // pobranie i wyœwietlenie komunikatu b³êdu
        GLint logLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetProgramInfoLog( program, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }

    // walidacja shaderów
    glValidateProgram( program );

    // sprawdzenie poprawnoœci walidacji obiektu programu
    glGetProgramiv( program, GL_VALIDATE_STATUS, &status );
    if( status == GL_FALSE )
    {
        std::cout << "Niepoprawna walidacja programu" << std::endl;

        // pobranie i wyœwietlenie komunikatu b³êdu
        GLint logLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetProgramInfoLog( program, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }
}
