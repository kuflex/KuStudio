#include "kuObject3D.h"
#include "pbFiles.h"


//--------------------------------------------------------
//загрузить obj-файл
void kuLoadObjFile( ofMesh &mesh, string fileName, bool useTex,
                   bool setupNormals, bool normalize,
                   bool separateFaces, bool shuffle, int texW, int texH ) {
    cout << "loading " << fileName << endl;
    mesh.clear();
    vector<string> lines = pbFiles::readStrings( ofToDataPath(fileName) );
    
    vector<ofPoint> v;      //вершины
    vector<ofIndexType> t;  //индексы треугольников, тройками
    vector<ofVec2f> tex;    //текстурные координаты
    
    ofPoint p;
    int f[4];
    
    for (int i=0; i<lines.size(); i++) {
        vector<string> list = ofSplitString( lines[i], " ", true, true );
        int n = list.size();
        if( n > 0 ) {
            if ( list[0] == "v" && n >= 4 ) {
                v.push_back( ofPoint(
                                     ofToFloat( list[1] ),
                                     ofToFloat( list[2] ),
                                     ofToFloat( list[3] )
                                     ) );
            }
            if ( useTex && list[0] == "vt" && n >= 3 ) {
                tex.push_back( ofVec2f(
                                       ofToFloat(list[1]) * texW,
                                       ofToFloat(list[2]) * texH
                                       ) );
            }
            if ( list[0] == "f" && n >= 4 ) {
                int N = min(n-1,4);
                for (int j=0; j<N; j++) {
                    vector<string> line = ofSplitString( list[j+1], "/", true, true );
                    if ( line.size() > 0 ) {
                        f[j] = ofToInt( line[0] ) - 1;
                        
                    }
                }
                t.push_back( f[0] );
                t.push_back( f[1] );
                t.push_back( f[2] );
                //4-угольная грань
                if ( n >= 5 ) {
                    t.push_back(f[0]);
                    t.push_back(f[2]);
                    t.push_back(f[3]);
                }
            }
            
        }
    }
    
    //нормализация в куб [-1,1]x[-1,1]x[-1,1]
    if ( normalize ) {
        ofPoint p0 = ofPoint( 0 );
        ofPoint p1 = p0;
        if ( v.size() > 0 ) {
            p0 = v[0];
            p1 = p0;
            for (int i=0; i<v.size(); i++) {
                ofPoint &p = v[i];
                p0.x = min( p0.x, p.x );
                p0.y = min( p0.y, p.y );
                p0.z = min( p0.z, p.z );
                p1.x = max( p1.x, p.x );
                p1.y = max( p1.y, p.y );
                p1.z = max( p1.z, p.z );
            }
        }
        ofPoint c = ( p0 + p1 ) * 0.5;
        ofPoint delta = p1 - p0;
        float scl = delta.x;
        scl = max( scl, delta.y );
        scl = max( scl, delta.z );
        if ( scl > 0 ) {
            scl = 1.0 / scl;
        }
        for (int j=0; j<v.size(); j++) {
            ofPoint &p = v[j];
            p = (p - c) * scl;
        }
    }

    //перемешивание
    if ( shuffle ) {
        kuMeshShuffle( v, t, tex, useTex );
    }
        
    //установка вершин
    mesh.addVertices( v );
    
    //установка текстурных координат
    if ( useTex ) {
        if (tex.size() >= v.size() ) {
            mesh.addTexCoords( tex );
        }
        else {
            cout << "Error in OBJ model, not enough texture coords" << endl;
        }
    }
    
    //установка треугольников
    mesh.addIndices(t);

    //нормали
    if ( setupNormals ) { kuSetNormals( mesh ); }
    
}

//--------------------------------------------------------
//перемешать вершины и треугольники нормали
void kuMeshShuffle( vector<ofPoint> &v, vector<ofIndexType> &t, vector<ofVec2f> &tex,
                   bool useTex ) {

    int n = v.size();
    vector<int> vto( n );
    vector<int> vfrom( n );
    for (int i=0; i<n; i++) {
        vto[i] = i;
        vfrom[i] = i;
    }
    //перемешивание вершин
    int cnt = n * 2;
    for (int k=0; k<cnt; k++) {
        int i = ofRandom( 0, n );
        i = min( i, n-1 );
        int j = ofRandom( 0, n );
        j = min( j, n-1 );
        if ( i != j ) {
            swap( vfrom[vto[i]], vfrom[vto[j]] );
            swap( vto[i], vto[j] );
        }
    }
    vector<ofPoint> v1 = v;
    for (int i=0; i<n; i++) {
        v[i] = v1[vto[i]];
    }
    if ( useTex && tex.size() >= n ) {
        vector<ofVec2f> tex1 = tex;
        for (int i=0; i<n; i++) {
            tex[i] = tex1[vto[i]];
        }
    }
    //подстройка треугольников
    vector<ofIndexType> t1 = t;
    for (int i=0; i<t.size(); i++) {
        t[i] = vfrom[t1[i]];
    }
    
    //перемешивание треугольников
    int T = t.size() / 3;
    int count = T * 2;
    for (int k=0; k<count; k++) {
        int i = ofRandom( 0, T );
        i = min( i, T-1 );
        int j = ofRandom( 0, T );
        j = min( j, T-1 );
        if ( i != j ) {
            int a = i * 3;
            int b = j * 3;
            swap( t[a], t[b] );
            swap( t[a+1], t[b+1] );
            swap( t[a+2], t[b+2] );
        }
    }
}


//--------------------------------------------------------
//установить нормали
void kuSetNormals( ofMesh &mesh ) {
    //The number of the vertices
	int nV = mesh.getNumVertices();
	
	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;
    
	vector<ofPoint> norm( nV ); //Array for the normals
    
	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
	for (int t=0; t<nT; t++) {
        
		//Get indices of the triangle t
		int i1 = mesh.getIndex( 3 * t );
		int i2 = mesh.getIndex( 3 * t + 1 );
		int i3 = mesh.getIndex( 3 * t + 2 );
		
		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex( i1 );
		const ofPoint &v2 = mesh.getVertex( i2 );
		const ofPoint &v3 = mesh.getVertex( i3 );
		
		//Compute the triangle's normal
		ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
		
		//Accumulate it to norm array for i1, i2, i3
		norm[ i1 ] += dir;
		norm[ i2 ] += dir;
		norm[ i3 ] += dir;
	}
    
	//Normalize the normal's length
	for (int i=0; i<nV; i++) {
		norm[i].normalize();
	}
    
	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals( norm );
}

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
void Mesh3D::load( string objFileName )
{
    v.clear();
    f.clear();
    vector<string> lines = pbFiles::readStrings( ofToDataPath(objFileName) );
    for (int i=0; i<lines.size(); i++) {
        vector<string> list = ofSplitString( lines[i], " " );
        int n = list.size();
        if( n > 0 ) {
            if ( list[0] == "v" && n >= 4 ) {
                ofPoint p;
                p.x = ofToFloat( list[1] );
                p.y = ofToFloat( list[2] );
                p.z = ofToFloat( list[3] );
                v.push_back( p );
            }
            if ( list[0] == "f" && n >= 4 ) {
                MeshFace t;
                for (int j=0; j<3; j++) {
                    vector<string> line = ofSplitString( list[j+1], "/" );
                    if ( line.size() > 0 ) {
                        t.v[j] = ofToFloat( line[0] ) - 1;
                    }
                }
                f.push_back( t );
            }
            
        }
    }

}

//--------------------------------------------------------
void Mesh3D::boundBox( ofPoint &p0, ofPoint &p1 )
{
    p0 = ofPoint( 0 );
    p1 = p0;
    if ( v.size() > 0 ) {
        p0 = v[0];
        p1 = p0;
        for (int i=0; i<v.size(); i++) {
            ofPoint &p = v[i];
            p0.x = min( p0.x, p.x );
            p0.y = min( p0.y, p.y );
            p0.z = min( p0.z, p.z );
            p1.x = max( p1.x, p.x );
            p1.y = max( p1.y, p.y );
            p1.z = max( p1.z, p.z );
        }
    }
}

//--------------------------------------------------------
void Mesh3D::drawWireFrame()
{
    drawWireFramePart( 1 );
}

//--------------------------------------------------------
void Mesh3D::drawWireFramePart( float part ) {
    ofNoFill();
    part = ofClamp( part, 0, 1 );
    int n = f.size() * part;
    
    for (int i=0; i<n; i++) {
        MeshFace &fc = f[i];
        /*
         //оптимизация для случая, когда рисуется вся фигура - чтобы два раза линии не рисовать
         int j1, a, b;
        for (int j=0; j<3; j++){
            j1 = (j+1) % 3;
            a = fc.v[j];
            b = fc.v[j1];
            if ( a < b ) {
                ofLine( v[ a ], v[ b ] );
            }
        }*/
        //треугольниками
        ofTriangle( v[fc.a()], v[fc.b()], v[fc.c()] );
    }
}

//--------------------------------------------------------
void Mesh3D::drawFacesPart( float part ) {
    ofFill();
    part = ofClamp( part, 0, 1 );
    int n = f.size() * part;
    
    for (int i=0; i<n; i++) {
        MeshFace &fc = f[i];
        ofTriangle( v[fc.a()], v[fc.b()], v[fc.c()] );
    }
}


//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
void kuObject3D::setup()
{
    _mesh.clear();
    mode_ = 1;
}

//--------------------------------------------------------
void kuObject3D::addMesh3D( string objFileName )
{
    _mesh.push_back( Mesh3D() );
    _mesh[ _mesh.size() - 1 ].load( objFileName );
    
    if ( _mesh.size() == 1 ) {
        _m = _mesh[0];  //инициализируем результирующую сетку
    }
    
}

//--------------------------------------------------------
void kuObject3D::normalize()
{
    ofPoint p0( 100000, 100000, 100000 );
    ofPoint p1 = -p0;
    for (int i=0; i<_mesh.size(); i++) {
        ofPoint p0_, p1_;
        _mesh[i].boundBox( p0_, p1_ );
        p0.x = min( p0.x, p0_.x );
        p0.y = min( p0.y, p0_.y );
        p0.z = min( p0.z, p0_.z );
        p1.x = max( p1.x, p1_.x );
        p1.y = max( p1.y, p1_.y );
        p1.z = max( p1.z, p1_.z );
    }
    ofPoint c = ( p0 + p1 ) * 0.5;
    ofPoint delta = p1 - p0;
    float scl = delta.x;
    scl = max( scl, delta.y );
    scl = max( scl, delta.z );
    if ( scl > 0 ) {
        scl = 1.0 / scl;
    }
    for (int i=0; i<_mesh.size(); i++) {
        vector<ofPoint> &v = _mesh[i].getVRef();
        for (int j=0; j<v.size(); j++) {
            ofPoint &p = v[j];
            p = (p - c) * scl;
        }
    }
    if ( _mesh.size() > 0 ) {
        _m = _mesh[0];
    }
    
}

//--------------------------------------------------------
void kuObject3D::shuffle() { //перемешать треугольники
    if ( _mesh.empty() ) return;
    
    int T = _mesh[0].getFRef().size();
    int count = T*2;
    for (int k=0; k<count; k++) {
        int i = ofRandom( 0, T );
        i = min( i, T-1 );
        int j = ofRandom( 0, T );
        j = min( j, T-1 );
        if ( i != j ) {
            MeshFace temp;
            for (int q=0; q<_mesh.size(); q++) {        //???
                vector<MeshFace> &f = _mesh[q].getFRef();
                temp = f[i];
                f[i] = f[j];
                f[j] = temp;
                //swap( f[i], f[j] );
            }
        }
    }
    if ( _mesh.size() > 0 ) {
        _m = _mesh[0];
    }
}

//--------------------------------------------------------
void kuObject3D::setMode( int renderMode )
{
    mode_ = renderMode;
}

//--------------------------------------------------------
void kuObject3D::draw()
{
    drawPart(1);
    
}

//--------------------------------------------------------
void kuObject3D::drawPart( float part ) {    //0..1 - рисовать не все треугольники
    if ( mode_ == 0 ){
        //точки
    }
    if ( mode_ == 1 ) {
        _m.drawWireFramePart( part );
    }
    if ( mode_ == 2 ) {
        _m.drawFacesPart( part );
    }
}

//--------------------------------------------------------
void kuObject3D::updateWeights( const vector<float> &wgh )
{
    vector<float> w = wgh;
    float sum = 0;
    for (int i=0; i<w.size(); i++) {
        sum += w[i];
    }
    if ( sum > 0 ) {
        for (int i=0; i<w.size(); i++) {
            w[i] /= sum;
        }
    }
    
    //морфинг
    int n = min( w.size(), _mesh.size() );
    vector<ofPoint> &v = _m.getVRef();
    int N = v.size();
    for (int i=0; i<N; i++) {
        v[i] = ofPoint(0, 0, 0);
    }
    for (int i=0; i<n; i++) {
        vector<ofPoint> &v1 = _mesh[i].getVRef();
        for (int j=0; j<N; j++) {
            v[j] += v1[j] * w[i];
        }
    }
    
}

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
void kuObjects3D::setup() {
    i_ = -1;

}

//--------------------------------------------------------
void kuObjects3D::addObject( string objFileName ) {
    obj_.push_back( kuObject3D() );
    kuObject3D &obj = obj_[obj_.size()-1];
    obj.setup();
    obj.addMesh3D( objFileName );
    if ( i_ == -1 ) {
        i_ = 0;
    }
}

//--------------------------------------------------------
void kuObjects3D::normalize() {
    for (int i=0; i<obj_.size(); i++) {
        obj_[i].normalize();
    }
}

//--------------------------------------------------------
void kuObjects3D::shuffle() {
    for (int i=0; i<obj_.size(); i++) {
        obj_[i].shuffle();
    }
}
    
//--------------------------------------------------------
void kuObjects3D::drawPart( float part, int mode, float alpha ) {
    ofSetColor( 255, 255 * alpha );
    if ( i_ >= 0 && i_ < obj_.size() ) {
        kuObject3D &obj = obj_[i_];
        obj.setMode( mode );
        obj.drawPart( part );
    }
}

//--------------------------------------------------------
void kuObjects3D::selectObject( int i ) {
    i_ = i;    
}

//--------------------------------------------------------


