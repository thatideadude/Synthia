#include "Lfo.hpp"
#include <cmath>

float	Lfo::next(LfoShape shape, float rate)
{
	float	p = static_cast<float>(_phase);
	float	out;

	switch (shape)
	{
		case LfoTriangle	: out = 1.0f - 4.0f * std::fabs(p - 0.05f); break ;
		case LfoSquare		: out = (p < 0.5f) ? 1.0f : -1.0f; break ;
		case LfoSaw			: out = 2.0f * p - 1.0f; break ;
		case LfoSampleHold	: out = _held; break;
		default				: out = std::sin(2.0f * static_cast<float>(M_PI) * p); break ;
	}
	_phase += rate / _sampleRate;
	if (_phase >= 1.0)
	{
		_phase -= 1.0;
		_rng ^= _rng << 13;
		_rng ^= _rng >> 17;
		_rng ^= _rng << 5;
		_held = static_cast<int32_t>(_rng) / 2147483648.0f;
	}
	return (out);
}
