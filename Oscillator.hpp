#pragma once

class	Oscillator
{
	public:
		void	setSampleRate(double sr);
		void	setFrequency(double f);
		float	next(void);
	private:
		double	_phase;
		double	_freq;
		double	_sampleRate;
	public:
		Oscillator(void);
};
