#pragma once

//Загрузка OBJ-файлов

#include "ofMain.h"

//загрузить obj-файл
void kuLoadObjFile( ofMesh &mesh, string fileName, bool useTex,
                   bool setupNormals, bool normalize,
                   bool separateFaces, bool shuffle, int texW, int texH );

//установить нормали
void kuSetNormals( ofMesh &mesh );

//перемешать вершины и треугольники нормали
void kuMeshShuffle( vector<ofPoint> &v, vector<ofIndexType> &t, vector<ofVec2f> &tex,
                   bool useTex = true );

//-------------------------
struct MeshFace {
    int v[3];
    int a() { return v[0]; }
    int b() { return v[1]; }
    int c() { return v[2]; }
    MeshFace() {
        v[0] = v[1] = v[2] = 0;
    }
};

const int RenderModeWireFrame = 0;

//один объект
struct Mesh3D {
    void load( string objFileName );
    void boundBox( ofPoint &p0, ofPoint &p1 );
    
    vector<ofPoint> &getVRef() { return v; }
    vector<MeshFace> &getFRef() { return f; }
    
    void drawWireFrame();
    void drawWireFramePart( float part );
    void drawFacesPart( float part );
private:
    vector<ofPoint> v;
    vector<MeshFace> f;
};




//морфинг между несколькими объектами, и шум
struct kuObject3D {
    void setup();
    void addMesh3D( string objFileName );
    void normalize();
    void shuffle(); //перемешать треугольники
    
    void setMode( int renderMode );
    //0 - точки
    //1 - вершины
    //2 - треугольники
    
    void draw();
    void drawPart( float part );    //0..1 - рисовать не все треугольники
    
    //веса для морфинга
    void updateWeights( const vector<float> &wgh );
    
private:
    vector<Mesh3D> _mesh;    
    
    Mesh3D _m;      //сетка для рисования, морфинг
    int mode_;
};


//Несколько 3D-объектов
struct kuObjects3D {
    void setup();
    void addObject( string objFileName );
    void normalize();
    void shuffle();
    
    void drawPart( float part, int mode, float alpha );
    void selectObject( int i );
    int size() { return obj_.size(); }
private:
    vector<kuObject3D> obj_;
    int i_; //выбранный
    
};







