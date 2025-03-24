#pragma once
#ifndef TIMESTEP_H
#define TIMESTEP_H

namespace overtime {
	class timeStep {
	public:
		timeStep(float time = 0.0f) : m_Time(time) {}

		operator float() const { return m_Time; }

		float getSeconds() const { return m_Time; }
		float getMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};

}

#endif