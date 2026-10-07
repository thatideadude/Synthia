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
	_sampleRate = sr;
	_osc.setSampleRate(sr);
	_env.setSampleRate(sr);
	_fEnv.setSampleRate(sr);
	_filter.setSampleRate(sr);
}

void	Voice::noteOn(int note, uint64_t age)
{
	_note = note;
	_held = true;
	_age = age;
	_pitch = _target = static_cast<float>(note);
	_env.noteOn();
	_fEnv.noteOn();
}

void	Voice::glideTo(int note)
{
	_note = note;
	_target = static_cast<float>(note);
}

void	Voice::noteOff(void)
{
	_held = false;
	_env.noteOff();
	_fEnv.noteOff();
}

float	Voice::render(const Patch &p, float lfo)
{
	if (!_env.isActive())
		return (0.0f);
	if (_pitch != _target)
	{
		float	coef = (p.glide < 0.001f) ? 1.0f : 1.0f - std::exp(-1.0f / (p.glide * static_cast<float>(_sampleRate)));
		_pitch += (_target - _pitch) * coef;
		if (std::fabs(_target - _pitch) < 0.001f)
			_pitch = _target;
	}
	float	semis = _pitch + p.lfoPitch * lfo;
	_osc.setFrequency(440.0 * std::pow(2.0, (semis - 69.0) / 12));
	_env.setParams(p.attack, p.decay, p.sustain, p.release);
	_fEnv.setParams(p.fAttack, p.fDecay, p.fSustain, p.fRelease);

	float	cutoff = p.cutoff * std::pow(2.0f, p.envAmount * _fEnv.next() + p.lfoCutoff * lfo);
	_filter.setParams(cutoff, p.resonance);
	float	s = _osc.next(p.waveform);
	s = _filter.process(s, p.filterMode);
	return (s * _env.next());
}
