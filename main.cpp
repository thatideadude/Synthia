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
	std::cout << "Play with a w s e d f t g y h u j k. Space = release, q = quit.\n";
	RawTerminal	raw;
	int c;
	int note;
	while ((c = getchar()) != EOF && c != 'q')
	{
	    if (c != ' ' && c != 'q') note = keyToNote(c);
	    if (c == ' ')
	        synth.postNoteOff(note);
	    else
	    {
	        if (note >= 0)
	            synth.postNoteOn(note);
	    }
	}
}
