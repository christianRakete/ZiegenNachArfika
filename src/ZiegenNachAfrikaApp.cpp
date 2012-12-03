#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Box2D/Box2d.h"
#include "cinder/Camera.h"


#include "Conversions.h"
#include "Globals.h"
#include "Particle.h"
#include "Hills.h"

using namespace ci;
using namespace ci::app;
using namespace std;

b2Vec2 gravity(0.0f, 10.0f);
b2World world(gravity);
b2Body* m_car;
b2Body* m_wheel1;
b2Body* m_wheel2;
float32 m_hz;
float32 m_zeta;
float32 m_speed;
b2WheelJoint* m_spring1;
b2WheelJoint* m_spring2;

class ZiegenNachAfrikaApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );
    void mouseMove( MouseEvent event );
	void update();
	void draw();
    
    Particle p = Particle();
    Hills* mHills = new Hills(world);
    ci::CameraOrtho mCamera;

};

void ZiegenNachAfrikaApp::mouseMove( MouseEvent event ) {
//    console() << "m = " << Conversions::toPhysics(event.getPos().x) << std::endl;
}


void ZiegenNachAfrikaApp::setup()
{
    mCamera.setOrtho( 100, getWindowWidth()+100, getWindowHeight(), 0, -1, 1 );
    
    
    b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(Conversions::toPhysics(0), Conversions::toPhysics(getWindowHeight()));
    
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    
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
    
    b2PolygonShape chassis;
    b2Vec2 vertices[8];
    vertices[0].Set(-1.5f, -0.5f);
    vertices[1].Set(1.5f, -0.5f);
    vertices[2].Set(1.5f, 0.0f);
    vertices[3].Set(0.0f, 0.9f);
    vertices[4].Set(-1.15f, 0.9f);
    vertices[5].Set(-1.5f, 0.2f);
    chassis.Set(vertices, 6);
    
    b2CircleShape circle;
    circle.m_radius = 0.4f;
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(0.0f, 1.0f);
    m_car = world.CreateBody(&bd);
    m_car->CreateFixture(&chassis, 1.0f);
    
    b2FixtureDef fd;
    fd.shape = &circle;
    fd.density = 1.0f;
    fd.friction = 0.9f;
    
    bd.position.Set(-1.0f, 0.35f);
    m_wheel1 = world.CreateBody(&bd);
    m_wheel1->CreateFixture(&fd);
    
    bd.position.Set(1.0f, 0.4f);
    m_wheel2 = world.CreateBody(&bd);
    m_wheel2->CreateFixture(&fd);
    
    b2WheelJointDef jd;
    b2Vec2 axis(0.0f, 1.0f);
    
    jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
    jd.motorSpeed = 30.0f;
    jd.maxMotorTorque = 20.0f;
    jd.enableMotor = true;
    jd.frequencyHz = m_hz;
    jd.dampingRatio = m_zeta;
    m_spring1 = (b2WheelJoint*)world.CreateJoint(&jd);
    
    jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
    jd.motorSpeed = 0.0f;
    jd.maxMotorTorque = 10.0f;
    jd.enableMotor = false;
    jd.frequencyHz = m_hz;
    jd.dampingRatio = m_zeta;
    m_spring2 = (b2WheelJoint*)world.CreateJoint(&jd);
    
    
}

void ZiegenNachAfrikaApp::mouseDown( MouseEvent event )
{
}

void ZiegenNachAfrikaApp::update() {
    // step physics world
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
    mHills->update(p.body->GetPosition());
    
	world.Step(timeStep, velocityIterations, positionIterations);

    
    float left = Conversions::toScreen(m_car->GetPosition().x) - getWindowWidth()/2;
    float right = Conversions::toScreen(m_car->GetPosition().x) + getWindowWidth()/2;
    float top = Conversions::toScreen(m_car->GetPosition().y) - getWindowHeight()/2;
    float bottom = Conversions::toScreen(m_car->GetPosition().y) + getWindowHeight()/2;
    
    mCamera.setOrtho( left, right,bottom, top, -1, 1 );
    

}

void ZiegenNachAfrikaApp::draw()
{
    gl::setMatrices( mCamera );
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    p.draw();
    mHills->draw();
    gl::drawLine( Vec2f(0, getWindowHeight()), Vec2f(getWindowWidth(), getWindowHeight()));
    
    Vec2f pos = Conversions::toScreen( m_wheel1->GetPosition() );
    float t = Conversions::radiansToDegrees( m_wheel1->GetAngle() );
    
    glPushMatrix();
    gl::translate( pos );
    gl::rotate( t );
    
    Rectf rect(-10, -5, 10, 5);
    gl::drawSolidRect(rect);
    
    glPopMatrix();
}

CINDER_APP_BASIC( ZiegenNachAfrikaApp, RendererGl )
