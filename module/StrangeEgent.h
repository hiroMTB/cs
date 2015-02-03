//
//  StrangeEgent.h
//  gradient_3
//
//  Created by MatobaHiroshi on 2/3/15.
//
//

#pragma once

class StrangeAgent{
    
public:
    
    StrangeAgent(){}
    
    void init(){
        for (int i=0; i<100; i++) {
            next();
        }
    }
    
    void next(){
        float newx = z*sin(a*x) + cos(b*y);
        float newy = x*sin(c*y) + cos(d*z);
        float newz = y*sin(e*z) + cos(f*z);
        x = newx;
        y = newy;
        z = newz;
    }

    void operator*=( float mp){
        x *= mp;
        y *= mp;
        z *= mp;
    }
    
    float x, y, z;
    float a, b, c, d, e, f;
    
};