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

    void addGrad( float x, float y, float radius, ColorAf gcol);
    
    int mWin_w = 1280;
    int mWin_h = 720;

    Surface16u mSur;
    Exporter mExp;
    
    void paintToWhite( ColorAf & cA, const ColorAf & cB );
    
};


void gradientApp::paintToWhite( ColorAf &cA, const ColorAf & cB){
    
    // paint red
    cA.g -= cB.r;
    cA.b -= cB.r;
    
    // blue
    cA.r -= cB.b;
    cA.g -= cB.b;
    
    // green
    cA.r -= cB.g;
    cA.b -= cB.g;
}

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
#ifdef RENDER
    mExp.startRender();
#endif
    
    mSur = Surface16u( mWin_w, mWin_h, true);
    
    // set white
    for (int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            Vec2i pos(x, y);
            mSur.setPixel(pos, ColorAf(1,1,1,1) );
        }
    }
    
    addGrad( 200, 200, 400, ColorAf(0.7, 0.1, 0.1, 1) );
    addGrad( 1000, 500, 1000, ColorAf(0.1, 0.7, 0.3, 1) );
}

void gradientApp::addGrad(float x, float y, float radius, ColorAf gcol){
    
    Vec2f point(x, y);
    for (int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            Vec2i pos(x, y);
            float dist = point.distance(pos);
            if( dist <= radius ){
                float rate = 1.0 - (dist*dist)/(radius*radius);
                if(0<=rate && rate <=1.0){
                    ColorAf col = mSur.getPixel(pos);
                    paintToWhite(col, gcol*rate);
                    mSur.setPixel(pos, col);
                }
            }
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
    }
}

CINDER_APP_NATIVE( gradientApp, RendererGl(0) )
