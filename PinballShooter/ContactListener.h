#pragma once
#include <Box2D/Box2D.h>
#include "Pinball.h"

class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) override {
		//check if fixture A was a pinball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<Pinball*>(bodyUserData)->startContact();

		//check if fixture B was a pinball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<Pinball*>(bodyUserData)->startContact();

	}

	void EndContact(b2Contact* contact) override {

		//check if fixture A was a ballpin
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<Pinball*>(bodyUserData)->endContact();

		//check if fixture B was a ballpin
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<Pinball*>(bodyUserData)->endContact();

	}

};
