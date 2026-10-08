#include "Delay.hpp"
#include <cmath>
#include <algorithm>

constexpr float Delay::MAX_TIME;

void	Delay::setSampleRate(double sr)
{
	_sampleRate = sr;
	_buf.assign(static_cast<size_t>(sr * MAX_TIME) + 2, 0.0f);
	_write = 0;
	_lp = 0.0f;
	_cur = _target = static_cast<float>(0.35 * sr);
}

void	Delay::setParams(float time, float feedback, float mix)
{
	float	maxSamples = static_cast<float>(_buf.size()) - 2.0f;
	float	t = std::min(std::max(time, 0.02f), MAX_TIME);

	_target = std::min(t * static_cast<float>(_sampleRate), std::max(maxSamples, 1.0f));
	_feedback = std::min(std::max(feedback, 0.0f), 0.95f);
	_mix = std::min(std::max(mix, 0.0f), 1.0f);
}

float	Delay::process(float in)
{
	if (_buf.empty())
		return (in);

	size_t	size = _buf.size();
	_cur += (_target - _cur) * 0.0005f;

	float	rp = static_cast<float>(_write) - _cur;
	if (rp < 0.0f)
		rp += static_cast<float>(size);
	size_t	i0 = static_cast<size_t>(rp) % size;
	size_t	i1 = (i0 + 1) % size;
	size_t	fr = rp - std::floor(rp);
	float	d = _buf[i0] * (1.0f - fr) + _buf[i1] * fr;

	_lp += 0.6f * (d - _lp);
	if (std::fabs(_lp) < 1e-5f)
		_lp = 0.0f;

	_buf[_write] = in + _lp * _feedback;
	_write = (_write + 1) % size;
	return (in + d * _mix);
}
