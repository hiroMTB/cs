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

class cApp : public AppNative {
  public:
	void setup();
    void keyDown( KeyEvent event );
	void update();
	void draw();
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    Surface32f mSur;
    Perlin mPln;
    Exporter mExp;
};

void cApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    
    mPln.setOctaves(16);

    
    mSur = Surface32f( mWin_w, mWin_h, true);
    
    for (int x=0; x<mWin_w; x++) {
        for (int y=0; y<mWin_h; y++){
            Vec2i pos(x, y);
            ColorAf col( 1, 1, 1, 1);
            mSur.setPixel( pos, col );
        }
    }
    
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);

#ifdef RENDER
    mExp.startRender();
#endif
}


void cApp::update(){
    float frame = getElapsedFrames();
}

void cApp::draw(){
    
    gl::enableAlphaBlending();
    mExp.begin(); {
        gl::setMatricesWindow(mWin_w, mWin_h);
        gl::scale(1, -1);
        gl::translate(0, -mWin_h, 0);
        gl::clear();
        gl::draw( gl::Texture(mSur) );
    } mExp.end();
    
    mExp.draw();
}


void cApp::keyDown( KeyEvent event ) {
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

CINDER_APP_NATIVE( cApp, RendererGl(0) )
