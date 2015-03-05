#include "cinder/Cinder.h"
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"
#include "cinder/Matrix.h"
#include "cinder/Vector.h"
#include "cinder/Quaternion.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "csUtil.h"
#include "Exporter.h"
#include "cinder/Easing.h"

#define RENDER

using namespace ci;
using namespace ci::app;
using namespace std;

class gradientApp : public AppNative {
  public:
	void setup();
    void keyDown( KeyEvent event );
	void update();
	void draw();
    void makeGradient( float frame );
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    ColorAf background;
    Surface16u mSur;
    Exporter mExp;
    
    Perlin pln;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    background.set(0, 0, 0, 1);
    mExp.setup( mWin_w, mWin_h, 1000, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
    
#ifdef RENDER
    mExp.startRender();
#endif
    
    mSur = Surface16u( mWin_w, mWin_h, true);
    cs::fillSurface(mSur, ColorAf(0,0,0,0) );
    
    pln.setOctaves(4);
    pln.setSeed(123);
}

void gradientApp::update(){
    
    cs::fillSurface(mSur, ColorAf(0,0,0,0));
    
    float frame = getElapsedFrames();
    makeGradient( frame );
}

void gradientApp::makeGradient( float frame ){
    
#pragma mark LINEAR_GRADIENT
    if(0){
        Vec3f dir(1, 0, 0);
        dir.rotate(Vec3f(0,0,1), frame*0.01);
        Ray line1( Vec3f(mWin_w*0.4,0,0), dir);
        Ray line2( Vec3f(mWin_w*0.7,0,0), dir);
        Ray line3( Vec3f(mWin_w*1.0,0,0), dir);
        cs::linearGrad( mSur, line1, mWin_w*1, ColorAf(0,0.2,0.3,1), ColorAf(0.1,0,0.2,1), EaseInOutQuad() );
        cs::linearGrad( mSur, line2, mWin_w*0.5, ColorAf(0.1,0.3,0.2,1), ColorAf(0.3,0.1,0,1), EaseInOutQuad() );
        cs::linearGrad( mSur, line3, mWin_w*0.7, ColorAf(0.2,0,0.1,1), ColorAf(0.1,0.2,0.3,1),  EaseInOutQuad() );
    }
    
#pragma mark RADIAL_GRADIENT
    if(1){
        float diagonal = Vec2i(mWin_w, mWin_h).length();
        cs::radialGrad( mSur, Vec2f(0,0),                   diagonal*frame*0.005, ColorAf(0.1, 0.3, 0.5, 1), ColorAf(0.3, 0.2, 0, 1), EaseInOutQuad() );
        cs::radialGrad( mSur, Vec2f(mWin_w, mWin_h),        diagonal*(1-frame*0.005), ColorAf(0.3, 0.1, 0.2, 1), ColorAf(0.1, 0.3, 0.1, 1), EaseInOutQuad() );
        cs::radialGrad( mSur, Vec2f(mWin_w*0.7-frame*3 , mWin_h*0.5),diagonal*0.6, ColorAf(0.1+frame*0.005, 0.1, 0, 1), ColorAf(0, 0.2, 0.4, 1), EaseInOutQuad() );
        cs::radialGrad( mSur, Vec2f(mWin_w*0.45 - frame*2, mWin_h*0.8 + frame*3),diagonal*0.6, ColorAf(0, 0.3, 0, 1), ColorAf(0.2, 0.1, 0.4, 1), EaseInOutQuad());
    }
}

void gradientApp::draw(){
    
    gl::enableAlphaBlending();
    
    mExp.begin(); {
        gl::pushMatrices(); {
            gl::setMatricesWindow(mWin_w, mWin_h);
            gl::scale(1, -1);
            gl::translate(0, -mWin_h, 0);
            gl::clear(background);
            gl::draw(gl::Texture(mSur));
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
