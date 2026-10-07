#include "Filter.hpp"
#include <cmath>
#include <algorithm>

void	Filter::setParams(float cutoff, float resonance)
{
	float	fc = std::min(std::max(cutoff, 20.0f), static_cast<float>(_sampleRate * 0.45));
	float	res = std::min(std::max(resonance, 0.0f), 0.98f);
	float	g = std::tan(static_cast<float>(M_PI) * fc / static_cast<float>(_sampleRate));

	_k = 2.0f - 2.0f * res;
	_a1 = 1.0f / (1.0f + g * (g + _k));
	_a2 = g * _a1;
	_a3 = g * _a2;
}

float	Filter::process(float in, FilterMode mode)
{
	float	v3 = in - _ic2;
	float	v1 = _a1 * _ic1 + _a2 * v3;
	float	v2 = _ic2 + _a2 * _ic1 + _a3 * v3;
	_ic1 = 2.0f * v1 - _ic1;
	_ic2 = 2.0f * v2 - _ic2;
	if (mode == BandPass)
		return (v1);
	if (mode == HighPass)
		return (in - _k * v1 - v2);
	return (v2);
}
