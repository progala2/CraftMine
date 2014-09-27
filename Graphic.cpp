#pragma once

#include <stack>
#include "shaders.h"
#include "vecmatquat.h"

//////////////////////////////////////////////////////////////////////
// rozmiary bry�y obcinania
//////////////////////////////////////////////////////////////////////
GLfloat left = -10.0f;
GLfloat right = 10.0f;
GLfloat bottom = -10.0f;
GLfloat top = 10.0f;
GLfloat near = 50.0f;
GLfloat far = 70.0f;

//////////////////////////////////////////////////////////////////////
// macierz rzutowania
//////////////////////////////////////////////////////////////////////
matrix4x4<> projectionMatrix;

//////////////////////////////////////////////////////////////////////
// stos macierzy modelu-widoku
//////////////////////////////////////////////////////////////////////
std::stack< matrix4x4<> > modelViewMatrix;

//////////////////////////////////////////////////////////////////////
// iloczyn macierzy modelu-widoku i rzutowania
//////////////////////////////////////////////////////////////////////
matrix4x4<> modelViewProjectionMatrix;

//////////////////////////////////////////////////////////////////////
// po�o�enie zmiennej jednorodnej modelViewProjectionMatrix
//////////////////////////////////////////////////////////////////////
GLint modelViewProjectionMatrixLoc;

//////////////////////////////////////////////////////////////////////
// wsp�lczynniki skalowania obiektu
//////////////////////////////////////////////////////////////////////
GLfloat scale = 1.0f;

//////////////////////////////////////////////////////////////////////
// k�ty obrotu obiektu
//////////////////////////////////////////////////////////////////////
GLfloat rotateX = 0.0f;
GLfloat rotateY = 0.0f;

//////////////////////////////////////////////////////////////////////
// przesuni�cie obiektu
//////////////////////////////////////////////////////////////////////
GLfloat translateX = 0.0f;
GLfloat translateY = 0.0f;

//////////////////////////////////////////////////////////////////////
// identyfikator obiektu programu
//////////////////////////////////////////////////////////////////////
GLuint program;

//////////////////////////////////////////////////////////////////////
// identyfikator obiektu bufora z danymi tablicy
// wierzcho�k�w - wsp�rz�dnymi wierzcho�k�w kraw�dzi obiektu
//////////////////////////////////////////////////////////////////////
GLuint vertexBuffer;

//////////////////////////////////////////////////////////////////////
// identyfikator obiektu tablic wierzcho�k�w
//////////////////////////////////////////////////////////////////////
GLuint vertexArray;

//////////////////////////////////////////////////////////////////////
// wsp�rz�dne wierzcho�k�w kraw�dzi sze�cianu
//////////////////////////////////////////////////////////////////////
GLfloat position [3*24] =
{
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f
};

//////////////////////////////////////////////////////////////////////
// funkcja rysuj�ca sze�cian
//////////////////////////////////////////////////////////////////////
void WireCube()
{
    // za�adowanie zmiennej jednorodnej - iloczynu macierzy modelu-widoku i rzutowania
    modelViewProjectionMatrix = projectionMatrix * modelViewMatrix.top();
    glUniformMatrix4fv( modelViewProjectionMatrixLoc, 1, GL_TRUE, modelViewProjectionMatrix.data() );

    // narysowanie danych zawartych w tablicach wierzcho�k�w
    glDrawArrays( GL_LINES, 0, 24 );
}

//////////////////////////////////////////////////////////////////////
// funkcja rysuj�ca blok 2x2
//////////////////////////////////////////////////////////////////////
void Cube2x2()
{
    WireCube();
    modelViewMatrix.top().Translatef( 2.0f, 0.0f, 0.0f );
    WireCube();
    modelViewMatrix.top().Translatef( 0.0f, -2.0f, 0.0f );
    WireCube();
    modelViewMatrix.top().Translatef( -2.0f, 0.0f, 0.0f );
    WireCube();
}

//////////////////////////////////////////////////////////////////////
// funkcja rysuj�ca blok 3x3
//////////////////////////////////////////////////////////////////////
void Cube3x3()
{
    Cube2x2();
    modelViewMatrix.top().Translatef( -2.0f, 0.0f, 0.0f );
    WireCube();
    modelViewMatrix.top().Translatef( 0.0f, 2.0f, 0.0f );
    WireCube();
    modelViewMatrix.top().Translatef( 0.0f, 2.0f, 0.0f );
    WireCube();
    modelViewMatrix.top().Translatef( 2.0f, 0.0f, 0.0f );
    WireCube();
    modelViewMatrix.top().Translatef( 2.0f, 0.0f, 0.0f );
    WireCube();
}

//////////////////////////////////////////////////////////////////////
// funkcja rysuj�ca piramid� z sze�cian�w
//////////////////////////////////////////////////////////////////////
void Pyramid()
{
    // pierwszy poziom 5x5
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, -2.0f );
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 2.0f, 2.0f, 0.0f );
    Cube3x3();
    modelViewMatrix.pop();
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 2.0f, -2.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( -4.0f, 4.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( -2.0f, -2.0f, 0.0f );
    Cube3x3();
    modelViewMatrix.pop();
    modelViewMatrix.pop();

    // drugi poziom 4x4
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 1.0f,-1.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 1.0f, 3.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( -3.0f, 3.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( -3.0f, -1.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();

    // trzeci poziom 3x3
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, 2.0f );
    Cube3x3();
    modelViewMatrix.pop();

    // czwarty poziom 2x2
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, 4.0f );
    modelViewMatrix.top().Translatef( -1.0f, 1.0f, 0.0f );
    Cube2x2();
    modelViewMatrix.pop();

    // pi�ty poziom 1x1
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, 6.0f );
    WireCube();
    modelViewMatrix.pop();
}

//////////////////////////////////////////////////////////////////////
// funkcja generuj�ca scen� 3D
//////////////////////////////////////////////////////////////////////
void DisplayScene()
{
    // macierz modelu-widoku = macierz jednostkowa
    modelViewMatrix.top().LoadIdentity();

    // przesuni�cie uk�adu wsp�rz�dnych obiektu do �rodka bry�y obcinania
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, -(near+far)/2.0f );

    // skalowanie obiektu
    modelViewMatrix.top().Scalef( scale, scale, scale );

    // przesuni�cie obiektu
    modelViewMatrix.top().Translatef( translateX, translateY, 0.0f );

    // obroty obiektu
    quaternion<> rotate( vector3<> ( 1.0f, 0.0f, 0.0f ), rotateX );
    rotate *= quaternion<> ( vector3<> ( 0.0f, 1.0f, 0.0f ), rotateY );
    modelViewMatrix.top() *= rotate;

    // czyszczenie bufora koloru
    glClear( GL_COLOR_BUFFER_BIT );

    // w��czenie obiektu tablic wierzcho�k�w
    glBindVertexArray( vertexArray );

    // w��czenie shadera
    glUseProgram( program );

    // narysowanie piramidy z sze�cian�w
    Pyramid();
	//WireCube();

    // wy��czenie shadera
    glUseProgram( 0 );

    // wy��czenie obiektu tablic wierzcho�k�w
    glBindVertexArray( 0 );
}

//////////////////////////////////////////////////////////////////////
// zmiana wielko�ci okna
//////////////////////////////////////////////////////////////////////
void Reshape( int width, int height )
{
    // obszar renderingu - ca�e okno
    glViewport( 0, 0, width, height );

    // macierz rzutowania = macierz jednostkowa
    projectionMatrix.LoadIdentity();

    // parametry bry�y obcinania - rzutowanie perspektywiczne
    // wysoko�� okna wi�ksza od szeroko�ci okna
    if( width < height && width > 0 )
         projectionMatrix.Frustum( left, right, bottom*height/width, top*height/width, near, far );
    else
        // szeroko�� okna wi�ksza lub r�wna wysoko�ci okna
        if (width >= height && height > 0)
            projectionMatrix.Frustum( left*width/height, right*width/height, bottom, top, near, far );
        else
            projectionMatrix.Frustum( left, right, bottom, top, near, far );
}

//////////////////////////////////////////////////////////////////////
// inicjalizacja sta�ych element�w maszyny stanu OpenGL
//////////////////////////////////////////////////////////////////////
void InitScene()
{
    // kolor t�a - zawarto�� bufora koloru
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	
    // wczytanie shader�w i przygotowanie obs�ugi programu
    AttachVertexShader( program, "Cube_vs.glsl" );
    AttachFragmentShader( program, "Cube_fs.glsl" );
    LinkProgram( program );

    // pobranie po�o�enia zmiennej jednorodnej modelViewProjectionMatrix
    modelViewProjectionMatrixLoc = glGetUniformLocation( program, "modelViewProjectionMatrix" );

    // generowania identyfikatora obiektu tablic wierzcho�k�w
    glGenVertexArrays( 1, &vertexArray );
	

    // utworzenie obiektu tablic wierzcho�k�w
    glBindVertexArray( vertexArray );

    // generowanie identyfikatora obiektu bufora
    glGenBuffers( 1, &vertexBuffer );

    // utworzenie obiektu bufora wierzcho�k�w (VBO) i za�adowanie danych
    glGenBuffers( 1, &vertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( position ), position, GL_STATIC_DRAW );
    GLuint positionLoc = glGetAttribLocation( program, "inPosition" );
    glVertexAttribPointer( positionLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    // w��czenie tablic wierzcho�k�w
    glEnableVertexAttribArray( positionLoc );

    // wy��czenie obiektu tablic wierzcho�k�w
    glBindVertexArray( 0 );

    // za�adowanie macierzy na szczyt stosu macierzy modelu-widoku
    matrix4x4<> matrix;
    modelViewMatrix.push( matrix );
}

//////////////////////////////////////////////////////////////////////
// usuni�cie obiekt�w OpenGL
//////////////////////////////////////////////////////////////////////
void DeleteScene()
{
    // porz�dki
    glDeleteProgram( program );
    glDeleteBuffers( 1, &vertexBuffer );
    glDeleteVertexArrays( 1, &vertexArray );
}
