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
    
    void cropColor( ColorAf & col ){
        col.r = math<float>::clamp(col.r,0,1);
        col.g = math<float>::clamp(col.g,0,1);
        col.b = math<float>::clamp(col.b,0,1);
        col.a = math<float>::clamp(col.a,0,1);
    }
    
    void fillSurface( Surface16u sur, ColorAf col){

        Surface16u::Iter itr = sur.getIter();
        while (itr.line() ) {
            while( itr.pixel()){
                itr.r() = col.r;
                itr.g() = col.g;
                itr.b() = col.b;
                itr.a() = col.a;
            }
        }
    }
    
    float distanceToLine(Ray ray, Vec3f point){
        return cross(ray.getDirection(), point - ray.getOrigin()).length();
    }

}