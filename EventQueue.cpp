#include "Global.hpp"
#include "EventQueue.hpp"

bool	EventQueue::push(const Event &e)
{
	size_t	h = _head.load(std::memory_order_relaxed);
	size_t	next = (h + 1) % N;
	if (next == _tail.load(std::memory_order_acquire))
		return (false);
	_buf[h] = e;
	_head.store(next, std::memory_order_release);
	return (true);
}

bool	EventQueue::pop(Event &e)
{
	size_t t = _tail.load(std::memory_order_relaxed);
	if (t == _head.load(std::memory_order_acquire))
		return (false);
	e = _buf[t];
	_tail.store((t + 1) % N, std::memory_order_release);
	return (true);
}
