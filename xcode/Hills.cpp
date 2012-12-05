//
//  Hills.cpp
//  ZiegenNachAfrika
//
//  Created by Christian Latsch on 12/2/12.
//
//

#include "Hills.h"
#include "Globals.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Conversions.h"
#include "Box2D/Box2d.h"
#include <cmath>

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"


using namespace ci;
using namespace ci::app;
using namespace std;


Hills::Hills(b2World &world) {
    
    m_World = &world;
    m_HillWidth = 1280;
    m_HillSliceWidth = m_HillWidth / 10;
    m_HillCounter = 0;
    m_HillStartY = 400;
    m_Color = ci::ColorA(1, 1, 0, 0.5);  // blue to violet
    this->create(0);
}

Hills::~Hills() {
}

void Hills::update(b2Vec2 position){
        
    if(position.x > m_LastHillXPos) {
        this->create(0);
    }
  
  
  for( std::vector<b2Body*>::iterator it = m_HillsBodyVector.begin(); it != m_HillsBodyVector.end(); it++)
  {
    if ((*it)->GetPosition().x < position.x - Conversions::toPhysics(getWindowWidth())){
      m_World->DestroyBody((*it));
      m_HillsBodyVector.erase(it);
      --it;
    }
  }
  
  for( std::vector<std::pair <b2Vec2, b2Vec2> >::iterator it = m_HillsPolygonVector.begin(); it != m_HillsPolygonVector.end(); it++)
  {
    if ((*it).first.x < Conversions::toScreen(position.x) - getWindowWidth()){
      m_HillsPolygonVector.erase(it);
      --it;
    }
  }
  
//
//    // better body removing
//    
//    for( size_t i = 0; i < m_HillsBodyVector.size(); ){
//        if( m_HillsBodyVector.at(i)->GetPosition().x < position.x - Conversions::toPhysics(640)) {
//            m_World->DestroyBody(m_HillsBodyVector.at(i));
//            m_HillsBodyVector.at(i) = m_HillsBodyVector.back();
//            m_HillsBodyVector.pop_back();
//        } else {
//            ++i;
//        }
//    }
  
}


void Hills::create(int yStartHight){
    
  int xOffset = m_HillCounter * m_HillWidth;
  
  float hillStartY = yStartHight;
  
  float randomHeight =  Rand::randFloat( 0.0f, 70.0f );
  
  if(m_HillCounter > 0){
    m_HillStartY -= randomHeight;
  }
  
  for (int i = 0; i < m_HillSliceWidth; i++){
    std::vector<b2Vec2> hillVector;
    
    b2Vec2 lowerLeft = b2Vec2(i*10+xOffset, 800 );
    b2Vec2 upperLeft = b2Vec2(i*10+xOffset, m_HillStartY+randomHeight*cos(-2*M_PI/m_HillSliceWidth*i) );
    b2Vec2 upperRight = b2Vec2((i+1)*10+xOffset, m_HillStartY+randomHeight*cos(-2*M_PI/m_HillSliceWidth*(i+1)));
    b2Vec2 lowerRight = b2Vec2((i+1)*10+xOffset, 800 );
    
    hillVector.push_back(lowerLeft);
    hillVector.push_back(upperLeft);
    hillVector.push_back(upperRight);
    hillVector.push_back(lowerRight);
    
    m_HillsPolygonVector.push_back(std::make_pair(upperLeft, upperRight));
        
    b2Vec2 centeroid = computeCentroid(hillVector);
    
    int32 count = hillVector.size();
        
    b2Vec2 vertices[4];
    for(int j = 0; j < 4; j++) {
        float x = Conversions::toPhysics(hillVector.at(j).x - centeroid.x);
        float y = Conversions::toPhysics(hillVector.at(j).y - centeroid.y);
        vertices[j].Set(x, y);
    }
    
    
    b2BodyDef polygonBodyDef;
    polygonBodyDef.type = b2_staticBody; //this will be a dynamic body
    polygonBodyDef.position.Set(Conversions::toPhysics(centeroid.x), Conversions::toPhysics(centeroid.y));
    
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, count);
            
    b2FixtureDef polygonFixtureDef;
    polygonFixtureDef.friction = 1.0f;
    polygonFixtureDef.shape = &polygonShape; //change the shape of the fixture
    
    
    b2Body* sliceBody = m_World->CreateBody(&polygonBodyDef);
    sliceBody->CreateFixture(&polygonFixtureDef);
    m_HillsBodyVector.push_back(sliceBody);
    
    if (i == m_HillSliceWidth - 1 ) {
        m_LastHillXPos = Conversions::toPhysics((i+1)*10+xOffset) - Conversions::toPhysics(getWindowWidth()/1.5);
    }
    
    hillVector.clear();

  }
  m_HillStartY=m_HillStartY+randomHeight;
  m_HillCounter += 1;

}

void Hills::draw(){
  glColor4f(ci::ColorA(1, 0, 0, 1));
//  ci::gl::Texture hillPattern = gl::Texture( loadImage( loadResource( "texture.jpg" ) ) );
  for (int i = 0; i < m_HillsPolygonVector.size(); i++) {
    b2Vec2 point1 = m_HillsPolygonVector.at(i).first;
    b2Vec2 point2 = m_HillsPolygonVector.at(i).second;
//    hillPattern.enableAndBind();
//    hillPattern.setWrap(GL_REPEAT, GL_REPEAT);
    glPushMatrix();
      glBegin(GL_TRIANGLE_STRIP);
        glColor4f(ci::ColorA(1, 1, 1, 1));
        glVertex2f( point1.x, point1.y);
//        glTexCoord2f(0,256);
        glColor4f(ci::ColorA(0.6, 0.6, 0.6, 1));
        glVertex2f( point1.x, point1.y+500 );
//        glTexCoord2f(1,256);
        glColor4f(ci::ColorA(1, 1, 1, 1));
        glVertex2f( point2.x, point2.y );//vertex 1
//        glTexCoord2f(1,0);
        glColor4f(ci::ColorA(0.6, 0.6, 0.6, 1));
        glVertex2f( point2.x, point1.y+500); //vertex 2
//        glTexCoord2f(0,0);
//        glTexCoord2f(0,0);
         //vertex 3
//        glTexCoord2f(512,512);
         //vertex 4
//        glTexCoord2f(512,0);
//      glVertex2f( point1.x, point1.y+512); //vertex 1
//      glTexCoord2f(point1.x/512,1);
//      glVertex2f( point1.x, point1.y); //vertex 2
//      glTexCoord2f(point1.x/512,0);
//      glVertex2f( point2.x, point1.y+512 ); //vertex 3
//      glTexCoord2f(point2.x/512,1);
//      glVertex2f( point2.x, point2.y ); //vertex 4
//      glTexCoord2f(point2.x/512,1);
      glEnd();
    glPopMatrix();
//    hillPattern.unbind();
//    gl::drawLine( Vec2f(point1.x, point1.y), Vec2f(point2.x, point2.y));
  }
//  hillPattern.unbind();
  
  
//    ci::gl::Texture hillPattern = gl::Texture( loadImage( loadResource( "hillPatternNew.jpg" ) ) );
//    
//    for (int i = 0; i < m_HillsBodyVector.size(); i++) {
//        
//        b2Body* body = m_HillsBodyVector.at(i);
//
//        glColor4f(ci::ColorA(1, 1, 1, 1));
//        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
//            b2Shape::Type shapeType = fixture->GetType();
//            if ( shapeType == b2Shape::e_polygon )
//            {
//                b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();
//                int count = polygonShape->GetVertexCount();
//                Vec2f bodyPos = Conversions::toScreen(body->GetPosition());
//              Vec2f patternPos;
//                glBegin(GL_POLYGON);
//                    for(int j = 0; j < count; j++) {
//                        Vec2f vertexPos = Conversions::toScreen(polygonShape->GetVertex(j));
//                        glVertex3f(bodyPos.x + vertexPos.x, bodyPos.y + vertexPos.y, 0.0);
//                      if (j == 1) {
//                        patternPos = Vec2f(bodyPos.x + vertexPos.x, bodyPos.y + vertexPos.y);
//                      }
//                    }
//                glEnd();
//              
//              
////              glPushMatrix();
////                glTranslatef(patternPos.x, patternPos.y+10, 0);
////                gl::draw( hillPattern);
////              gl::popMatrices();
//              
//                
//                
//            }
//        }
//        
//    }
}


b2Vec2 Hills::computeCentroid(std::vector<b2Vec2> vertices) {
    b2Vec2 centroid;
    double signedArea = 0.0;
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double a = 0.0;  // Partial signed area
    
    // For all vertices except last
    int i;
    for (i = 0; i < 3; ++i)
    {
        x0 = vertices.at(i).x;
        y0 = vertices.at(i).y;
        x1 = vertices.at(i+1).x;
        y1 = vertices.at(i+1).y;
        a = x0*y1 - x1*y0;
        signedArea += a;
        centroid.x += (x0 + x1)*a;
        centroid.y += (y0 + y1)*a;
    }
    
    // Do last vertex
    x0 = vertices.at(i).x;
    y0 = vertices.at(i).y;
    x1 = vertices.at(0).x;
    y1 = vertices.at(0).y;
    a = x0*y1 - x1*y0;
    signedArea += a;
    centroid.x += (x0 + x1)*a;
    centroid.y += (y0 + y1)*a;
    
    signedArea *= 0.5;
    centroid.x /= (6*signedArea);
    centroid.y /= (6*signedArea);
    
    return centroid;
}










