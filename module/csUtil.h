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
    
    void normalizeColor( ColorAf & col ){
        col.r = MIN(col.r, 1);
        col.g = MIN(col.g, 1);
        col.b = MIN(col.b, 1);
        col.a = MIN(col.a, 1);

        col.r = MAX(0, col.r);
        col.g = MAX(0, col.g);
        col.b = MAX(0, col.b);
        col.a = MAX(0, col.a);
    }
}