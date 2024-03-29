/*
	audio_alsa: sound output with Advanced Linux Sound Architecture 1.x API

	copyright 2006 by the mpg123 project - free software under the terms of the LGPL 2.1
	see COPYING and AUTHORS files in distribution or http://mpg123.de

	written by Clemens Ladisch <clemens@ladisch.de>
*/

#include "config.h"
#include "mpg123.h"
#include "debug.h"
#include <errno.h>

/* make ALSA 0.9.x compatible to the 1.0.x API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#define ALSA_PCM_NEW_SW_PARAMS_API

#ifdef ASOUNDLIB_H
#include <alsa/asoundlib.h>
#endif
#ifdef ASOUND_H
#include <sound/asound.h>
#endif

/* My laptop has probs playing low-sampled files with only 0.5s buffer... this should be a user setting -- ThOr */
#define BUFFER_LENGTH 0.5	/* in seconds */

static const struct {
	snd_pcm_format_t alsa;
	int mpg123;
} format_map[] = {
	{ SND_PCM_FORMAT_S16,    AUDIO_FORMAT_SIGNED_16   },
	{ SND_PCM_FORMAT_U16,    AUDIO_FORMAT_UNSIGNED_16 },
	{ SND_PCM_FORMAT_U8,     AUDIO_FORMAT_UNSIGNED_8  },
	{ SND_PCM_FORMAT_S8,     AUDIO_FORMAT_SIGNED_8    },
	{ SND_PCM_FORMAT_A_LAW,  AUDIO_FORMAT_ALAW_8      },
	{ SND_PCM_FORMAT_MU_LAW, AUDIO_FORMAT_ULAW_8      },
};
#define NUM_FORMATS (sizeof format_map / sizeof format_map[0])

static int initialize_device(struct audio_info_struct *ai);
static int initialize_mixer_device(struct audio_info_struct *ai, char *error);

int audio_open(struct audio_info_struct *ai)
{
	const char *pcm_name;
	snd_pcm_t *pcm;

	pcm_name = ai->device ? ai->device : "default";
	if (snd_pcm_open(&pcm, pcm_name, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		fprintf(stderr, "audio_open(): cannot open device %s\n", pcm_name);
		return -1;
	}
	ai->handle = pcm;
	if (ai->format != -1) {
		/* we're going to play: initalize sample format */
                initialize_mixer_device(ai, NULL);
		return initialize_device(ai);
	} else {
		/* query mode; sample format will be set for each query */
		return 0;
	}
}

static int rates_match(long int desired, unsigned int actual)
{
	return actual * 100 > desired * (100 - AUDIO_RATE_TOLERANCE) &&
	       actual * 100 < desired * (100 + AUDIO_RATE_TOLERANCE);
}

static int initialize_device(struct audio_info_struct *ai)
{
	snd_pcm_hw_params_t *hw;
	int i;
	snd_pcm_format_t format;
	unsigned int rate;
	snd_pcm_uframes_t buffer_size;
	snd_pcm_uframes_t period_size;
	snd_pcm_sw_params_t *sw;
	snd_pcm_uframes_t boundary;

	snd_pcm_hw_params_alloca(&hw);
	if (snd_pcm_hw_params_any(ai->handle, hw) < 0) {
		fprintf(stderr, "initialize_device(): no configuration available\n");
		return -1;
	}
	if (snd_pcm_hw_params_set_access(ai->handle, hw, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		fprintf(stderr, "initialize_device(): device does not support interleaved access\n");
		return -1;
	}
	format = SND_PCM_FORMAT_UNKNOWN;
	for (i = 0; i < NUM_FORMATS; ++i) {
		if (ai->format == format_map[i].mpg123) {
			format = format_map[i].alsa;
			break;
		}
	}
	if (format == SND_PCM_FORMAT_UNKNOWN) {
		fprintf(stderr, "initialize_device(): invalid sample format %d\n", ai->format);
		errno = EINVAL;
		return -1;
	}
	if (snd_pcm_hw_params_set_format(ai->handle, hw, format) < 0) {
		fprintf(stderr, "initialize_device(): cannot set format %s\n", snd_pcm_format_name(format));
		return -1;
	}
	if (snd_pcm_hw_params_set_channels(ai->handle, hw, ai->channels) < 0) {
		fprintf(stderr, "initialize_device(): cannot set %d channels\n", ai->channels);
		return -1;
	}
	rate = ai->rate;
	if (snd_pcm_hw_params_set_rate_near(ai->handle, hw, &rate, NULL) < 0) {
		fprintf(stderr, "initialize_device(): cannot set rate %u\n", rate);
		return -1;
	}
	if (!rates_match(ai->rate, rate)) {
		fprintf(stderr, "initialize_device(): rate %ld not available, using %u\n", ai->rate, rate);
		/* return -1; */
	}
	buffer_size = rate * BUFFER_LENGTH;
	if (snd_pcm_hw_params_set_buffer_size_near(ai->handle, hw, &buffer_size) < 0) {
		fprintf(stderr, "initialize_device(): cannot set buffer size\n");
		return -1;
	}
	period_size = buffer_size / 4;
	if (snd_pcm_hw_params_set_period_size_near(ai->handle, hw, &period_size, NULL) < 0) {
		fprintf(stderr, "initialize_device(): cannot set period size\n");
		return -1;
	}
	if (snd_pcm_hw_params(ai->handle, hw) < 0) {
		fprintf(stderr, "initialize_device(): cannot set hw params\n");
		return -1;
	}

	snd_pcm_sw_params_alloca(&sw);
	if (snd_pcm_sw_params_current(ai->handle, sw) < 0) {
		fprintf(stderr, "initialize_device(): cannot get sw params\n");
		return -1;
	}
	/* start playing after the first write */
	if (snd_pcm_sw_params_set_start_threshold(ai->handle, sw, 1) < 0) {
		fprintf(stderr, "initialize_device(): cannot set start threshold\n");
		return -1;
	}
	if (snd_pcm_sw_params_get_boundary(sw, &boundary) < 0) {
		fprintf(stderr, "initialize_device(): cannot get boundary\n");
		return -1;
	}
	/* never stop on underruns */
	if (snd_pcm_sw_params_set_stop_threshold(ai->handle, sw, boundary) < 0) {
		fprintf(stderr, "initialize_device(): cannot set stop threshold\n");
		return -1;
	}
	/* wake up on every interrupt */
	if (snd_pcm_sw_params_set_avail_min(ai->handle, sw, 1) < 0) {
		fprintf(stderr, "initialize_device(): cannot set min avail\n");
		return -1;
	}
#if 0
	/* always write as many frames as possible */
	if (snd_pcm_sw_params_set_xfer_align(ai->handle, sw, 1) < 0) {
		fprintf(stderr, "initialize_device(): cannot set transfer alignment\n");
		return -1;
	}
#endif
	/* play silence when there is an underrun */
	if (snd_pcm_sw_params_set_silence_size(ai->handle, sw, boundary) < 0) {
		fprintf(stderr, "initialize_device(): cannot set silence size\n");
		return -1;
	}
	if (snd_pcm_sw_params(ai->handle, sw) < 0) {
		fprintf(stderr, "initialize_device(): cannot set sw params\n");
		return -1;
	}
	return 0;
}

int audio_get_formats(struct audio_info_struct *ai)
{
	snd_pcm_hw_params_t *hw;
	unsigned int rate;
	int supported_formats, i;

	snd_pcm_hw_params_alloca(&hw);
	if (snd_pcm_hw_params_any(ai->handle, hw) < 0) {
		fprintf(stderr, "audio_get_formats(): no configuration available\n");
		return -1;
	}
	if (snd_pcm_hw_params_set_access(ai->handle, hw, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
		return -1;
	if (snd_pcm_hw_params_set_channels(ai->handle, hw, ai->channels) < 0)
		return 0;
	rate = ai->rate;
	if (snd_pcm_hw_params_set_rate_near(ai->handle, hw, &rate, NULL) < 0)
		return -1;
	if (!rates_match(ai->rate, rate))
		return 0;
	supported_formats = 0;
	for (i = 0; i < NUM_FORMATS; ++i) {
		if (snd_pcm_hw_params_test_format(ai->handle, hw, format_map[i].alsa) == 0)
			supported_formats |= format_map[i].mpg123;
	}
	return supported_formats;
}

int audio_play_samples(struct audio_info_struct *ai, unsigned char *buf, int bytes)
{
	snd_pcm_uframes_t frames;
	snd_pcm_sframes_t written;

#if SND_LIB_VERSION >= 0x000901
	snd_pcm_sframes_t delay;
	if (snd_pcm_delay(ai->handle, &delay) >= 0 && delay < 0)
		/* underrun - move the application pointer forward to catch up */
		snd_pcm_forward(ai->handle, -delay);
#endif
	frames = snd_pcm_bytes_to_frames(ai->handle, bytes);
	written = snd_pcm_writei(ai->handle, buf, frames);
	if (written >= 0)
		return snd_pcm_frames_to_bytes(ai->handle, written);
	else
		return written;
}

void audio_queueflush(struct audio_info_struct *ai)
{
	/* is this the optimal solution? - we should figure out what we really whant from this function */
	snd_pcm_drop(ai->handle);
	snd_pcm_prepare(ai->handle);
}

int audio_close(struct audio_info_struct *ai)
{
	if(ai->handle != NULL) /* be really generous for being called without any device opening */
	{
		if (snd_pcm_state(ai->handle) == SND_PCM_STATE_RUNNING)
			snd_pcm_drain(ai->handle);
		return snd_pcm_close(ai->handle);
	}
	else return 0;
}

int audio_reset_parameters(struct audio_info_struct *ai)
{
  return 0;
}

int mixer_callback_func(snd_mixer_t *ctl,
                                    unsigned int mask,
                                    snd_mixer_elem_t *elem)
{
    return 0;
}


static int initialize_mixer_device(struct audio_info_struct *ai, char *error)
{
    int              sts;
    snd_mixer_t      *mixer;
    char             *mixerdev = "default";
    const char       *elemnam;
    snd_mixer_elem_t *mixerelem;

    sts = snd_mixer_open(&mixer, 0);
    if (sts) {
        if (error) {
            sprintf(error, "snd_mixer_open failed; %d\n", sts);
        }
        return 1;
    }

    if (mixer) {
        sts = snd_mixer_attach(mixer, mixerdev);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_attach: failed; %d\n", sts);
            }
            return 1;
        }
        sts = snd_mixer_selem_register(mixer, NULL, NULL);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_selem_register: failed; %d\n", sts);
            }
            return 1;
        }
        sts = snd_mixer_load(mixer);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_selem_register: failed; %d\n", sts);
            }
             
            return 1;
        }

        mixerelem = snd_mixer_first_elem(mixer);
        snd_mixer_set_callback(mixer, mixer_callback_func);
        
        while (mixerelem) {
            elemnam = snd_mixer_selem_get_name(mixerelem);
            if (strcasecmp(elemnam, "Master") == 0) {
                ai->vh = mixerelem;
                return 0;
            }
	    mixerelem = snd_mixer_elem_next(mixerelem);
        }
    }
    return 1;
}

/*
 * Set/get current PCM mixer gain. 
 * Return value: 0 success, 1 failure
 * Returns the current mixer setting in ret_value
 * Sets the mixer to new_value is set, does nothing when NULL
 */
static int alsa_gain(struct audio_info_struct *ai, long *ret_value, long *new_value, char *error)
{
    int              sts;
    snd_mixer_elem_t *mixerelem;
    long             value;
    long             left;
    long             right;

    mixerelem = ai->vh;
    if (!mixerelem) {
        if (error) {
            sprintf(error, "Mixer Device is not initialized\n");
        }
        return 1;
    }

    if (ret_value) {
        sts = snd_mixer_selem_get_playback_volume(mixerelem,
                  SND_MIXER_SCHN_FRONT_LEFT, &value);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_selem_get_playback_volume(LEFT): "
                        "failed; %d\n", sts);
            }
            return 1;
        }
        left = value;

        sts = snd_mixer_selem_get_playback_volume(mixerelem,
                  SND_MIXER_SCHN_FRONT_LEFT, &value);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_selem_get_playback_volume(RIGHT): "
                        "failed; %d\n", sts);
            }
            return 1;
        }
        right = value;

        *ret_value = (left + right) / 2;
    }

    if (new_value) {
        sts = snd_mixer_selem_set_playback_volume(mixerelem, 
                  SND_MIXER_SCHN_FRONT_LEFT, *new_value);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_selem_set_playback_volume(LEFT): "
                        "failed; %d\n", sts);
            }
            return 1;
        }
        sts = snd_mixer_selem_set_playback_volume(mixerelem, 
                  SND_MIXER_SCHN_FRONT_RIGHT, *new_value);
        if (sts) {
            if (error) {
                sprintf(error, "snd_mixer_selem_set_playback_volume(RIGHT): "
                        "failed; %d\n", sts);
            }
            return 1;
        }
    }

    return 0;
}


int audio_set_gain(struct audio_info_struct *ai)
{
    int rvol;
    int lvol;
    long volume;
    long old_volume = 0;

    if (!ai) {
        return -1;
    }

    rvol = ai->gain >> 8;
    lvol = (ai->gain & 0xff);
    volume = (rvol*655 + lvol*655) / 2;

    alsa_gain(ai, &old_volume, NULL,  NULL);
    volume = (volume + old_volume) / 2;
    
    alsa_gain(ai, NULL, &volume, NULL);
    return 0;
}


int audio_get_gain(struct audio_info_struct *ai)
{
    long volume;
    int sts;
    if (!ai) {
        return -1;
    }

    sts = alsa_gain(ai, &volume, NULL, NULL);
    if (sts) {
        return -1;
    }
    volume /= 655;
    ai->gain = ((volume & 0xff) << 8) | (volume & 0xff);

    return 0;
}

