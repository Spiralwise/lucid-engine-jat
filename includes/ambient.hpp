#ifndef _LUCID_LIGHT_AMBIENT_
#define _LUCID_LIGHT_AMBIENT_

#include "light.hpp"


class AmbientLight : public Light
{

public:
	// Constructor
	AmbientLight (const glm::vec3&, const float power=1.0f);

};

#endif