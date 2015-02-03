#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "Dna.h"
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
    void marriage();
    
    int mWin_w = 1920;
    int mWin_h = 1080;

    Surface16u mSur;
    Exporter mExp;
    
    vector<Dna> mDnas;
};

void gradientApp::setup(){
    
    setWindowPos(0, 0);
    setWindowSize(mWin_w, mWin_h);
    mExp.setup( mWin_w, mWin_h, 100, GL_RGBA16F_ARB, cs::getRenderPath(), 0);
#ifdef RENDER
    mExp.startRender();
#endif
    
    mSur = Surface16u( mWin_w, mWin_h, true);
    
    mDnas.assign( mWin_h, Dna() );
    marriage();
}

void gradientApp::marriage(){

    Perlin pln(4, 123);
    
    for( int i=0; i<mDnas.size(); i++ ){
        Dna & dna = mDnas[i];
        dna.setup( 3 );
        dna.setGene( 0, randFloat(0.3) + 0.1 );
        dna.setGene( 1, randFloat(0.3) + 0.1 );
        dna.setGene( 2, randFloat(0.3) + 0.1 );
        ColorAf c(dna.get(0), dna.get(1), dna.get(2) );
        cs::normalizeColor(c);
        mSur.setPixel( Vec2i(0,i), c );
    }
    
    int size = mDnas.size();
    int mNum = size/5;
    
    for (int gen=0; gen<mWin_w; gen++ ) {
        for( int i=0; i<mNum; i++ ){
            float ny = pln.fBm( gen*2.5 ) * 0.5;
            
            float brave = pln.noise(gen*0.5)+0.5 + randFloat(0.1,0.6);
            brave *= 0.01;
            
            int maleId = randInt(0, size);
            int femaleId = maleId + ny*size*brave;
            femaleId = MIN(femaleId, size-1);
            femaleId = MAX(femaleId, 0);
            
            Dna & male = mDnas[maleId];
            Dna & female = mDnas[femaleId];
            male.setMateMode( 1 );
            male.mate( female, 0.1 );
            
            ColorAf mc( male.get(0), male.get(1), male.get(2) );
            cs::normalizeColor(mc);
            mSur.setPixel( Vec2i(gen,maleId), mc );
            
            ColorAf fc( female.get(0), female.get(1), female.get(2) );
            cs::normalizeColor(fc);
            mSur.setPixel( Vec2i(gen,femaleId), fc );
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
