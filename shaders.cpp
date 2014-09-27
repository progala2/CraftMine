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
// za�adowanie kodu �r�d�owego shadera z pliku i jego kompilacja,
// rodzaj shadera okre�la parametr type; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadShader( const GLenum type, const char *name )
{
    // odczyt kodu shadera do bufora
    std::ifstream file;
    file.open( name, std::ios::binary );

    // sprawdzenie poprawno�ci odczytu pliku
    if( file.bad() )
    {
        std::cout << "Niepoprawny odczyt pliku shadera " << name << std::endl;
        exit( 0 );
    }

    // sprawdzenie d�ugo�ci plinu
    file.seekg( 0, std::ios::end );
    int len = static_cast<int>( file.tellg() );
    if( len <= 0 )
    {
        std::cout << "Niepoprawny odczyt pliku shadera " << name << std::endl;
        exit( 0 );
    }

    // w�a�ciwy odczyt pliku
    file.seekg( 0, std::ios::beg );
    GLchar *srcBuf = new GLchar[(len + 1) * sizeof( GLchar )];
    file.read( srcBuf, len );
    srcBuf[len] = '\0';
    file.close();

    // utworzenie obiektu shadera
    GLuint shader = glCreateShader( type );

    // za�adowanie kodu do obiektu shadera
    glShaderSource( shader, 1, const_cast<const GLchar**>( &srcBuf ), NULL );

    // porz�dki
    delete[] srcBuf;

    // kompilacja shadera
    glCompileShader( shader );

    // sprawdzenie statusu kompilacji shadera
    GLint status;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        std::cout << "Niepoprawna kompilacja shadera " << name << std::endl;

        // pobranie i wy�wietlenie komunikatu b��du
        GLint logLength;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetShaderInfoLog( shader, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }

    // zwr�cenie identyfikatora obiektu shadera
    return shader;
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera i jego kompilacja,
// rodzaj shadera okre�la parametr type; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadShader( const GLenum type, const char **src )
{
    // utworzenie obiektu shadera
    GLuint shader = glCreateShader( type );

    // za�adowanie kodu do obiektu shadera
    glShaderSource( shader, 1, src, NULL );

    // kompilacja shadera
    glCompileShader( shader );

    // sprawdzenie statusu kompilacji shadera
    GLint status;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        std::cout << "Niepoprawna kompilacja shadera" << std::endl;

        // pobranie i wy�wietlenie komunikatu b��du
        GLint logLength;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetShaderInfoLog( shader, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }

    // zwr�cenie identyfikatora obiektu shadera
    return shader;
}

//////////////////////////////////////////////////////////////////////
// wy��czenie obs�ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera kontroli teselacji z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessControlShader( const char *name )
{
    return LoadShader( GL_TESS_CONTROL_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera kontroli teselacji
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessControlShader( const char **src )
{
    return LoadShader( GL_TESS_CONTROL_SHADER, src );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera ewaluacji teselacji z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessEvaluationShader( const char *name )
{
    return LoadShader( GL_TESS_EVALUATION_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera ewaluacji teselacji
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadTessEvaluationShader( const char **src )
{
    return LoadShader( GL_TESS_EVALUATION_SHADER, src );
}

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera wierzcho�k�w z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadVertexShader( const char *name )
{
    return LoadShader( GL_VERTEX_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera wierzcho�k�w
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadVertexShader( const char **src )
{
    return LoadShader( GL_VERTEX_SHADER, src );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera fragment�w z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadFragmentShader( const char *name )
{
    return LoadShader( GL_FRAGMENT_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera fragment�w
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadFragmentShader( const char **src )
{
    return LoadShader( GL_FRAGMENT_SHADER, src );
}

//////////////////////////////////////////////////////////////////////
// wy��czenie obs�ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera geometrii z pliku
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadGeometryShader( const char *name )
{
    return LoadShader( GL_GEOMETRY_SHADER, name );
}

//////////////////////////////////////////////////////////////////////
// za�adowanie kodu �r�d�owego shadera geometrii
// i jego kompilacja; funkcja zwraca identyfikator
// obiektu shadera ze skompilowanym shaderem
//////////////////////////////////////////////////////////////////////
GLuint LoadGeometryShader( const char **src )
{
    return LoadShader( GL_GEOMETRY_SHADER, src );
}

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// powi�zanie obiektu shadera z obiektem programu;
// w razie potrzeby tworzony jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachShader( GLuint &program, const GLuint shader )
{
    // w razie potrzeby tworzymy obiekt programu
    if( !glIsProgram( program ) )
        program = glCreateProgram();

    // do��czenie obiektu shadera do obiektu programu
    glAttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera wierzcho�k�w, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachVertexShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadVertexShader( name );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera wierzcho�k�w, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachVertexShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadVertexShader( src );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera fragment�w, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachFragmentShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadFragmentShader( name );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera fragment�w, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachFragmentShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadFragmentShader( src );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// wy��czenie obs�ugi w przypadku OpenGL ES 2.0
//////////////////////////////////////////////////////////////////////
#if !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera geometrii, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachGeometryShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadGeometryShader( name );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera geometrii, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachGeometryShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadGeometryShader( src );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera kontroli teselacji, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessControlShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadTessControlShader( name );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera kontroli teselacji, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessControlShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadTessControlShader( src );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera ewaluacji teselacji, reprezentowanego przez kod
// �r�d�owy w pliku, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessEvaluationShader( GLuint &program, const char *name )
{
    // odczyt kodu shadera z pliku i jego kompilacja
    GLuint shader = LoadTessEvaluationShader( name );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

//////////////////////////////////////////////////////////////////////
// powi�zanie shadera ewaluacji teselacji, reprezentowanego przez kod
// �r�d�owy, z obiektem programu; w razie potrzeby tworzony
// jest nowy obiekt programu
//////////////////////////////////////////////////////////////////////
void AttachTessEvaluationShader( GLuint &program, const char **src )
{
    // odczyt kodu shadera i jego kompilacja
    GLuint shader = LoadTessEvaluationShader( src );

    // do��czenie obiektu shadera do obiektu programu
    AttachShader( program, shader );
}

#endif // !( defined GL_ES_VERSION_2_0 || defined CONTEXT_ES2_PROFILE )

//////////////////////////////////////////////////////////////////////
// konsolidacja i walidacja obiektu programu
//////////////////////////////////////////////////////////////////////
void LinkProgram( GLuint program )
{
    // konsolidacja shader�w
    glLinkProgram( program );

    // sprawdzenie poprawno�ci konsolidacji obiektu programu
    GLint status;
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( status == GL_FALSE )
    {
        std::cout << "Niepoprawna konsolidacja programu" << std::endl;

        // pobranie i wy�wietlenie komunikatu b��du
        GLint logLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetProgramInfoLog( program, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }

    // walidacja shader�w
    glValidateProgram( program );

    // sprawdzenie poprawno�ci walidacji obiektu programu
    glGetProgramiv( program, GL_VALIDATE_STATUS, &status );
    if( status == GL_FALSE )
    {
        std::cout << "Niepoprawna walidacja programu" << std::endl;

        // pobranie i wy�wietlenie komunikatu b��du
        GLint logLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetProgramInfoLog( program, logLength, NULL, log );
        std::cout << log << std::endl;
        delete[] log;
        exit( 0 );
    }
}
