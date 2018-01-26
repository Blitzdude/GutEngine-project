#include "Player.h"
#include <SDL/SDL.h>



Player::Player()
{
	
}

Player::~Player()
{

}

void 
Player::init(
	b2World * world,
	const glm::vec2 position,
	const glm::vec2 dimensions,
	Gutengine::GLTexture texture,
	Gutengine::ColorRGBA8 color,
	bool fixedRotation,
	b2BodyType bodyType,
	glm::vec4 uvRect)
{
	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;

	// make body.
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(position.x, position.y);	// set bodys position
	bodyDef.fixedRotation = fixedRotation;			// sets if the body rotates
	m_body = world->CreateBody(&bodyDef);			// use world to create the body 

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f); // Box2D creates box by half the length and width

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void
Player::draw(Gutengine::SpriteBatch spriteBatch)
{
	glm::vec4 destRect;	// create the destination rectangle
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	spriteBatch.draw(destRect,					// Destination rectangle
		m_uvRect,								// Uv-rectangle x,y,w,h
		m_texture.id, 0.0f,						// texture to draw
		m_color,								// color
		m_body->GetAngle());					// rotation of the spritebatch
}

void
Player::update(Gutengine::InputManager& inputManager, Gutengine::Camera2D camera)
{
	b2Body* body = m_body; // reference to body

	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	if (inputManager.isKeyDown(SDLK_w)) {
		body->ApplyForceToCenter(b2Vec2(0.0f, 100.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_s)) {
		body->ApplyForceToCenter(b2Vec2(0.0f, -100.0f), true);
	}
	else { // apply damping - if no button is pressed, deaccelerate the player
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y * 0.95f));
	}

	// set maximum speed in the x direction
	if (body->GetLinearVelocity().x < -6.0f) {
		body->SetLinearVelocity(b2Vec2(-6.0f, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > 6.0f) {
		body->SetLinearVelocity(b2Vec2(6.0f, body->GetLinearVelocity().y));
	}

	//set maximum speed in the y direction
	if (body->GetLinearVelocity().y < -6.0f) {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -6.0f));
	}
	else if (body->GetLinearVelocity().y > 6.0f) {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 6.0f));
	}

	//get mouse coordinates and set turn the player sprite to face there

	glm::vec2 mouseCoord = camera.convertScreenToWorld(inputManager.getMouseCoords());

	glm::vec2 playerCoord = glm::vec2(body->GetPosition().x, body->GetPosition().y);


	m_direction = glm::normalize(mouseCoord - playerCoord);

	// get the angle to the desired location
	float desiredAngle = atan2f(-m_direction.x, m_direction.y);


	// set the angle of the collision box, where the sprite resides
	body->SetTransform(body->GetPosition(), desiredAngle + 1.5f);

}

