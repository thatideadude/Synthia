#pragma once
#include "Patch.hpp"

class	Filter
{
	public:
		Filter(void) : _sampleRate(48000.0), _ic1(0.0f), _ic2(0.0f) {}
		void	setSampleRate(double sr) { _sampleRate = sr; }
		void	setParams(float cutoff, float resonance);
		float	process(float in, FilterMode mode);

	private:
		double	_sampleRate;
		float	_a1 = 0, _a2 = 0, _a3 = 0, _k = 2;
		float	_ic1, _ic2;
};;
