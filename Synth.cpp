#include "Synth.hpp"
#include <cmath>

void	Synth::setSampleRate(double sr)
{
	_lfo.setSampleRate(sr);
	_delay.setSampleRate(sr);
	_reverb.setSampleRate(sr);
	for (auto &v : _voices)
		v.setSampleRate(sr);
}

Voice	*Synth::allocateVoice(int note)
{
	for (auto &v : _voices)
		if (v.isHeld() && v.note() == note)
			return (&v);
	for (auto &v : _voices)
		if (!v.isActive())
			return (&v);
	Voice	*victim = nullptr;
	for (auto &v : _voices)
		if (!v.isReleasing() && (!victim || v.age() < victim->age()))
			victim = &v;
	if (victim)
		return (victim);
	victim = &_voices[0];
	for (auto &v : _voices)
		if (v.age() < victim->age())
			victim = &v;
	return (victim);
}

void	Synth::noteOn(int note)
{
	if (_mono)
	{
		monoNoteOn(note);
		return ;
	}
	Voice	*v = allocateVoice(note);
	v->noteOn(note, ++_noteCounter);
}

void	Synth::noteOff(int note)
{
	if (_mono)
	{
		monoNoteOff(note);
		return ;
	}
	for (auto &v : _voices)
		if (v.isHeld() && (note < 0 || v.note() == note))
			v.noteOff();
}

void	Synth::postNoteOn(int note)
{
	Event	e;
	e.type = Event::NoteOn;
	e.note = note;
	_events.push(e);
}

void	Synth::postNoteOff(int note)
{
	Event e;
	e.type = Event::NoteOff;
	e.note = note;
	_events.push(e);
}


void Synth::render(float *out, int frames)
{
	Event e;
	while (_events.pop(e))
	{
		if (e.type == Event::NoteOn)
			noteOn(e.note);
		else if (e.type == Event::NoteOff)
			noteOff(e.note);
		else
			applyParam(e.param, e.value);
	}
	_delay.setParams(_patch.delayTime, _patch.delayFeedback, _patch.delayMix);
	_reverb.setParams(_patch.reverbSize, _patch.reverbDamp, _patch.reverbMix);
	const bool	reverbFirst = _patch.reverbFirst;
	for (int i = 0; i < frames; ++i)
	{
		float	lfo = _lfo.next(_patch.lfoShape, _patch.lfoRate);
		float 	mix = 0.0f;
		for (auto &v : _voices)
			mix += v.render(_patch, lfo);

		float	s = mix * 0.2f;
		if (reverbFirst)
		{
			s = _reverb.process(s);
			s = _delay.process(s);
		}
		else
		{
			s = _delay.process(s);
			s = _reverb.process(s);
		}
		out[i] = s;
	}
}

void	Synth::postParam(int id, float value)
{
	Event 	e;
	e.type = Event::SetParam;
	e.note = 0;
	e.param = id;
	e.value = value;
	_events.push(e);
}

void	Synth::stackRemove(int note)
{
	int	j = 0;
	for (int i = 0; i < _stackSize; ++i)
		if (_stack[i] != note)
			_stack[j++] = _stack[i];
	_stackSize = j;
}

void	Synth::monoNoteOn(int note)
{
	stackRemove(note);
	if (_stackSize == STACK_MAX)
	{
		for (int i = 1; i < _stackSize; ++i)
			_stack[i - 1] = _stack[i];
		--_stackSize;
	}
	_stack[_stackSize++] = note;

	Voice	&v = _voices[0];
	if (v.isHeld())
		v.glideTo(note);
	else
		v.noteOn(note, ++_noteCounter);
}

void	Synth::monoNoteOff(int note)
{
	Voice	&v = _voices[0];
	if (note < 0)
	{
		_stackSize = 0;
		v.noteOff();
		return ;
	}
	stackRemove(note);
	if (_stackSize > 0)
		v.glideTo(_stack[_stackSize - 1]);
	else
		v.noteOff();
}

void	Synth::setMono(bool on)
{
	if (on == _mono)
		return ;
	for (auto &x :_voices)
		if (x.isHeld())
			x.noteOff();
	_stackSize = 0;
	_mono = on;
}

void	Synth::applyParam(int id, float v)
{
	switch (id)
	{
		case P_Waveform			: _patch.waveform = static_cast<Waveform>(static_cast<int>(v)); break ;
		case P_Cutoff			: _patch.cutoff = v; break;
		case P_Resonance		: _patch.resonance = v; break ;
		case P_EnvAmount		: _patch.envAmount = v; break ;
		case P_FilterMode		: _patch.filterMode = static_cast<FilterMode>(static_cast<int>(v)); break ;
		case P_LfoShape			: _patch.lfoShape = static_cast<LfoShape>(static_cast<int>(v)); break ;
		case P_LfoRate			: _patch.lfoRate = v; break ;
		case P_LfoPitch			: _patch.lfoPitch = v; break ;
		case P_LfoCutoff		: _patch.lfoCutoff = v; break ;
		case P_Glide			: _patch.glide = v; break ;
		case P_Mono				: setMono(v >= 0.5f); break ;
		case P_DelayTime		: _patch.delayTime = v; break ;
		case P_DelayFeedback 	: _patch.delayFeedback = v; break ;
		case P_DelayMix			: _patch.delayMix = v; break ;
		case P_ReverbSize		: _patch.reverbSize = v; break ;
		case P_ReverbDamp		: _patch.reverbDamp = v; break ;
		case P_ReverbMix		: _patch.reverbMix = v; break ;
		case P_FxOrder			: _patch.reverbFirst = (v >= 0.5f); break ;
	}
}
