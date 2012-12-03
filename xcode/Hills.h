//
//  Hills.h
//  ZiegenNachAfrika
//
//  Created by Christian Latsch on 12/2/12.
//
//

#ifndef ZiegenNachAfrika_Hills_h
#define ZiegenNachAfrika_Hills_h

#include "cinder/Color.h"
#include <Box2D/Box2d.h>
#include <vector>


class Hills {
public:
    Hills(b2World &world);
    ~Hills();
    
    void update(b2Vec2 position);
    void create(int yStartHight);
    void draw();
    b2Vec2 computeCentroid(std::vector <b2Vec2> vertices);
    b2Vec2 computeCentroid2(std::vector<b2Vec2> vertices);
    
    //private:
    // store a pointer to the particle in the physics world from the main app
    std::vector <b2Body*> mHillsBodyVector;
    float mHillWidth, mHillSliceWidth, mLastHillXPos;
    int mHillCounter;
    
    ci::Color mColor;
    
    b2World *mWorld;
    
};

#endif


