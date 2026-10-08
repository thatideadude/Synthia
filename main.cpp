#include <iostream>
#include "AudioEngine.hpp"
#include "Synth.hpp"
#include "Terminal.hpp"
#include <cstring>

static int	keyToNote(int c)
{
	static const char	*keys = "awsedftgyhujk";
	const char			*p = std::strchr(keys, c);
	return ((p && c) ? 60 + static_cast<int>(p - keys) : -1);
}

static float	clampf(float v, float lo, float hi)
{
	return (std::min(std::max(v, lo), hi));
}

int	main(void)
{
	Synth		synth;
	AudioEngine	engine(synth);

	if (!engine.start())
	{
		std::cerr << "Could not start audio device\n";
		return (1);
	}
std::cout << "Play with a w s e d f t g y h u j k. Space = release, q = quit.\r\n"
			  << "1-4 wave | [ ] cutoff | - = res | , . env amt | m filter mode\r\n"
			  << "l lfo shape | 7 8 lfo rate | i o lfo>pitch | z x lfo>cutoff\r\n"
			  << "p mono on/off | 5 6 glide\r\n"
			  << "b n delay time | 9 0 delay fb | ; ' delay mix\r\n"
			  << "c v reverb size | { } reverb damp | ( ) reverb mix | r swap order\r\n";

	RawTerminal	raw;
	int		c;
	int		wave = Saw, mode = LowPass, lfoShape = LfoSine;
	bool	mono = false;
	float	cutoff = 800.0f, res = 0.3f, amt = 3.0f;
	float	rate = 5.0f, vib = 0.0f, wob = 0.0f, glide = 0.0f;
	float	dTime = 0.35f, dFb = 0.4f, dMix = 0.3f;
	float	rSize = 0.7f, rDamp = 0.4f, rMix = 0.25f;
	bool	reverbFirst = false;
	while ((c = getchar()) != EOF && c != 'q')
	{
		switch (c)
		{
			case '1': case '2': case '3': case '4':
				wave = c - '1';
				synth.postParam(P_Waveform, static_cast<float>(wave)); break ;
			case '[': cutoff = clampf(cutoff / 1.15f, 40.0f, 18000.0f);
				synth.postParam(P_Cutoff, cutoff); break ;
			case ']': cutoff = clampf(cutoff * 1.15f, 40.0f, 18000.0f);
				synth.postParam(P_Cutoff, cutoff); break ;
			case '-': res = clampf(res - 0.05f, 0.0f, 0.95f);
				synth.postParam(P_Resonance, res); break ;
			case '=': res = clampf(res + 0.05f, 0.0f, 0.95f);
				synth.postParam(P_Resonance, res); break ;
			case ',': amt = clampf(amt - 0.5f, 0.0f, 6.0f);
				synth.postParam(P_EnvAmount, amt); break ;
			case '.': amt = clampf(amt + 0.5f, 0.0f, 6.0f);
				synth.postParam(P_EnvAmount, amt); break ;
			case 'm': mode = (mode + 1) % 3;
				synth.postParam(P_FilterMode, static_cast<float>(mode)); break ;
			case 'l': lfoShape = (lfoShape + 1) % 5;
				synth.postParam(P_LfoShape, static_cast<float>(lfoShape)); break ;
			case '7': rate = clampf(rate / 1.25f, 0.1f, 30.0f);
				synth.postParam(P_LfoRate, rate); break ;
			case '8': rate = clampf(rate * 1.25f, 0.1f, 30.0f);
				synth.postParam(P_LfoRate, rate); break ;
			case 'i': vib = clampf(vib - 0.25f, 0.0f, 7.0f);
				synth.postParam(P_LfoPitch, vib); break ;
			case 'o': vib = clampf(vib + 0.25f, 0.0f, 7.0f);
				synth.postParam(P_LfoPitch, vib); break ;
			case 'z': wob = clampf(wob - 0.25f, 0.0f, 4.0f);
				synth.postParam(P_LfoCutoff, wob); break ;
			case 'x': wob = clampf(wob + 0.25f, 0.0f, 4.0f);
				synth.postParam(P_LfoCutoff, wob); break ;
			case '5': glide = clampf(glide - 0.02f, 0.0f, 1.0f);
				synth.postParam(P_Glide, glide); break ;
			case '6': glide = clampf(glide + 0.02f, 0.0f, 1.0f);
				synth.postParam(P_Glide, glide); break ;
			case 'p': mono = !mono;
				synth.postParam(P_Mono, mono ? 1.0f : 0.0f); break ;
			case 'b': dTime = clampf(dTime / 1.1f, 0.02f, 1.5f);
				synth.postParam(P_DelayTime, dTime); break ;
			case 'n': dTime = clampf(dTime * 1.1f, 0.02f, 1.5f);
				synth.postParam(P_DelayTime, dTime); break ;
			case '9': dFb = clampf(dFb - 0.05f, 0.0f, 0.95f);
				synth.postParam(P_DelayFeedback, dFb); break ;
			case '0': dFb = clampf(dFb + 0.05f, 0.0f, 0.95f);
				synth.postParam(P_DelayFeedback, dFb); break ;
			case ';': dMix = clampf(dMix - 0.05f, 0.0f, 1.0f);
				synth.postParam(P_DelayMix, dMix); break ;
			case '\'': dMix = clampf(dMix + 0.05f, 0.0f, 1.0f);
				synth.postParam(P_DelayMix, dMix); break ;
			case 'c': rSize = clampf(rSize - 0.05f, 0.0f, 1.0f);
				synth.postParam(P_ReverbSize, rSize); break ;
			case 'v': rSize = clampf(rSize + 0.05f, 0.0f, 1.0f);
				synth.postParam(P_ReverbSize, rSize); break ;
			case '{': rDamp = clampf(rDamp - 0.05f, 0.0f, 1.0f);
				synth.postParam(P_ReverbDamp, rDamp); break ;
			case '}': rDamp = clampf(rDamp + 0.05f, 0.0f, 1.0f);
				synth.postParam(P_ReverbDamp, rDamp); break ;
			case '(': rMix = clampf(rMix - 0.05f, 0.0f, 1.0f);
				synth.postParam(P_ReverbMix, rMix); break ;
			case ')': rMix = clampf(rMix + 0.05f, 0.0f, 1.0f);
				synth.postParam(P_ReverbMix, rMix); break ;
			case 'r': reverbFirst = !reverbFirst;
				synth.postParam(P_FxOrder, reverbFirst ? 1.0f : 0.0f); break ;
			case ' ':
				synth.postNoteOff(-1);
				continue ;
			default:
			{
				int	note = keyToNote(c);
				if (note >= 0)
					synth.postNoteOn(note);
				continue ;
			}
		}
		std::cout << (mono ? "MONO" : "POLY") << " glide " << glide
				  << " | wave " << wave << " mode " << mode << " cut " << static_cast<int>(cutoff)
				  << " res " << res << " env " << amt
				  << " | lfo " << lfoShape << " " << rate << "Hz pitch " << vib
				  << " cut " << wob << "\r\n" << std::flush
				  << " | fx " << (reverbFirst ? "REV>DLY" : "DLY>REV")
				  << " dly " << dTime << "s fb " << dFb << " mix " << dMix
				  << " rev " << rSize << "/" << rDamp << " mix " << rMix << "\r\n" << std::flush;
	}
}
