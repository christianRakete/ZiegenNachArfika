//
//  Particle.cpp
//  ZiegenNachAfrika
//
//  Created by Christian Latsch on 12/1/12.
//
//

#include "Particle.h"

#include "Globals.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Conversions.h"
#include "Box2D/Box2d.h"


using namespace ci;
using namespace ci::app;
using namespace std;

    
Particle::Particle() {
}

Particle::~Particle() {
    //body->GetWorld()->DestroyBody( body );  // this ruins everything
}


void Particle::setup(ci::Vec2f boxSize) {
    
    size = boxSize;
    
    color = ci::ColorA(ci::Rand::randFloat(0,.8), 0, 1, 1);  // blue to violet
}

void Particle::update() {
    
}

void Particle::draw() {
    
    glColor4f(color);
    
    Vec2f pos = Conversions::toScreen( body->GetPosition() );
    float t = Conversions::radiansToDegrees( body->GetAngle() );
    
    glPushMatrix();
    gl::translate( pos );
    gl::rotate( t );
    
    Rectf rect(-size.x, -size.y, size.x, size.y);
    gl::drawSolidRect(rect);
    
    glPopMatrix();
}

