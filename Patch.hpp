#pragma once

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
};
