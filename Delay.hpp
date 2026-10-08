#pragma once
#include <vector>
#include <cstddef>


class	Delay
{
	public:
		void	setSampleRate(double sr);
		void	setParams(float time, float feedback, float mix);
		float	process(float in);
	private:
		static constexpr float	MAX_TIME = 2.0f;

		std::vector<float>		_buf;
		size_t					_write = 0;
		double					_sampleRate = 48000.0;
		float					_cur = 0.0f;
		float					_target = 0.0f;
		float					_feedback = 0.04f;
		float					_mix = 0.3f;
		float					_lp = 0.0f;
};
