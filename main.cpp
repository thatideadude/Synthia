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
		  << "1-4 wave | [ ] cutoff | - = resonance | , . env amount | m filter mode\r\n";
RawTerminal	raw;
int		c;
int		wave = Saw, mode = LowPass;
float	cutoff = 800.0f, res = 0.3f, amt = 3.0f;

while ((c = getchar()) != EOF && c != 'q')
{
	switch (c)
	{
		case '1': case '2': case '3': case '4':
			wave = c - '1';
			synth.postParam(P_Waveform, static_cast<float>(wave));
			continue ;
		case '[': cutoff = std::max(cutoff / 1.15f, 40.0f);
			synth.postParam(P_Cutoff, cutoff); break ;
		case ']': cutoff = std::min(cutoff * 1.15f, 18000.0f);
			synth.postParam(P_Cutoff, cutoff); break ;
		case '-': res = std::max(res - 0.05f, 0.0f);
			synth.postParam(P_Resonance, res); break ;
		case '=': res = std::min(res + 0.05f, 0.95f);
			synth.postParam(P_Resonance, res); break ;
		case ',': amt = std::max(amt - 0.5f, 0.0f);
			synth.postParam(P_EnvAmount, amt); break ;
		case '.': amt = std::min(amt + 0.5f, 6.0f);
			synth.postParam(P_EnvAmount, amt); break ;
		case 'm': mode = (mode + 1) % 3;
			synth.postParam(P_FilterMode, static_cast<float>(mode)); break ;
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
	std::cout << "wave " << wave << "  mode " << mode << "  cutoff " << static_cast<int>(cutoff)
			  << "  res " << res << "  env " << amt << "\r\n" << std::flush;
}
}
