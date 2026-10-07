#include "Voice.hpp"
#include "Patch.hpp"
#include <cmath>

Voice::Voice(void)
{
	_note = -1;
	_held = false;
	_age = 0;

}

void	Voice::setSampleRate(double sr)
{
	_osc.setSampleRate(sr);
	_env.setSampleRate(sr);
	_fEnv.setSampleRate(sr);
	_filter.setSampleRate(sr);
}

void	Voice::noteOn(int note, float freq, uint64_t age)
{
	_note = note;
	_held = true;
	_age = age;
	_osc.setFrequency(freq);
	_env.noteOn();
	_fEnv.noteOn();
}

void	Voice::noteOff(void)
{
	_held = false;
	_env.noteOff();
	_fEnv.noteOff();
}

float	Voice::render(const Patch &p)
{
	if (!_env.isActive())
		return (0.0f);
	_env.setParams(p.attack, p.decay, p.sustain, p.release);
	_fEnv.setParams(p.fAttack, p.fDecay, p.fSustain, p.fRelease);
	float	cutoff = p.cutoff * std::pow(2.0f, p.envAmount * _fEnv.next());
	_filter.setParams(cutoff, p.resonance);
	float	s = _osc.next(p.waveform);
	s = _filter.process(s, p.filterMode);
	return (s * _env.next());
}
