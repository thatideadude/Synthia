#pragma once
#include "miniaudio.h"
#include "Synth.hpp"

class	AudioEngine
{
	public:
		AudioEngine(Synth &Synth);
		~AudioEngine(void);
		bool	start(void);
		void	stop(void);
	private:
		static void	callback(ma_device *dev, void *out, const void *, ma_uint32 frames);
		Synth		&_synth;
		ma_device	_dev;
		bool		_initialized;
};
