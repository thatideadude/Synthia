#pragma once
#include "Waveform.hpp"

enum	Waveform
{
	Sine,
	Saw,
	Square,
	Triangle
};

enum	FilterMode
{
	LowPass,
	BandPass,
	HighPass
};

enum	Param
{
	P_Waveform,
	P_Cutoff,
	P_Resonance,
	P_EnvAmount,
	P_FilterMode
};

struct	Patch
{
	Waveform	oscAWave;
	float		oscAfine;
	float		cutoff, resonance, filterEnvAmount;
	float		attack = 0.01f;
	float		decay = 0.1f;
	float		sustain = 0.7f;
	float		release = 0.03f;
	float		fltA, fltD, fltS, fltR;
	float		delayTime, delayFeedback, delayMix;
	float		reverbSize, reverbMix;
};
