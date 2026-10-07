#include "Oscillator.hpp"
#include <cmath>

Oscillator::Oscillator(void)
{
	_phase = 0;
	_freq = 440.0;
	_sampleRate = 48000.0;
}

float	Oscillator::next(Waveform w)
{
	double	dt = _freq/ _sampleRate;
	double	out;

	switch(w)
	{
		case	Saw:
			out = 2.0 * _phase - 1.0;
			out -= polyBlep(_phase, dt);
			break ;
		case	Square:
			out = (_phase < 0.5) ? 1.0 : -1.0;
			out += polyBlep(_phase, dt);
			out -= polyBlep(std::fmod(_phase + 0.5f, 1.0), dt);
			break ;
		case	Triangle:
			out = 2.0 * std::fabs(2.0 * _phase - 1.0) - 1.0;
			break ;
		default:
			out = std::sin(2.0 * M_PI * _phase);
			break ;
	}
	_phase += dt;
	if (_phase >= 1.0)
		_phase -= 1.0;
	return (static_cast<float>(out));
}

void	Oscillator::setSampleRate(double sr)
{
	_sampleRate = sr;
}

void	Oscillator::setFrequency(double f)
{
	_freq = f;
}

double	Oscillator::polyBlep(double t, double dt)
{
	if (t < dt)
	{
		t /= dt;
		return (t + t - t * t - 1.0);
	}
	if (t > 1.0 -dt)
	{
		t = (t - 1.0) / dt;
		return (t * t + t + t + 1.0);
	}
	return (0.0);
}
