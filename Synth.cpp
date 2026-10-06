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
		else
			noteOff(e.note);
	}
	for (int i = 0; i < frames; ++i)
	{
		float mix = 0.0f;
		for (auto &v : _voices)
			mix += v.render(_patch);
		out[i] = mix * 0.2f;
	}
}
