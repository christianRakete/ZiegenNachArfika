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

Car::Car(b2World &world){
  
  m_World = &world;
  
  m_hz = 6.0f;
  m_zeta = 0.9f;
  m_speed = 40.0f;
  

  
  b2PolygonShape chassis;
  b2Vec2 vertices[8];
  vertices[0].Set(-2.0f, 0.5f);
  vertices[5].Set(2.0f, 0.5f);
  vertices[4].Set(2.0f, 0.0f);
  vertices[3].Set(0.0f, -0.9f);
  vertices[2].Set(-1.85f, -0.9f);
  vertices[1].Set(-2.0f, -0.2f);
  chassis.Set(vertices, 6);
  
  b2CircleShape circle;
  circle.m_radius = 0.4f;
  
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(0.0f, 1.0f);
  m_car = m_World->CreateBody(&bd);
  m_car->CreateFixture(&chassis, 1.0f);
  
  b2FixtureDef fd;
  fd.shape = &circle;
  fd.density = 1.0f;
  fd.friction = 1.0f;
  
  
  bd.position.Set(-1.4f, 1.8f);
  m_leftWheel = m_World->CreateBody(&bd);
  m_leftWheel->CreateFixture(&fd);
  
  bd.position.Set(1.4f, 1.8f);
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
  jd.motorSpeed = 0.0f;
  jd.maxMotorTorque = 10.0f;
  jd.enableMotor = false;
  jd.frequencyHz = m_hz;
  jd.dampingRatio = m_zeta;
  m_spring2 = (b2WheelJoint*)m_World->CreateJoint(&jd);

}

Car::~Car(){
}

b2Vec2 Car::GetPosition(){
  return m_car->GetPosition();
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
    ci::gl::Texture m_wheelTexture = gl::Texture( loadImage( loadResource( "wheelBW.png" ) ) );
    ci::gl::Texture m_chassisTexture = gl::Texture( loadImage( loadResource( "chassisBW.png" ) ) );
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
