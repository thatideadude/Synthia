#include "Oscillator.hpp"
#include <cmath>

Oscillator::Oscillator(void)
{
	_phase = 0;
	_freq = 440.0;
	_sampleRate = 48000.0;
}

float	Oscillator::next(void)
{
	float	out = static_cast<float>(std::sin(2.0 * M_PI * _phase));
	_phase += _freq/ _sampleRate;
	if (_phase >= 1)
		_phase -= 1;
	return (out);
}

void	Oscillator::setSampleRate(double sr)
{
	_sampleRate = sr;
}

void	Oscillator::setFrequency(double f)
{
	_freq = f;
}
