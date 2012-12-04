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
    
    mWorld = &world;
    mHillWidth = 640;
    mHillSliceWidth = mHillWidth / 10;
    mHillCounter = 0;
    mHillStartY = 400;
    mColor = ci::ColorA(1, 1, 0, 0.5);  // blue to violet
    this->create(0);
}

Hills::~Hills() {
    //body->GetWorld()->DestroyBody( body );  // this ruins everything
}

void Hills::update(b2Vec2 position){
    
//    console() << "p = " << position.x << std::endl;
    
    if(position.x > mLastHillXPos) {
        this->create(0);
    
    }
    
    for( size_t i = 0; i < mHillsBodyVector.size(); ){
        if( mHillsBodyVector.at(i)->GetPosition().x < position.x - Conversions::toPhysics(640)) {
            mWorld->DestroyBody(mHillsBodyVector.at(i));
            mHillsBodyVector.at(i) = mHillsBodyVector.back();
            mHillsBodyVector.pop_back();
        } else {
            ++i;
        }
    }
  
  console() << mHillsBodyVector.size() << std::endl;
    
}


void Hills::create(int yStartHight){
    
  int xOffset = mHillCounter * mHillWidth;
  
  float hillStartY = yStartHight;
  
  float randomHeight =  Rand::randFloat( 0.0f, 30.0f );
  
  if(mHillCounter > 0){
    mHillStartY -= randomHeight;
  }
  
  for (int i = 0; i < mHillSliceWidth; i++){
    std::vector<b2Vec2> hillVector;
    
    
    hillVector.push_back(b2Vec2(i*10+xOffset, 800 ));
    hillVector.push_back(b2Vec2(i*10+xOffset, mHillStartY+randomHeight*cos(2*M_PI/mHillSliceWidth*i) ));
    hillVector.push_back(b2Vec2((i+1)*10+xOffset, mHillStartY+randomHeight*cos(2*M_PI/mHillSliceWidth*(i+1))));
    hillVector.push_back(b2Vec2((i+1)*10+xOffset, 800 ));
    
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
    
    
    b2Body* sliceBody = mWorld->CreateBody(&polygonBodyDef);
    sliceBody->CreateFixture(&polygonFixtureDef);
    mHillsBodyVector.push_back(sliceBody);
    
    if (i == mHillSliceWidth - 1 ) {
        mLastHillXPos = Conversions::toPhysics((i+1)*10+xOffset) - Conversions::toPhysics(320);
    }
    
    hillVector.clear();

  }
  mHillStartY=mHillStartY+randomHeight;
  mHillCounter += 1;

}

void Hills::draw(){
    ci::gl::Texture hillPattern = gl::Texture( loadImage( loadResource( "hillPatternNew.jpg" ) ) );
    
    for (int i = 0; i < mHillsBodyVector.size(); i++) {
        
        b2Body* body = mHillsBodyVector.at(i);

        glColor4f(ci::ColorA(1, 1, 1, 1));
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Shape::Type shapeType = fixture->GetType();
            if ( shapeType == b2Shape::e_polygon )
            {
                b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();
                int count = polygonShape->GetVertexCount();
                Vec2f bodyPos = Conversions::toScreen(body->GetPosition());
              Vec2f patternPos;
                glBegin(GL_POLYGON);
                    for(int j = 0; j < count; j++) {
                        Vec2f vertexPos = Conversions::toScreen(polygonShape->GetVertex(j));
                        glVertex3f(bodyPos.x + vertexPos.x, bodyPos.y + vertexPos.y, 0.0);
                      if (j == 1) {
                        patternPos = Vec2f(bodyPos.x + vertexPos.x, bodyPos.y + vertexPos.y);
                      }
                    }
                glEnd();
              
              
              glPushMatrix();
                glTranslatef(patternPos.x, patternPos.y+10, 0);
                gl::draw( hillPattern);
              gl::popMatrices();
              
                
                
            }
        }
        
    }
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










