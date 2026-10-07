#pragma once
#include "Patch.hpp"
#include <cstdint>

class	Lfo
{
	public:
		void	setSampleRate(double sr) { _sampleRate = sr; }
		float	next(LfoShape shape, float rate);
	private:
		double		_sampleRate = 48000.0;
		double		_phase = 0.0;
		float		_held = 0.0f;
		uint32_t	_rng = 22222u;
};
