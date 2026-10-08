#include "Reverb.hpp"
#include <algorithm>

static const int	COMB_TUNING[8] = {1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 };
static const int	ALLPASS_TUNING[4] = {556, 441, 341, 225 };

static float	clamp01(float v)
{
	return (std::min(std::max(v, 0.0f), 1.0f));
}

void	Reverb::setSampleRate(double sr)
{
	double	scale = sr / 44100.0;

	for (int i = 0; i < NUM_COMBS; ++i)
	{
		_combs[i].buf.assign(static_cast<size_t>(COMB_TUNING[i] * scale) + 1, 0.0f);
		_combs[i].idx = 0;
		_combs[i].store = 0.0f;
	}
	for (int i = 0; i < NUM_ALLPASS; ++i)
	{
		_allpass[i].buf.assign(static_cast<size_t>(ALLPASS_TUNING[i] * scale) + 1, 0.0f);
		_allpass[i].idx = 0;
	}
}

void	Reverb::setParams(float size, float damp, float mix)
{
	_feedback = 0.07f + 0.28f * clamp01(size);
	_damp = 0.4f * clamp01(damp);
	_mix = clamp01(mix);
}

float	Reverb::process(float in)
{
	if (_combs[0].buf.empty())
		return (in);
	float	x = in * 0.015f;
	float	wet = 0.0f;

	for (auto &c : _combs)
		wet += c.process(x, _feedback, _damp);
	for (auto &a : _allpass)
		wet = a.process(wet);
	return (in + wet * _mix * 3.0f);
}
