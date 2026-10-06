#include "Envelope.hpp"
#include <algorithm>

static const float MIN_TIME = 0.001f;

Envelope::Envelope(void)
{
	_stage = Idle;
	_level = 0.0f;
	_sampleRate = 480000.0;
	_attack = 0.01f;
	_decay = 0.07f;
	_sustain = 0.7f;
	_release = 0.03f;
	_releaseStep = 0.0f;
}

void	Envelope::setSampleRate(double sr)
{
	_sampleRate = sr;
}

void	Envelope::setParams(float a, float d, float s, float r)
{
	_attack = std::max(a, MIN_TIME);
	_decay = std::max(d, MIN_TIME);
	_sustain = std::min(std::max(s, 0.0f), 1.0f);
	_release = std::max(r, MIN_TIME);
}

void	Envelope::noteOn(void)
{
	_stage = Attack;
}

void	Envelope::noteOff(void)
{
	if (_stage == Idle || _stage == Release)
		return ;
	if (_level <= 0.0001f)
	{
		_level = 0.0f;
		_stage = Idle;
		return ;
	}
	_releaseStep = _level / static_cast<float>(_release * _sampleRate);
	_stage = Release;
}

float	Envelope::next(void)
{
	switch (_stage)
	{
		case	Attack:
			_level += 1.0f / static_cast<float>(_attack * _sampleRate);
			if (_level >= 1.0f)
			{
				_level = 1.0f;
				_stage = Decay;
			}
			break ;
		case	Decay:
			_level -= (1.0f - _sustain) / static_cast<float>(_decay * _sampleRate);
			if (_level <= _sustain)
			{
				_level = _sustain;
				_stage = Sustain;
			}
			break ;
		case	Sustain:
			_level = _sustain;
			break ;
		case	Release:
			_level -= _releaseStep;
			if (_level <= 0.0f)
			{
				_level = 0.0f;
				_stage = Idle;
			}
			break ;
		case	Idle:
			break ;
	}
	return (_level);

}
