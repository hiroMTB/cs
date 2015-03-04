#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "csUtil.h"
#include "Exporter.h"
#include "cinder/Easing.h"

//#define RENDER

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::function<float (float)> EaseFunc;

class gradientApp : public AppNative {
  public:
	void setup();
    void keyDown( KeyEvent event );
	void update();
	void draw();

    void linearGrad( Surface16u & sur, Ray line, float dist, ColorAf gcol, EaseFunc ease );
    void radialGrad( Surface16u & sur, Vec2f point, float radius, ColorAf gcol, EaseFunc ease=EaseNone() );
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    ColorAf background;
    Surface16u mSur;
    Exporter mExp;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    background.set(0, 0, 0, 0);
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
    
#ifdef RENDER
    mExp.startRender();
#endif
    
    mSur = Surface16u( mWin_w, mWin_h, true);
    cs::fillSurface(mSur, ColorAf(0,0,0,0) );
    
#pragma mark LINEAR_GRADIENT
    if(1){
        Vec3f origin(mWin_w*0.5, mWin_h*0.8,0);
        Vec3f dir(1,-1,0);
        Ray line( origin, dir);
        linearGrad( mSur, line, 800, ColorAf(0.1,0.3,0.7,1), EaseInOutQuad() );
    }
    
#pragma mark RADIAL_GRADIENT
    if(1){
        float diagonal = sqrt(mWin_w*mWin_w + mWin_h*mWin_h);
        radialGrad( mSur, Vec2f(0,0),            diagonal, ColorAf(0.1, 0.3, 0.7, 1), EaseInOutQuad() );
        radialGrad( mSur, Vec2f(mWin_w, mWin_h), diagonal, ColorAf(0.6, 0.2, 0.4, 1), EaseInOutQuad() );
    }
    
}

void gradientApp::linearGrad( Surface16u &sur, Ray line, float max_dist, ColorAf gcol, EaseFunc ease ){
    
    int w = sur.getWidth();
    int h = sur.getHeight();

    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++) {
            Vec2f pos(x, y);
            float dist = cs::distanceToLine(line, Vec3f(pos.x, pos.y, 0) );
            float t = dist/max_dist;
            t = math<float>::clamp(t);
            
            float easedVal = ease(t);
            float rate = 1.0 - easedVal;
            ColorAf col = sur.getPixel(pos);
            col += gcol*rate;
            cs::cropColor(col);
            sur.setPixel(pos, col);
        }
    }
}

void gradientApp::radialGrad( Surface16u &sur, Vec2f point, float radius, ColorAf gcol, EaseFunc ease){
    
    int w = sur.getWidth();
    int h = sur.getHeight();
    
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++) {
            Vec2f pos(x, y);
            float dist = point.distance(pos);
            float t = dist/radius;
            t = math<float>::clamp(t);
            float easedVal = ease(t);
            float rate = 1.0 - easedVal;
            ColorAf col = sur.getPixel(pos);
            col += gcol*rate;
            cs::cropColor(col);
            sur.setPixel(pos, col);
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
