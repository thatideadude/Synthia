#pragma once
#include <termios.h>
#include <unistd.h>

class	RawTerminal
{
	public:
		RawTerminal(void)
		{
			tcgetattr(STDIN_FILENO, &_old);
			termios	raw = _old;
			raw.c_lflag &= ~(ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &raw);
		}
		~RawTerminal(void) { tcsetattr(STDIN_FILENO, TCSANOW, &_old); }
	private:
		termios	_old;
};
