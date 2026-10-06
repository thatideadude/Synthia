#pragma once

class	Envelope
{
	public:
		enum Stage
		{
			Idle,
			Attack,
			Decay,
			Sustain,
			Release
		};
		Envelope(void);

		void	setSampleRate(double sr);
		void	setParams(float a, float d, float s, float r);
		void	noteOn(void);
		void	noteOff(void);
		float	next(void);

		bool	isActive(void) const { return (_stage != Idle); }
		bool	isReleasing(void) const { return (_stage == Release); }
	private:
		Stage	_stage;
		float	_level;
		double	_sampleRate;
		float	_attack;
		float	_decay;
		float	_sustain;
		float	_release;
		float	_releaseStep;
};
