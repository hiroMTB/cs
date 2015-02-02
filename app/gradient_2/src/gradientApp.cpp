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
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    Surface32f mSur;
    Perlin mPln;
    Exporter mExp;
    
    vector<Vec3f> speed;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    speed.assign( mWin_w*mWin_h, Vec3f(0,0,0) );
    
    mSur = Surface32f( mWin_w, mWin_h, true);
    
    for (int x=0; x<mWin_w; x++) {
        for (int y=0; y<mWin_h; y++){
            Vec2i pos(x, y);
            float rate = (float)x/mWin_w;
            ColorAf col( 0.2, rate, rate, 1);
            //ColorAf col(1,1,1,1);
            mSur.setPixel( pos, col );
        }
    }
    
    
    /*
     *      We need to apply small PR for 16 bit image rendering
     *      See here http://goo.gl/TAjPbX
     */
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
    //mExp.setup( mWin_w, mWin_h, 100, GL_RGBA, 0);
    mExp.startRender();
    
    mPln.setOctaves(16);
}


void gradientApp::update(){
    //return;
    float frame = getElapsedFrames();
    int index = 0;
    for ( int y=0; y<mWin_h; y++){
        for (int x=0; x<mWin_w; x++) {
            Vec2i pos(x,y);
            ColorAf c =mSur.getPixel( pos );

            Vec3f n1 = mPln.dfBm( c.r+c.g, c.b, frame );
            Vec3f n2 = mPln.dfBm(x, y, frame);
            //Vec3f n = mPln.dnoise(x, y, frame);
            n1 *= 0.025;
            n2 *= 0.001;
            speed[index] += n2;
            
            c.r += n1.x;
            c.g += n1.y;
            c.b += n1.z;

            c.r += speed[index].x;
            c.g += speed[index].y;
            c.b += speed[index].z;
            speed[index] *= 0.9;
            mSur.setPixel( pos, c);
            
            index++;
        }
    }
}

void gradientApp::draw(){
    
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
