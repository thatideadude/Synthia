#pragma once
#include <array>
#include "Delay.hpp"
#include "Reverb.hpp"
#include "Lfo.hpp"
#include "Patch.hpp"
#include "Voice.hpp"
#include "Fx.hpp"
#include "EventQueue.hpp"

class	Synth
{
	public:
		void	setSampleRate(double sr);
		void	postNoteOn(int note);
		void	postNoteOff(int note);
		void	render(float *out, int frames);
		void	postParam(int id, float value);
		void	applyParam(int id, float value);
		void	setMono(bool on);
		void	monoNoteOn(int note);
		void	monoNoteOff(int note);
		void	stackRemove(int note);
	private:
		static const int		STACK_MAX = 16;
		Lfo						_lfo;
		bool					_mono = false;
		int						_stack[STACK_MAX];
		int						_stackSize = 0;
		void					noteOn(int midiNote);
		void					noteOff(int note);
		Voice					*allocateVoice(int note);
		uint64_t				_noteCounter = 0;
		Patch					_patch;
		std::array<Voice, 8>	_voices;
		EventQueue				_events;
		Delay					_delay;
		Reverb					_reverb;
};
