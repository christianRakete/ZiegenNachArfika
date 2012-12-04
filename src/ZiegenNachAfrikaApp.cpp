#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Box2D/Box2d.h"
#include "cinder/Camera.h"


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
b2World m_world(gravity);


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
  Hills* mHills = new Hills(m_world);
  ci::CameraOrtho mCamera;
  
  Car* m_car = new Car(m_world);

};

void ZiegenNachAfrikaApp::mouseMove( MouseEvent event ) {
//    console() << "m = " << Conversions::toPhysics(event.getPos().x) << std::endl;
}


void ZiegenNachAfrikaApp::setup()
{
    mCamera.setOrtho( 100, getWindowWidth()+100, getWindowHeight(), 0, -1, 1 );
  
    
    
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(-Conversions::toPhysics(200), Conversions::toPhysics(getWindowHeight()-150));
    
  b2Body* groundBody = m_world.CreateBody(&groundBodyDef);
    
  b2PolygonShape groundBox;
	groundBox.SetAsBox(Conversions::toPhysics(app::getWindowWidth()), Conversions::toPhysics(1.0f)); // size the ground
    
	// 4. create fixture on body
	groundBody->CreateFixture(&groundBox, 0.0f);
        
	// create a dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(Conversions::toPhysics(320), Conversions::toPhysics(200));
    
	// instead of just creating body...
	// b2Body* body = world->CreateBody(&bodyDef);
	// do the following to create it with a circular reference to it's corresponding particle
	bodyDef.userData = &p;
  p.body = m_world.CreateBody(&bodyDef);
  
	b2PolygonShape dynamicBox;
	float boxSizeX = Rand::randFloat(global::BOX_X_MIN, global::BOX_X_MAX);
	float boxSizeY = Rand::randFloat(global::BOX_Y_MIN, global::BOX_Y_MAX);
    
	dynamicBox.SetAsBox(Conversions::toPhysics(boxSizeX), Conversions::toPhysics(boxSizeY));
    
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
    
  p.body->CreateFixture(&fixtureDef);
  
	// rest of initialization particle can do for itself
  p.setup(Vec2f(boxSizeX, boxSizeY));
  
}

void ZiegenNachAfrikaApp::mouseDown( MouseEvent event )
{
}

void ZiegenNachAfrikaApp::update() {
    // step physics world
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
  mHills->update(m_car->GetPosition());
    
	m_world.Step(timeStep, velocityIterations, positionIterations);
  
    
 float left = Conversions::toScreen(m_car->GetPosition().x) - getWindowWidth()/2;
 float right = Conversions::toScreen(m_car->GetPosition().x) + getWindowWidth()/2;
 float top = Conversions::toScreen(m_car->GetPosition().y) - getWindowHeight()/2;
 float bottom = Conversions::toScreen(m_car->GetPosition().y) + getWindowHeight()/2;
  
// mCamera.setOrtho( left, right,bottom, top, -1, 1 );
  

}

void ZiegenNachAfrikaApp::draw()
{
    gl::setMatrices( mCamera );
    gl::enableAlphaBlending();
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  p.draw();
    mHills->draw();
    gl::drawLine( Vec2f(0, getWindowHeight()), Vec2f(getWindowWidth(), getWindowHeight()));
//  m_car->draw();
  
//  debugDraw(true, true);
//  m_car->draw();
    debugDraw(true, true);
  
//    Vec2f pos = Conversions::toScreen( m_wheel1->GetPosition() );
//    float t = Conversions::radiansToDegrees( m_wheel1->GetAngle() );
//    
//    glPushMatrix();
//    gl::translate( pos );
//    gl::rotate( t );
//    
//    Rectf rect(-10, -5, 10, 5);
//    gl::drawSolidRect(rect);
  
//    glPopMatrix();
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
		b2Body* bodies = m_world.GetBodyList();
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
		b2Contact* contacts = m_world.GetContactList();
    
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
