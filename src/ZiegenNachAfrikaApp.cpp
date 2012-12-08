#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Timer.h"
#include "Box2D/Box2d.h"
#include "cinder/Camera.h"
#include <cmath>

#include "Conversions.h"
#include "Globals.h"
#include "Particle.h"
#include "Hills.h"
#include "Car.h"

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

b2Vec2 gravity(0.0f, 10.0f);
b2World m_World(gravity);

class ZiegenNachAfrikaApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );
  void mouseMove( MouseEvent event );
	void update();
	void draw();
  void debugDraw( bool drawBodies, bool drawContacts );
    
  gl::Texture m_wheelTexture;

    
  Particle p = Particle();
  Hills* m_Hills;
  ci::CameraOrtho mCamera;
  
  Car* m_Car;
  
  std::vector <ci::gl::Texture> m_BackgroundTexturesVec;
  ci::gl::Texture m_ForegroundTexture;


  ci::gl::Texture m_BG1Texture;
  ci::gl::Texture m_BG2Texture;
  
  float m_Background1Pos;
  float m_Background2Pos;
  float m_bg1Offset;
  float m_bg2Offset;
  
  float m_Center;
  float m_CenterY;
  
  float m_CarVelocity;
  
  float mResetCounter;
  
  Vec2f m_oldPosition;
  
  Timer m_ResetTimer;
  bool m_ResetTimerRunning;

};

void ZiegenNachAfrikaApp::mouseMove( MouseEvent event ) {
//    console() << "m = " << Conversions::toPhysics(event.getPos().x) << std::endl;
}


void ZiegenNachAfrikaApp::setup()
{
  ////// Box2d Setup
  
  m_Hills = new Hills(m_World);
  m_Car = new Car(m_World, b2Vec2(400, 100));
  
  
  mResetCounter = 420;
  
  m_ResetTimer = new Timer();
  m_ResetTimerRunning = false;
  
  ////// Camera Setup

  mCamera.setOrtho( 700, getWindowWidth()+700, getWindowHeight(), 0, -1, 1 );
  
  
//  ////// Textures Setup
//  
//  m_BackgroundTexturesVec.push_back(gl::Texture( loadImage( loadResource( "background-2-1.png" ) ) ));
//  m_BackgroundTexturesVec.push_back(gl::Texture( loadImage( loadResource( "background-2-2.png" ) ) ));
//
//  m_ForegroundTexture = gl::Texture( loadImage( loadResource( "foreground.png" ) ) );
//  
//  m_BG1Texture = m_BackgroundTexturesVec.at(Rand::randInt( 0, m_BackgroundTexturesVec.size()));
//  m_BG2Texture = m_BackgroundTexturesVec.at(Rand::randInt( 0, m_BackgroundTexturesVec.size()));
//
//  
//  m_Background1Pos = -global::BG_TEXTURE_WIDTH/2;
//  m_bg1Offset = 0;
//  m_bg2Offset = 0;
//
//  b2BodyDef groundBodyDef;
//  groundBodyDef.position.Set(-Conversions::toPhysics(200), Conversions::toPhysics(getWindowHeight()-150));
//    
//  b2Body* groundBody = m_World.CreateBody(&groundBodyDef);
//    
//  b2PolygonShape groundBox;
//	groundBox.SetAsBox(Conversions::toPhysics(app::getWindowWidth()), Conversions::toPhysics(1.0f)); // size the ground
//    
//	// 4. create fixture on body
//	groundBody->CreateFixture(&groundBox, 0.0f);
//        
//	// create a dynamic body
//	b2BodyDef bodyDef;
//	bodyDef.type = b2_dynamicBody;
//	bodyDef.position.Set(Conversions::toPhysics(320), Conversions::toPhysics(200));
//    
//	// instead of just creating body...
//	// b2Body* body = world->CreateBody(&bodyDef);
//	// do the following to create it with a circular reference to it's corresponding particle
//	bodyDef.userData = &p;
//  p.body = m_World.CreateBody(&bodyDef);
//  
//	b2PolygonShape dynamicBox;
//	float boxSizeX = Rand::randFloat(global::BOX_X_MIN, global::BOX_X_MAX);
//	float boxSizeY = Rand::randFloat(global::BOX_Y_MIN, global::BOX_Y_MAX);
//    
//	dynamicBox.SetAsBox(Conversions::toPhysics(boxSizeX), Conversions::toPhysics(boxSizeY));
//    
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &dynamicBox;
//	fixtureDef.density = 1.0f;
//	fixtureDef.friction = 0.3f;
//	fixtureDef.restitution = 0.5f; // bounce
  
//  p.body->CreateFixture(&fixtureDef);
  
	// rest of initialization particle can do for itself
//  p.setup(Vec2f(boxSizeX, boxSizeY));
  
}

void ZiegenNachAfrikaApp::mouseDown( MouseEvent event )
{
}

void ZiegenNachAfrikaApp::update() {
    // step physics world
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
  m_Hills->update(m_Car->GetPosition());
  
	m_World.Step(timeStep, velocityIterations, positionIterations);
  
//  0.2 * Conversions::toScreen(m_Car->GetPosition().x) - 0.8 * m_Center;
//  console() << smoothCameraOffset << "vec = " << m_CameraVector.x << std::endl;
  
  
  float newCarPosX = Conversions::toScreen(m_Car->GetPosition().x);

  
//  m_SmoothCarSpeed =  0.9 * m_SmoothCarSpeed - 0.1 * speed;
  
  m_Center = newCarPosX;
  m_CenterY = m_Car->GetPosition().y;
  
  
  m_CarVelocity = (m_Car->getBody()->GetLinearVelocity().x * 10) * 0.1 + m_CarVelocity * 0.9;
    
  
  float left = m_Center - m_CarVelocity  +300 - getWindowWidth()/2;
  float right = m_Center - m_CarVelocity +300 + getWindowWidth()/2;
  float top = 0;
  float bottom = getWindowHeight();
  
  Vec2f pos = Conversions::toScreen(m_Car->GetPosition());
  
  mCamera.setOrtho( left, right,bottom, top, -1, 1 );
  
// m_Background1Pos = m_Center/1.2 - global::BG_TEXTURE_WIDTH/2 + m_bg1Offset * global::BG_TEXTURE_WIDTH;
// m_Background2Pos = m_Center/1.2 + global::BG_TEXTURE_WIDTH/2 + m_bg2Offset * global::BG_TEXTURE_WIDTH;
////
//// console() << "camera = " << m_Center << " Texture = " << m_Background1Pos << std::endl;
////  
//  if (m_Center - global::BG_TEXTURE_WIDTH > m_Background1Pos + global::BG_TEXTURE_WIDTH/2) {
//    m_bg1Offset +=2;
//    m_BG1Texture = m_BackgroundTexturesVec.at(Rand::randInt( 0, m_BackgroundTexturesVec.size()));
//  }
//  
//  if (m_Center - global::BG_TEXTURE_WIDTH > m_Background2Pos + global::BG_TEXTURE_WIDTH/2) {
//    m_bg2Offset +=2;
//    m_BG2Texture = m_BackgroundTexturesVec.at(Rand::randInt( 0, m_BackgroundTexturesVec.size()));
//  }
////
  
  float xrange = sqrt((pos.x - m_oldPosition.x) * (pos.x - m_oldPosition.x));
  float yrange = sqrt((pos.y - m_oldPosition.y) * (pos.y - m_oldPosition.y));
  
  if (xrange < 4 && yrange < 4 && pos.x  > mResetCounter && m_ResetTimer.isStopped()) {
    m_ResetTimer.start();
  }
  
  if (xrange < 4 && yrange < 4 && pos.x  > mResetCounter && m_ResetTimer.getSeconds() > 10) {
    m_Car->DestroyBody();
    m_Car = new Car(m_World, b2Vec2(pos.x, 100));
    mResetCounter = m_Center + 420;
    m_ResetTimer.stop();
  
  } else if  (m_ResetTimer.getSeconds() > 10){
    m_ResetTimer.stop();
  }
  
  
  m_oldPosition = Conversions::toScreen(m_Car->GetPosition());

}

void ZiegenNachAfrikaApp::draw()
{
  
  //////
  
  gl::setMatrices( mCamera );
  gl::enableAlphaBlending();
	gl::clear( Color( 0, 0, 0 ) );
  
  
//  
//  glPushMatrix();
//  gl::translate(Vec2f(m_Background1Pos, m_CenterY));
//    gl::draw(m_BG1Texture);
//  glPopMatrix();
//  
//  glPushMatrix();
//  gl::translate(Vec2f(m_Background2Pos, m_CenterY));
//  gl::draw(m_BG2Texture);
//  glPopMatrix();
  
  
//  glPushMatrix();
//  gl::translate(Vec2f(m_Center/1.2 + global::BG_TEXTURE_WIDTH/2,0));
//  gl::draw(m_BackgroundTexturesVec.at(0));
//  glPopMatrix();
//  
//  glPushMatrix();
//  gl::translate(Vec2f(m_Center/1.+ 2*global::BG_TEXTURE_WIDTH/2,0));
//  gl::draw(m_BackgroundTexturesVec.at(0));
//  glPopMatrix();


  m_Car->draw();
  m_Hills->draw();
  
//  gl::drawLine( Vec2f(0, getWindowHeight()), Vec2f(getWindowWidth(), getWindowHeight()));
  

//  glPushMatrix();
//  gl::translate(Vec2f(0, 10));
//  gl::draw(m_ForegroundTexture);
//  glPopMatrix();
//  debugDraw(true, true);
}


void ZiegenNachAfrikaApp::debugDraw( bool drawBodies, bool drawContacts )
{
	// should utilize an extension of b2DebugDraw (will soon)
	//
	if( drawBodies )
	{
		//draw all bodies, contact points, etc
    
		gl::color( ColorA(1.0f, 0.0f, 0.1f, 0.5f) );
    
		//draw bodies
		b2Body* bodies = m_World.GetBodyList();
		while( bodies != NULL )
		{
			b2Vec2 pos = bodies->GetPosition();
			float32 angle = bodies->GetAngle();
      
			gl::pushMatrices();
      
			gl::translate( Conversions::toScreen(pos) );
			gl::rotate( Conversions::radiansToDegrees( angle ) );
      
			//draw the fixtures for this body
			b2Fixture* fixtures = bodies->GetFixtureList();
			while( fixtures != NULL )
			{
				//not sure why the base b2Shape doesn't contain the vertex methods...
				switch (fixtures->GetType()) {
					case b2Shape::e_polygon:
          {
            b2PolygonShape* shape = (b2PolygonShape*)fixtures->GetShape();
            
            glBegin(GL_POLYGON);
            
            for( int i=0; i != shape->GetVertexCount(); ++i )
            {
              gl::vertex( Conversions::toScreen( shape->GetVertex(i) ) );
            }
            
            glEnd();
          }
						break;
					case b2Shape::e_circle:
          {
            b2CircleShape* shape = (b2CircleShape*)fixtures->GetShape();
            gl::drawSolidCircle( Conversions::toScreen( shape->m_p ), Conversions::toScreen( shape->m_radius ) );
          }
						break;
            
					default:
						break;
				}
        
        
				fixtures = fixtures->GetNext();
			}
      
			gl::popMatrices();
      
			bodies = bodies->GetNext();
		}
	}
  
	if( drawContacts )
	{
		//draw contacts
		b2Contact* contacts = m_World.GetContactList();
    
		gl::color( ColorA( 0.0f, 0.0f, 1.0f, 0.8f ) );
		glPointSize(3.0f);
		glBegin(GL_POINTS);
    
		while( contacts != NULL )
		{
			b2WorldManifold m;
			contacts->GetWorldManifold(&m);	//grab the
      
			for( int i=0; i != b2_maxManifoldPoints; ++i )
			{
				Vec2f p = Conversions::toScreen( m.points[i] );
				gl::vertex( p );
			}
      
			contacts = contacts->GetNext();
		}
		glEnd();
	}
}


CINDER_APP_BASIC( ZiegenNachAfrikaApp, RendererGl )
