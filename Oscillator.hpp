#pragma once
#include "Patch.hpp"

class	Oscillator
{
	public:
		void			setSampleRate(double sr);
		void			setFrequency(double f);
		static double	polyBlep(double t, double dt);
		float			next(Waveform w);
	private:
		double	_phase;
		double	_freq;
		double	_sampleRate;
	public:
		Oscillator(void);
};
