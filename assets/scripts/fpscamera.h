#pragma once

#include "meteor.h"

using namespace meteor;

class FPSCamera : public meteor::Script
{
public:
	virtual void OnCreate() override;
	virtual void OnUpdate(meteor::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	mtrFloat m_MouseSensitivity = 7.0f;
};