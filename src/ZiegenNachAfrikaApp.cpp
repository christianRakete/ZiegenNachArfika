#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Box2D/Box2d.h"
#include "cinder/Camera.h"


#include "Conversions.h"
#include "Globals.h"
#include "Particle.h"

using namespace ci;
using namespace ci::app;
using namespace std;

b2Vec2 gravity(0.0f, 10.0f);
b2World world(gravity);

class ZiegenNachAfrikaApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    Particle p = Particle();
    ci::CameraOrtho mCamera;

};

void ZiegenNachAfrikaApp::setup()
{
    mCamera.setOrtho( 100, getWindowWidth()+100, getWindowHeight(), 0, -1, 1 );
    
    
    b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(Conversions::toPhysics(getWindowWidth()/2), Conversions::toPhysics(getWindowHeight()));
    
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    
    b2PolygonShape groundBox;
	groundBox.SetAsBox(Conversions::toPhysics(app::getWindowWidth()/2), Conversions::toPhysics(1.0f)); // size the ground
    
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
	p.body = world.CreateBody(&bodyDef);
    
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
	world.Step(timeStep, velocityIterations, positionIterations);
    
    float left = Conversions::toScreen(p.body->GetPosition().x) - getWindowWidth()/2;
    float right = Conversions::toScreen(p.body->GetPosition().x) + getWindowWidth()/2;
    float top = Conversions::toScreen(p.body->GetPosition().y) - getWindowHeight()/2;
    float bottom = Conversions::toScreen(p.body->GetPosition().y) + getWindowHeight()/2;
    
    mCamera.setOrtho( left, right,bottom, top, -1, 1 );
    

}

void ZiegenNachAfrikaApp::draw()
{
    gl::setMatrices( mCamera );
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    p.draw();
    gl::drawLine( Vec2f(0, getWindowHeight()), Vec2f(getWindowWidth(), getWindowHeight()));
}

CINDER_APP_BASIC( ZiegenNachAfrikaApp, RendererGl )
