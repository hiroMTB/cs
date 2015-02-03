#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "csUtil.h"
#include "Exporter.h"
#include "StrangeEgent.h"

//#define RENDER

using namespace ci;
using namespace ci::app;
using namespace std;

class gradientApp : public AppNative {
  public:
	void setup();
    void keyDown( KeyEvent event );
	void update();
	void draw();
    void drawSA();
    void shuffule();
    void sortColor();
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    Surface16u mSur;
    Exporter mExp;
    
    StrangeAgent mSa;
    vector<Vec3f> mVs;
    gl::VboMeshRef mPoints;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
#ifdef RENDER
    mExp.startRender();
#endif
    
    mSur = Surface16u( mWin_w, mWin_h, true);

    // alloc
    mPoints = gl::VboMesh::create(mWin_w*mWin_h, 0, cs::getVboLayout(), GL_POINTS);
    mVs.assign(mWin_w*mWin_h, Vec3f(0,0,0) );

    shuffule();
}

void gradientApp::shuffule(){

    StrangeAgent &sa = mSa;
    sa.x = randFloat();
    sa.y = randFloat();
    sa.z = randFloat();
    sa.a = randFloat();
    sa.b = randFloat();
    sa.c = randFloat();
    sa.d = randFloat();
    sa.e = randFloat();
    sa.f = randFloat();
    sa.init();
    
    Vec3f aabb_max(-9999,-9999,-9999);
    Vec3f aabb_min(9999,9999,9999);
    
    for (int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            int index = x + y*mWin_w;
            mVs[index].set(sa.x, sa.y, sa.z);
            
            aabb_max.x = MAX(aabb_max.x, sa.x);
            aabb_max.y = MAX(aabb_max.y, sa.y);
            aabb_max.z = MAX(aabb_max.z, sa.z);
            aabb_min.x = MIN(aabb_min.x, sa.x);
            aabb_min.y = MIN(aabb_min.y, sa.y);
            aabb_min.z = MIN(aabb_min.z, sa.z);

            sa.next();
        }
    }
    
    Vec3f aabb_size = aabb_max - aabb_min;
    
    for (int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            Vec2i pos(x, y);
            int index = x + y*mWin_w;
            float r = (mVs[index].x-aabb_min.x) / aabb_size.x;
            float g = (mVs[index].y-aabb_min.y) / aabb_size.y;
            float b = (mVs[index].z-aabb_min.z) / aabb_size.z;
            mVs[index].set(r, g, b);

            ColorAf col(r, g, b, 1);
            col.r = MIN(col.r, 1);
            col.g = MIN(col.g, 1);
            col.b = MIN(col.b, 1);
            col.r = MAX(col.r, 0);
            col.g = MAX(col.g, 0);
            col.b = MAX(col.b, 0);
            mSur.setPixel( pos, col );
        }
    }
    
    // VboMesh
    gl::VboMesh::VertexIter itr = mPoints->mapVertexBuffer();
    int i=0;
    while(!itr.isDone() ){
        itr.setPosition( mVs[i] );
        itr.setColorRGBA( ColorA( mVs[i].x, mVs[i].y,mVs[i].z,0.5) );
        ++i;
        ++itr;
    }
    
    sortColor();
}

void gradientApp::sortColor(){

    // Collect all color
    vector<ColorAf> cols;
    for( int y=0; y<mWin_h; y++ ){
        for( int x=0; x<mWin_w; x++ ){
            ColorAf col = mSur.getPixel( Vec2i(x, y));
            cols.push_back(col);
        }
    }

    // Sort Blue first
    std::sort(cols.begin(), cols.end(), []( const ColorAf & a, const ColorAf & b) -> bool{
        return a.b > b.b;
    });

    std::sort(cols.begin(), cols.end(), []( const ColorAf & a, const ColorAf & b) -> bool{
        return a.g > b.g;
    });

    std::sort(cols.begin(), cols.end(), []( const ColorAf & a, const ColorAf & b) -> bool{
        return a.r > b.r;
    });
    
    // back to Surface
    

    for( int y=0; y<mWin_h; y++ ){
        for( int x=0; x<mWin_w; x++ ){
            int index = y + x*mWin_h;
            mSur.setPixel( Vec2i(x, y), cols[index] );
        }
    }
}

void gradientApp::update(){}

void gradientApp::draw(){
    
    gl::enableAlphaBlending();
    
    mExp.begin(); {
        gl::pushMatrices(); {
            gl::setMatricesWindow(mWin_w, mWin_h);
            gl::scale(1, -1);
            gl::translate(0, -mWin_h, 0);
            gl::clear();
            gl::draw( gl::Texture(mSur) );
        } gl::popMatrices();
    } mExp.end();

    mExp.draw();

    gl::pushMatrices(); {
        gl::translate( 40, mWin_h-40, 0);
        gl::scale(1,-1);
        gl::color(0, 0, 0);
        gl::drawStrokedRect( Rectf(0,0, 1000, 1000) );
        gl::scale(1000, 1000);
        glPointSize(1);
        gl::color(1, 1, 1);
        gl::draw(mPoints);
    }gl::popMatrices();
}


void gradientApp::keyDown( KeyEvent event ) {
    char key = event.getChar();
    switch (key) {
        case 'S':
            mExp.startRender();
            break;
        case 'T':
            mExp.stopRender();
            break;

        case ' ':
            mExp.snapShot();
            break;
            
        case 'f':
            shuffule();
            break;
    }
}

CINDER_APP_NATIVE( gradientApp, RendererGl(0) )
