#include "Voice.hpp"
#include "Patch.hpp"

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
}

void	Voice::noteOn(int note, float freq, uint64_t age)
{
	_note = note;
	_held = true;
	_age = age;
	_osc.setFrequency(freq);
	_env.noteOn();
}

void	Voice::noteOff(void)
{
	_held = false;
	_env.noteOff();
}

float	Voice::render(const Patch &p)
{
	if (!_env.isActive())
		return (0.0f);
	_env.setParams(p.attack, p.decay, p.sustain, p.release);
	return (_osc.next() * _env.next());
}
