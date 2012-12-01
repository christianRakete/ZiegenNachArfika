//
//  Particle.h
//  ZiegenNachAfrika
//
//  Created by Christian Latsch on 12/1/12.
//
//

#ifndef ZiegenNachAfrika_Particle_h
#define ZiegenNachAfrika_Particle_h

#include "cinder/Color.h"
#include <Box2D/Box2d.h>

    
class Particle {
public:
    Particle();
    ~Particle();
    
    // pass in a pointer to the particle
    void setup(ci::Vec2f boxSize);
    void update();
    void draw();
    
    //private:
    // store a pointer to the particle in the physics world from the main app
    b2Body* body;
    ci::Color color;
    ci::Vec2f size;
};



#endif
