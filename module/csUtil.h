//
//  cs_util.h
//  gradient
//
//  Created by MatobaHiroshi on 2/1/15.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::function<float (float)> EaseFunc;

namespace cs {
    
    gl::VboMesh::Layout getVboLayout(){
        gl::VboMesh::Layout layout;
        layout.setDynamicColorsRGBA();
        layout.setDynamicPositions();
        layout.setStaticIndices();
        return layout;
    }
    
    
    string getTimeStamp(){        
        time_t curr;
        tm local;
        time(&curr);
        local =*(localtime(&curr));
        int yday = local.tm_yday;
        int hours = local.tm_hour;
        int min = local.tm_min;
        int sec = local.tm_sec;
        
        char stamp[16];
        sprintf(stamp, "%03d_%02d%02d_%02d", yday, hours, min, sec);
        return string(stamp);
    }
    
    string getTimeStampU(){
        return toString( time(NULL) );
    }
 
    
    void renderScreen( fs::path path, int frame ){
        string frame_name = "f_" + toString( frame ) + ".png";
        writeImage( path/frame_name, copyWindowSurface() );
        cout << "Render Image : " << frame << endl;
    }
    
    fs::path getRenderPath(){
        return expandPath("../../../_render/")/getTimeStamp();
    }
    
    
    
    float distanceToLine(Ray ray, Vec3f point){
        return cross(ray.getDirection(), point - ray.getOrigin()).length();
    }
    
    void clampColor( ColorAf & col ){
        col.r = math<float>::clamp(col.r,0,1);
        col.g = math<float>::clamp(col.g,0,1);
        col.b = math<float>::clamp(col.b,0,1);
        col.a = math<float>::clamp(col.a,0,1);
    }
    
    void linearGrad( Surface16u &sur, Ray line, float max_dist, ColorAf colA, ColorAf colB, EaseFunc ease ){
        
        Surface16u::Iter itr = sur.getIter();
        
        while (itr.line()) {
            while (itr.pixel()) {
                
                Vec2i pos = itr.getPos();
                float dist = distanceToLine(line, Vec3f(pos.x, pos.y, 0) );
                float t = dist/max_dist;
                t = math<float>::clamp(t);
                float rate = ease(t);
                ColorAf col = sur.getPixel(pos); // cs::getColorFromItr(itr);
                ColorAf adder = colA*(1.0-rate) + colB*rate;
                col += adder;
                clampColor(col);
                sur.setPixel(pos, col);
            }
        }
    }
    
    void radialGrad( Surface16u &sur, Vec2f point, float radius, ColorAf colA, ColorAf colB, EaseFunc ease){
        
        Surface16u::Iter itr = sur.getIter();
        
        while (itr.line()) {
            while (itr.pixel()) {
                Vec2i pos = itr.getPos();
                float dist = point.distance(pos);
                float t = dist/radius;
                t = math<float>::clamp(t);
                float rate = ease(t);
                ColorAf col = sur.getPixel(pos); // cs::getColorFromItr(itr);
                ColorAf adder = colA*(1.0-rate) + colB*rate;
                col += adder;
                //col.normalize();
                clampColor(col);
                sur.setPixel(pos, col);
            }
        }
    }

    void ellipseGrad( Surface16u &sur, Vec2f point1, Vec2f point2, float radius, ColorAf colA, ColorAf colB, EaseFunc ease){
        
        Surface16u::Iter itr = sur.getIter();
        
        while (itr.line()) {
            while (itr.pixel()) {
                Vec2i pos = itr.getPos();
                
                float dist1 = point1.distance(pos);
                float dist2 = point2.distance(pos);
                float dist = dist1 + dist2;
                float t = dist/radius;
                t = math<float>::clamp(t);
                float rate = ease(t);
                ColorAf col = sur.getPixel(pos); // cs::getColorFromItr(itr);
                ColorAf adder = colA*(1.0-rate) + colB*rate;
                col += adder;
                clampColor(col);
                sur.setPixel(pos, col);
            }
        }
    }
    
    void mixColor( ColorAf & colA, const ColorAf & colB ){
        colA.g -= colB.r;
        colA.b -= colB.r;
        colA.r -= colB.g;
        colA.b -= colB.g;
        colA.r -= colB.b;
        colA.b -= colB.b;
    }
    
    
    void fillSurface( Surface16u & sur, const ColorAf & col){

        Surface16u::Iter itr = sur.getIter();
        while (itr.line() ) {
            while( itr.pixel()){
                //setColorToItr( itr, col );
                sur.setPixel(itr.getPos(), col);
            }
        }
    }
  
    // this does work but strange behavior !!
    /*
     inline ColorAf getColorFromItr( const Surface16u::Iter & itr ){
     return ColorAf(itr.r(), itr.g(), itr.b(), itr.a() );
     }
     */
    
    // this does not work!!
    /*
     inline void setColorToItr( Surface16u::Iter & itr, const ColorAf & col ){
     itr.r() = col.r;
     itr.g() = col.g;
     itr.b() = col.b;
     itr.a() = col.a;
     }
     */
    
 }