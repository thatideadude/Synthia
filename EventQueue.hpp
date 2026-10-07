#pragma once
#include <atomic>
#include <array>
#include <cstddef>

struct Event
{
	enum	Type
	{
		NoteOn,
		NoteOff,
		SetParam
	} type;
	int		note;
	int		param;
	int		value;
};

class	EventQueue
{
	public:
		bool	push(const Event &e);
		bool	pop(Event &e);
	private:
		static constexpr	size_t N = 256;
		std::array<Event, N>	_buf;
		std::atomic<size_t>		_head {0}, _tail{0};
};
