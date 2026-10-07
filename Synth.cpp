#include "Synth.hpp"
#include <cmath>

static float	noteToFreq(int note)
{
	return (440.0f * std::pow(2.0f, (note - 69) / 12.0f));
}

void	Synth::setSampleRate(double sr)
{
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
	Voice	*v = allocateVoice(note);
	v->noteOn(note, noteToFreq(note), ++_noteCounter);
}

void	Synth::noteOff(int note)
{
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
	for (int i = 0; i < frames; ++i)
	{
		float mix = 0.0f;
		for (auto &v : _voices)
			mix += v.render(_patch);
		out[i] = mix * 0.2f;
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

void	Synth::applyParam(int id, float v)
{
	switch (id)
	{
		case P_Waveform		: _patch.waveform = static_cast<Waveform>(static_cast<int>(v)); break ;
		case P_Cutoff		: _patch.cutoff = v; break;
		case P_Resonance	: _patch.resonance = v; break ;
		case P_EnvAmount	: _patch.envAmount = v; break ;
		case P_FilterMode	: _patch.filterMode = static_cast<FilterMode>(static_cast<int>(v)); break ;
	}
}
