//
//  Car.cpp
//  ZiegenNachAfrika
//
//  Created by Christian Latsch on 03.12.12.
//
//

#include "Car.h"

#include "Globals.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Conversions.h"
#include "Box2D/Box2d.h"

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"



using namespace ci;
using namespace ci::app;

Car::Car(b2World &world, b2Vec2 position){
  
  m_World = &world;
  
  float xOffset = Conversions::toPhysics(position.x);
  float yOffset = Conversions::toPhysics(position.y);
  
  m_hz = 6.0f;
  m_zeta = 0.9f;
  m_speed = 50.0f;
//  m_speed = 0.0f;
  

  
  b2PolygonShape chassis;
  b2Vec2 vertices[8];
  vertices[0].Set(-2.2f, 0.5f);
  vertices[6].Set(2.18f, 0.5f);
  vertices[5].Set(2.23f, 0.35f);
  vertices[4].Set(2.0f, -0.1f);
  vertices[3].Set(0.3f, -0.9f);
  vertices[2].Set(-0.85f, -0.9f);
  vertices[1].Set(-1.9f, -0.2f);
  chassis.Set(vertices, 7);
  
  b2CircleShape circle;
  circle.m_radius = 0.4f;
  
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(0.0f + xOffset, 1.0f + yOffset);
  m_car = m_World->CreateBody(&bd);
  m_car->CreateFixture(&chassis, 1.0f);
  
  
  b2PolygonShape tree;
  b2Vec2 vertices2[8];
  vertices2[0].Set(1.2f, 0.0f);
  vertices2[1].Set(-2.5f, 0.0f);
  vertices2[2].Set(-0.5f, -0.1f);
  tree.Set(vertices2, 3);
  
  bd.position.Set(0.4f + xOffset, -0.0f + yOffset);
  m_tree = m_World->CreateBody(&bd);
  m_tree->CreateFixture(&tree, 1.0f);
  
  
  b2FixtureDef fd;
  fd.shape = &circle;
  fd.density = 1.0f;
  fd.friction = 1.0f;
  
  
  bd.position.Set(-1.25f+xOffset, 1.5f+yOffset);
  m_leftWheel = m_World->CreateBody(&bd);
  m_leftWheel->CreateFixture(&fd);
  
  bd.position.Set(1.58f+xOffset, 1.5f+yOffset);
  m_rightWheel = m_World->CreateBody(&bd);
  m_rightWheel->CreateFixture(&fd);
  
  b2WheelJointDef jd;
  b2Vec2 axis(0.0f, 1.0f);
  
  jd.Initialize(m_car, m_leftWheel, m_leftWheel->GetPosition(), axis);
  jd.motorSpeed = m_speed;
  jd.maxMotorTorque = 20.0f;
  jd.enableMotor = true;
  jd.frequencyHz = m_hz;
  jd.dampingRatio = m_zeta;
  m_spring1 = (b2WheelJoint*)m_World->CreateJoint(&jd);
  
  jd.Initialize(m_car, m_rightWheel, m_rightWheel->GetPosition(), axis);
  jd.motorSpeed = m_speed/2;
  jd.maxMotorTorque = 10.0f;
  jd.enableMotor = true;
  jd.frequencyHz = m_hz;
  jd.dampingRatio = m_zeta;
  m_spring2 = (b2WheelJoint*)m_World->CreateJoint(&jd);
  
  b2WheelJointDef jd2;
  b2Vec2 axis2(0.0f, -1.0f);
  
  jd2.Initialize(m_car, m_tree, m_car->GetPosition(), axis2);
  jd2.enableMotor = false;
  jd2.frequencyHz = m_hz;
  jd2.dampingRatio = m_zeta;
  m_spring3 = (b2WheelJoint*)m_World->CreateJoint(&jd2);
  
  jd2.Initialize(m_car, m_tree, b2Vec2(m_car->GetPosition().x-1, m_car->GetPosition().y), axis2);
  (b2WheelJoint*)m_World->CreateJoint(&jd2);
  

}

Car::~Car(){
}

b2Vec2 Car::GetPosition(){
  return m_car->GetPosition();
}

void Car::DestroyBody(){
  m_World->DestroyBody(m_rightWheel);
  m_World->DestroyBody(m_leftWheel);
  m_World->DestroyBody(m_car);
  m_World->DestroyBody(m_tree);
}


b2Body* Car::getBody() {
  return m_car;
}

void Car::update(){}

void Car::draw(){
//  
//  glPushMatrix();
//	
//	glTranslatef(m_Center.x, m_Center.y, 0);
//	glRotatef(m_Angle,0, 0, 1);
//	
//  
//	glBindTexture(GL_TEXTURE_2D, m_Textures);
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
//	glBegin(GL_POLYGON);
//	glTexCoord2f(0.5, 1); glVertex3f(m_PointOne.x, m_PointOne.y, 0);
//	glTexCoord2f(0, 0); glVertex3f(m_PointTwo.x, m_PointTwo.y, 0);
//	glTexCoord2f(1, 0); glVertex3f(m_PointThree.x, m_PointThree.y, 0);
//	glEnd();
//	
//	glPopMatrix();
  

  glPushMatrix();
    b2Fixture* fixtures = m_leftWheel->GetFixtureList();
    ci::gl::Texture m_wheelTexture = gl::Texture( loadImage( loadResource( "wheelVW.png" ) ) );
    ci::gl::Texture m_chassisTexture = gl::Texture( loadImage( loadResource( "chassisVW.png" ) ) );
    ci::gl::Texture m_treeTexture = gl::Texture( loadImage( loadResource( "tree.png" ) ) );
    glColor4f(ci::ColorA(1, 1, 1, 1));
    
    Vec2f pos = Conversions::toScreen( m_leftWheel->GetPosition());
    
    b2CircleShape* shape = (b2CircleShape*)fixtures->GetShape();
    
    float width = Conversions::toScreen( shape->m_radius * 2);
    
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(Conversions::radiansToDegrees( m_leftWheel->GetAngle()),0, 0, 1);
    glTranslatef(-width/2, -width/2, 0);
    
    gl::draw( m_wheelTexture);   
  gl::popMatrices();

  glPushMatrix();
    b2Fixture* rightWheelFixtures = m_rightWheel->GetFixtureList();
    glColor4f(ci::ColorA(1, 1, 1, 1));
    
    pos = Conversions::toScreen( m_rightWheel->GetPosition());
    
    b2CircleShape* rightWheelShape = (b2CircleShape*)rightWheelFixtures->GetShape();
    
    
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(Conversions::radiansToDegrees( m_rightWheel->GetAngle()),0, 0, 1);
    glTranslatef(-width/2, -width/2, 0);
    
    gl::draw( m_wheelTexture);
  gl::popMatrices();
  
  
  glPushMatrix();
    pos = Conversions::toScreen( m_car->GetPosition());
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(Conversions::radiansToDegrees( m_car->GetAngle()),0, 0, 1);
    glTranslatef(-228/2, -94/2, 0);
    gl::draw( m_chassisTexture);
  gl::popMatrices();
  
  glPushMatrix();
    pos = Conversions::toScreen( m_tree->GetPosition());
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(Conversions::radiansToDegrees( m_tree->GetAngle()),0, 0, 1);
    glTranslatef(-350/2, -194/2, 0);
    gl::draw( m_treeTexture);
  gl::popMatrices();





//  glColor4f(ci::ColorA(1, 0, 1, 0.5));
//
//  Vec2f pos = Conversions::toScreen( m_leftWheel->GetPosition() );
//  float t = Conversions::radiansToDegrees( m_leftWheel->GetAngle() );
//  float radius = Conversions::toScreen( 0.4f);
//  
//  glPushMatrix();
//    gl::translate( pos );
////    gl::rotate( t );
//    gl::drawSolidCircle(pos, radius, 0);
//  glPopMatrix();
//  
//  pos = Conversions::toScreen( m_rightWheel->GetPosition() );
//  t = Conversions::radiansToDegrees( m_rightWheel->GetAngle() );
//  radius = Conversions::toScreen( 0.4f);
//  
//  glPushMatrix();
//  gl::translate( pos );
//  //    gl::rotate( t );
//  gl::drawSolidCircle(pos, radius, 0);
//  glPopMatrix();
//  
//  pos = Conversions::toScreen( m_car->GetPosition() );
//  t = Conversions::radiansToDegrees( m_car->GetAngle() );
//  
//  glPushMatrix();
//  gl::translate( pos );
//  gl::rotate( t );
//  
//  Rectf rect(-Conversions::toScreen(1), -Conversions::toScreen(0.5), Conversions::toScreen(1), Conversions::toScreen(0.5));
//  gl::drawSolidRect(rect);
//  
//  glPopMatrix();
//


}
