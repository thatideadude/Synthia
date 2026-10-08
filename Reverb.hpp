#pragma once
#include <vector>
#include <cstddef>
#include <cmath>

class	Reverb
{
	public:
		void	setSampleRate(double sr);
		void 	setParams(float size, float damp, float mix);
		float	process(float in);

	private:
		struct	Comb
		{
			std::vector<float>	buf;
			size_t				idx = 0;
			float				store = 0.0f;

			float	process(float in, float feedback, float damp)
			{
				float	out = buf[idx];
				store = out * (1.0f - damp) + store * damp;
				if (std::fabs(store) < 1e-15f)
					store = 0.0f;
				buf[idx] = in + store * feedback;
				if (++idx >= buf.size())
					idx = 0;
				return (out);
			}
		};
		struct	Allpass
		{
			std::vector<float>	buf;
			size_t				idx = 0;

			float	process(float in)
			{
				float	b = buf[idx];
				float	out = b - in;
				buf[idx] = in + b * 0.5f;
				if (++idx >= buf.size())
					idx = 0;
				return (out);
			}
		};

		static const int	NUM_COMBS = 8;
		static const int	NUM_ALLPASS = 4;
		Comb				_combs[NUM_COMBS];
		Allpass				_allpass[NUM_ALLPASS];
		float				_feedback = 0.84f;
		float				_damp = 0.2f;
		float				_mix = 0.25f;
};
