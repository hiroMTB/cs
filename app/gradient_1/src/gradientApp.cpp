#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "csUtil.h"
#include "Exporter.h"

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
    
    int mWin_w = 1920/2;
    int mWin_h = 1080/2;
    gl::VboMeshRef mPoints;
    Perlin mPln;
    
    Exporter mExp;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
     
    gl::VboMesh::Layout layout = cs::getVboLayout();
    mPoints = gl::VboMesh::create( mWin_w*mWin_h, 0, layout, GL_POINTS );
    
    gl::VboMesh::VertexIter itr = mPoints->mapVertexBuffer();

    for (int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            itr.setPosition( Vec3f(x, y, 0) );
            itr.setColorRGBA( ColorA( (float)x/mWin_w, (float)y/mWin_h, 1, 1) );
            ++itr;
        }
    }
    
    mExp.setup( mWin_w, mWin_h, 100, GL_RGB16F_ARB, cs::getRenderPath(), 0);
}


void gradientApp::update(){
    float frame = getElapsedFrames();
    
    gl::VboMesh::VertexIter itr = mPoints->mapVertexBuffer();
    
    for (int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            
            //Vec3f * p = itr.getPositionPointer();
            ColorA * c = itr.getColorPointer();

            //Vec3f n = mPln.dfBm(x, y, frame);
            Vec3f n = mPln.dnoise(x, y, frame);
            n *= 0.01;
            c->r += n.x;
            c->g += n.y;
            c->b += n.z;
            
            //itr.setPosition( Vec3f(x, y, 0) );
            //itr.setColorRGBA( ColorA( (float)x/mWin_w, (float)y/mWin_h, 1) );
            ++itr;
        }
    }
}

void gradientApp::draw(){
    
    gl::enableAlphaBlending();
    gl::enableAlphaTest();
    
    mExp.begin(); {
        gl::setMatricesWindow(mWin_w, mWin_h);
        gl::scale(1, -1);
        gl::translate(0, -mWin_h, 0);
        gl::clear();
        gl::color(1, 1, 1);
        gl::drawSolidRect( Rectf(10, 10, 500, 500 ) );
        gl::draw(mPoints);
    } mExp.end();
    
    mExp.draw();
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
            
    }
}

CINDER_APP_NATIVE( gradientApp, RendererGl(0) )
