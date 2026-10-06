#include "Global.hpp"
#include "AudioEngine.hpp"

AudioEngine::AudioEngine(Synth &synth)
	: _synth(synth)
{
 _initialized = false;
}

AudioEngine::~AudioEngine(void)
{
	stop();
}

void	AudioEngine::callback(ma_device *dev, void *out, const void *, ma_uint32 frames)
{
	Synth *synth = static_cast<Synth *>(dev->pUserData);
	synth->render(static_cast<float *>(out), static_cast<int>(frames));
}

bool	AudioEngine::start(void)
{
	ma_device_config	cfg = ma_device_config_init(ma_device_type_playback);
	cfg.playback.format	= ma_format_f32;
	cfg.playback.channels = 1;
	cfg.sampleRate = 0;
	cfg.dataCallback = callback;
	cfg.pUserData = &_synth;

	if (ma_device_init(NULL, &cfg, &_dev) != MA_SUCCESS)
		return (false);
	_initialized = true;
	_synth.setSampleRate(_dev.sampleRate);
	return (ma_device_start(&_dev) == MA_SUCCESS);
}

void	AudioEngine::stop(void)
{
	if (_initialized)
	{
		ma_device_uninit(&_dev);
		_initialized = false;
	}
}
