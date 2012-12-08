//
//  Car.h
//  ZiegenNachAfrika
//
//  Created by Christian Latsch on 03.12.12.
//
//

#ifndef __ZiegenNachAfrika__Car__
#define __ZiegenNachAfrika__Car__

#include <iostream>
#include "Box2D/Box2d.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

class Car {
public:
  Car(b2World &world, b2Vec2 position);
  ~Car();
  
  // pass in a pointer to the particle
  void setup();
  void update();
  void draw();
  b2Vec2 GetPosition();
  void DestroyBody();
  b2Body* getBody();
  
  //private:
  // store a pointer to the particle in the physics world from the main app
  b2Body* m_car;
  b2Body* m_tree;
  b2Body* m_leftWheel;
  b2Body* m_rightWheel;
  b2WheelJoint* m_spring1;
  b2WheelJoint* m_spring2;
  b2WheelJoint* m_spring3;
  float32 m_hz;
  float32 m_zeta;
  float32 m_speed;
  
  b2World *m_World;
  
  

};


#endif /* defined(__ZiegenNachAfrika__Car__) */
