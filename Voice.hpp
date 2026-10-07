#pragma once
#include <cstdint>
#include "Oscillator.hpp"
#include "Filter.hpp"
#include "Envelope.hpp"

struct Patch;

class	Voice
{
	public:
		Voice(void);

		void	setSampleRate(double sr);
		void	noteOn(int note, uint64_t age);
		void	glideTo(int note);
		void	noteOff(void);
		float	render(const Patch &p, float lfo);

		bool		isActive(void) const { return (_env.isActive()); }
		bool		isReleasing(void) const { return (_env.isReleasing()); }
		bool		isHeld(void) const { return (_held); }
		int			note(void) const { return (_note); }
		uint64_t	age(void) const { return (_age); }
	private:
		Oscillator	_osc;
		Envelope	_env;
		Filter		_filter;
		Envelope	_fEnv;
		int			_note;
		bool		_held;
		uint64_t	_age;
		double		_sampleRate = 48000.0;
		float		_pitch = 60.0f;
		float		_target = 60.0f;
};
