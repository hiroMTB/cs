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
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    Surface16u mSur;
    
    Exporter mExp;
    
    vector<StrangeAgent> mSAs;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
#ifdef RENDER
    mExp.startRender();
#endif
    
    mSur = Surface16u( mWin_w, mWin_h, true);
    shuffule();
}

void gradientApp::shuffule(){

    {
        mSAs.clear();
        Perlin pln(32, randFloat(0,100) );
        int st = randFloat() * 100;
        float x, y, z, a, b, c, d, e, f;
        x=y=z=a=b=c=d=e=f=0;
        for (int j=0; j<mWin_h; j++) {
            float i = j + st;
            float rRate = 0.001;
            StrangeAgent sa;
            sa.x = x+=pln.noise(1, i);
            sa.y = y+=pln.noise(2, i);
            sa.z = z+=pln.noise(3, i);
            sa.a = a+=pln.noise( i*10 )       + randFloat(rRate);
            sa.b = b+=pln.noise( i*0.01 )   + randFloat(rRate);
            sa.c = c+=pln.noise( i*0.001 )  + randFloat(rRate);
            sa.d = d+=pln.fBm( i *12)         + randFloat(rRate);
            sa.e = e+=pln.fBm( i*0.01 )     + randFloat(rRate);
            sa.f = f+=pln.fBm( i*0.001 )    + randFloat(rRate);
            sa.init();
            mSAs.push_back(sa);
        }
    }
    
    int nextNum = randInt(1, 10);
    
    for (int y=0; y<mWin_h; y++){
        StrangeAgent & sa = mSAs[y];

        float r = 0;
        float g = 0;
        float b = 0;

        for (int x=0; x<mWin_w; x++) {
            Vec2i pos(x, y);
            
            r += sa.x*0.001;
            g += sa.y*0.001;
            b += sa.z*0.001;
            
            ColorAf col(1, 1, 1, 1);
            col.g -= (1-r);
            col.b -= (1-r);

            col.r -= (1-g) * 0.7;
            col.b -= (1-g) * 0.7;

//            col.r -= (1-b) * 0.5;
//            col.g -= (1-b) * 0.5;

            col.r = MIN(col.r, 1);
            col.g = MIN(col.g, 1);
            col.b = MIN(col.b, 1);
            col.r = MAX(col.r, 0);
            col.g = MAX(col.g, 0);
            col.b = MAX(col.b, 0);

            mSur.setPixel( pos, col );
            
            for (int i=0; i<nextNum; i++) {
                sa.next();
            }
        }
    }
}

void gradientApp::update(){}

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

        case ' ':
            mExp.snapShot();
            break;
            
        case 'f':
            shuffule();
            break;
    }
}

CINDER_APP_NATIVE( gradientApp, RendererGl(0) )
