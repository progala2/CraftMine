#pragma once

#include <stack>
#include "shaders.h"
#include "vecmatquat.h"

//////////////////////////////////////////////////////////////////////
// rozmiary bry³y obcinania
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
// po³o¿enie zmiennej jednorodnej modelViewProjectionMatrix
//////////////////////////////////////////////////////////////////////
GLint modelViewProjectionMatrixLoc;

//////////////////////////////////////////////////////////////////////
// wspólczynniki skalowania obiektu
//////////////////////////////////////////////////////////////////////
GLfloat scale = 1.0f;

//////////////////////////////////////////////////////////////////////
// k¹ty obrotu obiektu
//////////////////////////////////////////////////////////////////////
GLfloat rotateX = 0.0f;
GLfloat rotateY = 0.0f;

//////////////////////////////////////////////////////////////////////
// przesuniêcie obiektu
//////////////////////////////////////////////////////////////////////
GLfloat translateX = 0.0f;
GLfloat translateY = 0.0f;

//////////////////////////////////////////////////////////////////////
// identyfikator obiektu programu
//////////////////////////////////////////////////////////////////////
GLuint program;

//////////////////////////////////////////////////////////////////////
// identyfikator obiektu bufora z danymi tablicy
// wierzcho³ków - wspó³rzêdnymi wierzcho³ków krawêdzi obiektu
//////////////////////////////////////////////////////////////////////
GLuint vertexBuffer;

//////////////////////////////////////////////////////////////////////
// identyfikator obiektu tablic wierzcho³ków
//////////////////////////////////////////////////////////////////////
GLuint vertexArray;

//////////////////////////////////////////////////////////////////////
// wspó³rzêdne wierzcho³ków krawêdzi szeœcianu
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
// funkcja rysuj¹ca szeœcian
//////////////////////////////////////////////////////////////////////
void WireCube()
{
    // za³adowanie zmiennej jednorodnej - iloczynu macierzy modelu-widoku i rzutowania
    modelViewProjectionMatrix = projectionMatrix * modelViewMatrix.top();
    glUniformMatrix4fv( modelViewProjectionMatrixLoc, 1, GL_TRUE, modelViewProjectionMatrix.data() );

    // narysowanie danych zawartych w tablicach wierzcho³ków
    glDrawArrays( GL_LINES, 0, 24 );
}

//////////////////////////////////////////////////////////////////////
// funkcja rysuj¹ca blok 2x2
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
// funkcja rysuj¹ca blok 3x3
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
// funkcja rysuj¹ca piramidê z szeœcianów
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

    // pi¹ty poziom 1x1
    modelViewMatrix.push( modelViewMatrix.top() );
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, 6.0f );
    WireCube();
    modelViewMatrix.pop();
}

//////////////////////////////////////////////////////////////////////
// funkcja generuj¹ca scenê 3D
//////////////////////////////////////////////////////////////////////
void DisplayScene()
{
    // macierz modelu-widoku = macierz jednostkowa
    modelViewMatrix.top().LoadIdentity();

    // przesuniêcie uk³adu wspó³rzêdnych obiektu do œrodka bry³y obcinania
    modelViewMatrix.top().Translatef( 0.0f, 0.0f, -(near+far)/2.0f );

    // skalowanie obiektu
    modelViewMatrix.top().Scalef( scale, scale, scale );

    // przesuniêcie obiektu
    modelViewMatrix.top().Translatef( translateX, translateY, 0.0f );

    // obroty obiektu
    quaternion<> rotate( vector3<> ( 1.0f, 0.0f, 0.0f ), rotateX );
    rotate *= quaternion<> ( vector3<> ( 0.0f, 1.0f, 0.0f ), rotateY );
    modelViewMatrix.top() *= rotate;

    // czyszczenie bufora koloru
    glClear( GL_COLOR_BUFFER_BIT );

    // w³¹czenie obiektu tablic wierzcho³ków
    glBindVertexArray( vertexArray );

    // w³¹czenie shadera
    glUseProgram( program );

    // narysowanie piramidy z szeœcianów
    Pyramid();
	//WireCube();

    // wy³¹czenie shadera
    glUseProgram( 0 );

    // wy³¹czenie obiektu tablic wierzcho³ków
    glBindVertexArray( 0 );
}

//////////////////////////////////////////////////////////////////////
// zmiana wielkoœci okna
//////////////////////////////////////////////////////////////////////
void Reshape( int width, int height )
{
    // obszar renderingu - ca³e okno
    glViewport( 0, 0, width, height );

    // macierz rzutowania = macierz jednostkowa
    projectionMatrix.LoadIdentity();

    // parametry bry³y obcinania - rzutowanie perspektywiczne
    // wysokoœæ okna wiêksza od szerokoœci okna
    if( width < height && width > 0 )
         projectionMatrix.Frustum( left, right, bottom*height/width, top*height/width, near, far );
    else
        // szerokoœæ okna wiêksza lub równa wysokoœci okna
        if (width >= height && height > 0)
            projectionMatrix.Frustum( left*width/height, right*width/height, bottom, top, near, far );
        else
            projectionMatrix.Frustum( left, right, bottom, top, near, far );
}

//////////////////////////////////////////////////////////////////////
// inicjalizacja sta³ych elementów maszyny stanu OpenGL
//////////////////////////////////////////////////////////////////////
void InitScene()
{
    // kolor t³a - zawartoœæ bufora koloru
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	
    // wczytanie shaderów i przygotowanie obs³ugi programu
    AttachVertexShader( program, "Cube_vs.glsl" );
    AttachFragmentShader( program, "Cube_fs.glsl" );
    LinkProgram( program );

    // pobranie po³o¿enia zmiennej jednorodnej modelViewProjectionMatrix
    modelViewProjectionMatrixLoc = glGetUniformLocation( program, "modelViewProjectionMatrix" );

    // generowania identyfikatora obiektu tablic wierzcho³ków
    glGenVertexArrays( 1, &vertexArray );
	

    // utworzenie obiektu tablic wierzcho³ków
    glBindVertexArray( vertexArray );

    // generowanie identyfikatora obiektu bufora
    glGenBuffers( 1, &vertexBuffer );

    // utworzenie obiektu bufora wierzcho³ków (VBO) i za³adowanie danych
    glGenBuffers( 1, &vertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( position ), position, GL_STATIC_DRAW );
    GLuint positionLoc = glGetAttribLocation( program, "inPosition" );
    glVertexAttribPointer( positionLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    // w³¹czenie tablic wierzcho³ków
    glEnableVertexAttribArray( positionLoc );

    // wy³¹czenie obiektu tablic wierzcho³ków
    glBindVertexArray( 0 );

    // za³adowanie macierzy na szczyt stosu macierzy modelu-widoku
    matrix4x4<> matrix;
    modelViewMatrix.push( matrix );
}

//////////////////////////////////////////////////////////////////////
// usuniêcie obiektów OpenGL
//////////////////////////////////////////////////////////////////////
void DeleteScene()
{
    // porz¹dki
    glDeleteProgram( program );
    glDeleteBuffers( 1, &vertexBuffer );
    glDeleteVertexArrays( 1, &vertexArray );
}
