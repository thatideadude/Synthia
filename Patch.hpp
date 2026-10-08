#pragma once

enum	LfoShape
{
	LfoSine,
	LfoTriangle,
	LfoSquare,
	LfoSaw,
	LfoSampleHold
};

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
	P_FilterMode,
	P_LfoShape,
	P_LfoRate,
	P_LfoPitch,
	P_LfoCutoff,
	P_Glide,
	P_Mono,
	P_DelayTime,
	P_DelayFeedback,
	P_DelayMix,
	P_ReverbSize,
	P_ReverbDamp,
	P_ReverbMix,
	P_FxOrder
};

struct	Patch
{
	float		attack = 0.01f;
	float		decay = 0.1f;
	float		sustain = 0.7f;
	float		release = 0.03f;
	float		cutoff = 800.0f;
	float		resonance = 0.3f;
	float		envAmount = 3.0f;
	float		fAttack = 0.25f;
	float		fDecay = 0.25f;
	float		fSustain = 0.2f;
	float		fRelease = 0.1f;
	Waveform	waveform;
	FilterMode	filterMode;
	LfoShape	lfoShape = LfoSine;
	float		lfoRate = 5.0f;
	float		lfoPitch = 0.0f;
	float		lfoCutoff = 0.0f;
	float		glide = 0.0f;
	float		delayTime = 0.35f;
	float		delayFeedback = 0.4f;
	float		delayMix = 0.3f;
	float		reverbSize = 0.7f;
	float		reverbDamp = 0.4f;
	float		reverbMix = 0.25f;
	bool		reverbFirst = false;
};
