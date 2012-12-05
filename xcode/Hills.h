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
    
    //private:
    // store a pointer to the particle in the physics world from the main app
    std::vector <b2Body*> m_HillsBodyVector;
    std::vector <std::pair <b2Vec2, b2Vec2> > m_HillsPolygonVector;
    float m_HillWidth, m_HillSliceWidth, m_LastHillXPos, m_HillStartY;
    int m_HillCounter;
    
    ci::Color m_Color;
    
    b2World *m_World;
    
};

#endif


