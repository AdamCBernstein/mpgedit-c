/*
 * SccsId[] = "$Id: lame.h,v 1.1 2001/10/03 10:56:11 number6 Exp $"
 */

/*
 *	Interface to MP3 LAME encoding engine
 *
 *	Copyright (c) 1999 Mark Taylor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id: lame.h,v 1.1 2001/10/03 10:56:11 number6 Exp $ */

#ifndef LAME_LAME_H
#define LAME_LAME_H

#include <stdio.h>
#include <stdarg.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(WIN32)
#undef CDECL
#define CDECL _cdecl
#else
#define CDECL
#endif


typedef enum vbr_mode_e {
  vbr_off=0,
  vbr_mt,
  vbr_rh,
  vbr_abr,
  vbr_mtrh,
  vbr_max_indicator,    /* Don't use this! It's used for sanity checks.       */
  vbr_default=vbr_rh  /* change this to change the default VBR mode of LAME */
} vbr_mode;


/* MPEG modes */
typedef enum MPEG_mode_e {
  STEREO = 0,
  JOINT_STEREO,
  DUAL_CHANNEL,   /* LAME doesn't supports this! */
  MONO,
  NOT_SET,
  MAX_INDICATOR   /* Don't use this! It's used for sanity checks. */ 
} MPEG_mode;

/* Padding types */
typedef enum Padding_type_e {
  PAD_NO = 0,
  PAD_ALL,
  PAD_ADJUST,
  PAD_MAX_INDICATOR   /* Don't use this! It's used for sanity checks. */
} Padding_type;



struct lame_global_struct;
typedef struct lame_global_struct lame_global_flags;
typedef lame_global_flags *lame_t;




/***********************************************************************
 *
 *  The LAME API
 *  These functions should be called, in this order, for each
 *  MP3 file to be encoded 
 *
 ***********************************************************************/


/*
 * REQUIRED:
 * initialize the encoder.  sets default for all encoder paramters,
 * returns -1 if some malloc()'s failed
 * otherwise returns 0
 */
lame_global_flags * CDECL lame_init(void);
/* obsolete version */
int CDECL lame_init_old(lame_global_flags *);


/*
 * OPTIONAL:
 * set as needed to override defaults
 */

/********************************************************************
 *  input stream description
 ***********************************************************************/
/* number of samples.  default = 2^32-1   */
int CDECL lame_set_num_samples(lame_global_flags *, unsigned long);
unsigned long CDECL lame_get_num_samples(const lame_global_flags *);

/* input sample rate in Hz.  default = 44100hz */
int CDECL lame_set_in_samplerate(lame_global_flags *, int);
int CDECL lame_get_in_samplerate(const lame_global_flags *);

/* number of channels in input stream. default=2  */
int CDECL lame_set_num_channels(lame_global_flags *, int);
int CDECL lame_get_num_channels(const lame_global_flags *);

/* scale the input by this amount before encoding.  default=0 (disabled) */
/* (not used by decoding routines) */
int CDECL lame_set_scale(lame_global_flags *, float);
float CDECL lame_get_scale(const lame_global_flags *);

/* output sample rate in Hz.  default = 0, which means LAME picks best value  */
/* based on the amount of compression.  MPEG only allows: */
/* MPEG1    32, 44.1,   48khz */
/* MPEG2    16, 22.05,  24 */
/* MPEG2.5   8, 11.025, 12 */
/* (not used by decoding routines) */
int CDECL lame_set_out_samplerate(lame_global_flags *, int);
int CDECL lame_get_out_samplerate(const lame_global_flags *);


/********************************************************************
 *  general control parameters
 ***********************************************************************/
/* 1=cause LAME to collect data for an MP3 frame analzyer. default=0 */
int CDECL lame_set_analysis(lame_global_flags *, int);
int CDECL lame_get_analysis(const lame_global_flags *);

/* 1 = write a Xing VBR header frame. */
/* default = 1 for VBR/ABR modes, 0 for CBR mode */
/* this variable must have been added by a Hungarian notation Windows programmer :-) */
int CDECL lame_set_bWriteVbrTag(lame_global_flags *, int);
int CDECL lame_get_bWriteVbrTag(const lame_global_flags *);

/* 1=decode only.  use lame/mpglib to convert mp3/ogg to wav.  default=0 */
int CDECL lame_set_decode_only(lame_global_flags *, int);
int CDECL lame_get_decode_only(const lame_global_flags *);

/* 1=encode a Vorbis .ogg file.  default=0 */
int CDECL lame_set_ogg(lame_global_flags *, int);
int CDECL lame_get_ogg(const lame_global_flags *);

/* internal algorithm selection.  True quality is determined by the bitrate */
/* but this variable will effect quality by selecting expensive or cheap algorithms. */
/* quality=0..9.  0=best (very slow).  9=worst.   */
/* recommended:  2     near-best quality, not too slow */
/*               5     good quality, fast */
/*               7     ok quality, really fast */
int CDECL lame_set_quality(lame_global_flags *, int);
int CDECL lame_get_quality(const lame_global_flags *);

/* mode = 0,1,2,3 = stereo, jstereo, dual channel (not supported), mono */
/* default: lame picks based on compression ration and input channels */
int CDECL lame_set_mode(lame_global_flags *, MPEG_mode);
MPEG_mode CDECL lame_get_mode(const lame_global_flags *);

/* mode_automs.  Us a M/S mode with a switching threshold based on  */
/* compression ratio */
/* default = 0 (disabled) */
int CDECL lame_set_mode_automs(lame_global_flags *, int);
int CDECL lame_get_mode_automs(const lame_global_flags *);

/* force_ms.  Force M/S for all frames.  For testing only. */
/* default = 0 (disabled) */
int CDECL lame_set_force_ms(lame_global_flags *, int);
int CDECL lame_get_force_ms(const lame_global_flags *);

/* use free_format?  default = 0 (disabled) */
int CDECL lame_set_free_format(lame_global_flags *, int);
int CDECL lame_get_free_format(const lame_global_flags *);

/*
 * OPTIONAL:
 * Set printf like error/debug/message reporting functions.
 * The second argument has to be a pointer to a function which looks like
 *   void my_debugf(const char *format, va_list ap)
 *   {
 *       (void) vfprintf(stdout, format, ap);
 *   }
 * If you use NULL as the value of the pointer in the set function, the
 * lame buildin function will be used (prints to stderr).
 * To quiet any output you have to replace the body of the example function
 * with just "return;" and use it in the set function.
 */
int CDECL lame_set_errorf(lame_global_flags *,
                          void (*func)(const char *, va_list));
int CDECL lame_set_debugf(lame_global_flags *,
                          void (*func)(const char *, va_list));
int CDECL lame_set_msgf  (lame_global_flags *,
                          void (*func)(const char *, va_list));



/* set one of brate compression ratio.  default is compression ratio of 11.  */
int CDECL lame_set_brate(lame_global_flags *, int);
int CDECL lame_get_brate(const lame_global_flags *);
int CDECL lame_set_compression_ratio(lame_global_flags *, float);
float CDECL lame_get_compression_ratio(const lame_global_flags *);

/********************************************************************
 *  frame params
 ***********************************************************************/
/* mark as copyright.  default=0 */
int CDECL lame_set_copyright(lame_global_flags *, int);
int CDECL lame_get_copyright(const lame_global_flags *);

/* mark as original.  default=1 */
int CDECL lame_set_original(lame_global_flags *, int);
int CDECL lame_get_original(const lame_global_flags *);

/* error_protection.  Use 2 bytes from each fraome for CRC checksum. default=0 */
int CDECL lame_set_error_protection(lame_global_flags *, int);
int CDECL lame_get_error_protection(const lame_global_flags *);

/* padding_type.  0=pad no frames  1=pad all frames 2=adjust padding(default) */
int CDECL lame_set_padding_type(lame_global_flags *, Padding_type);
Padding_type CDECL lame_get_padding_type(const lame_global_flags *);

/* MP3 'private extension' bit  Meaningless */
int CDECL lame_set_extension(lame_global_flags *, int);
int CDECL lame_get_extension(const lame_global_flags *);

/* enforce strict ISO complience.  default=0 */
int CDECL lame_set_strict_ISO(lame_global_flags *, int);
int CDECL lame_get_strict_ISO(const lame_global_flags *);
 

/********************************************************************
 * quantization/noise shaping 
 ***********************************************************************/

/* disable the bit reservoir. For testing only. default=0 */
int CDECL lame_set_disable_reservoir(lame_global_flags *, int);
int CDECL lame_get_disable_reservoir(const lame_global_flags *);

/* select a different "best quantization" function. default=0  */
int CDECL lame_set_experimentalX(lame_global_flags *, int);
int CDECL lame_get_experimentalX(const lame_global_flags *);

/* another experimental option.  for testing only */
int CDECL lame_set_experimentalY(lame_global_flags *, int);
int CDECL lame_get_experimentalY(const lame_global_flags *);

/* another experimental option.  for testing only */
int CDECL lame_set_experimentalZ(lame_global_flags *, int);
int CDECL lame_get_experimentalZ(const lame_global_flags *);

/* Naoki's psycho acoustic model.  default=0 */
int CDECL lame_set_exp_nspsytune(lame_global_flags *, int);
int CDECL lame_get_exp_nspsytune(const lame_global_flags *);



/********************************************************************
 * VBR control
 ***********************************************************************/
/* Types of VBR.  default = vbr_off = CBR */
int CDECL lame_set_VBR(lame_global_flags *, vbr_mode);
vbr_mode CDECL lame_get_VBR(const lame_global_flags *);

/* VBR quality level.  0=highest  9=lowest  */
int CDECL lame_set_VBR_q(lame_global_flags *, int);
int CDECL lame_get_VBR_q(const lame_global_flags *);

/* Ignored except for VBR=vbr_abr (ABR mode) */
int CDECL lame_set_VBR_mean_bitrate_kbps(lame_global_flags *, int);
int CDECL lame_get_VBR_mean_bitrate_kbps(const lame_global_flags *);

int CDECL lame_set_VBR_min_bitrate_kbps(lame_global_flags *, int);
int CDECL lame_get_VBR_min_bitrate_kbps(const lame_global_flags *);

int CDECL lame_set_VBR_max_bitrate_kbps(lame_global_flags *, int);
int CDECL lame_get_VBR_max_bitrate_kbps(const lame_global_flags *);

/* 1=stricetly enforce VBR_min_bitrate.  Normally it will be violated for */
/* analog silence */
int CDECL lame_set_VBR_hard_min(lame_global_flags *, int);
int CDECL lame_get_VBR_hard_min(const lame_global_flags *);


/********************************************************************
 * Filtering control
 ***********************************************************************/
/* freq in Hz to apply lowpass. Default = 0 = lame chooses.  -1 = disabled */
int CDECL lame_set_lowpassfreq(lame_global_flags *, int);
int CDECL lame_get_lowpassfreq(const lame_global_flags *);
/* width of transition band, in Hz.  Default = one polyphase filter band */
int CDECL lame_set_lowpasswidth(lame_global_flags *, int);
int CDECL lame_get_lowpasswidth(const lame_global_flags *);

/* freq in Hz to apply highpass. Default = 0 = lame chooses.  -1 = disabled */
int CDECL lame_set_highpassfreq(lame_global_flags *, int);
int CDECL lame_get_highpassfreq(const lame_global_flags *);
/* width of transition band, in Hz.  Default = one polyphase filter band */
int CDECL lame_set_highpasswidth(lame_global_flags *, int);
int CDECL lame_get_highpasswidth(const lame_global_flags *);


/********************************************************************
 * psycho acoustics and other arguments which you should not change 
 * unless you know what you are doing
 ***********************************************************************/
/* only use ATH for masking */
int CDECL lame_set_ATHonly(lame_global_flags *, int);
int CDECL lame_get_ATHonly(const lame_global_flags *);

/* only use ATH for short blocks */
int CDECL lame_set_ATHshort(lame_global_flags *, int);
int CDECL lame_get_ATHshort(const lame_global_flags *);

/* disable ATH */
int CDECL lame_set_noATH(lame_global_flags *, int);
int CDECL lame_get_noATH(const lame_global_flags *);

/* select ATH formula */
int CDECL lame_set_ATHtype(lame_global_flags *, int);
int CDECL lame_get_ATHtype(const lame_global_flags *);

/* lower ATH by this many db */
int CDECL lame_set_ATHlower(lame_global_flags *, float);
float CDECL lame_get_ATHlower(const lame_global_flags *);

/* select adaptive ATH type */
int CDECL lame_set_adjust_type( lame_global_flags *, int);
int CDECL lame_get_adjust_type( const lame_global_flags *);

/* select adaptive ATH level adjustment scheme */
int CDECL lame_set_adapt_thres_type( lame_global_flags *, int);
int CDECL lame_get_adapt_thres_type( const lame_global_flags *);

/* adjust (in dB) the point below which adaptive ATH level adjustment occurs */
int CDECL lame_set_adapt_thres_level( lame_global_flags *, float);
float CDECL lame_get_adapt_thres_level( const lame_global_flags* );

/* predictability limit (ISO tonality formula) */
int CDECL lame_set_cwlimit(lame_global_flags *, int);
int CDECL lame_get_cwlimit(const lame_global_flags *);

/* allow blocktypes to differ between channels? */
/* default: 0 for jstereo, 1 for stereo */
int CDECL lame_set_allow_diff_short(lame_global_flags *, int);
int CDECL lame_get_allow_diff_short(const lame_global_flags *);

/* use temporal masking effect (default = 1) */
int CDECL lame_set_useTemporal(lame_global_flags *, int);
int CDECL lame_get_useTemporal(const lame_global_flags *);

/* disable short blocks */
int CDECL lame_set_no_short_blocks(lame_global_flags *, int);
int CDECL lame_get_no_short_blocks(const lame_global_flags *);

/* force short blocks */
int CDECL lame_set_force_short_blocks(lame_global_flags *, int);
int CDECL lame_get_force_short_blocks(const lame_global_flags *);

/* Input PCM is emphased PCM (for instance from one of the rarely
   emphased CDs), it is STRONGLY not recommended to use this, because
   psycho does not take it into account, and last but not least many decoders
   ignore these bits */
int CDECL lame_set_emphasis(lame_global_flags *, int);
int CDECL lame_get_emphasis(const lame_global_flags *);



/************************************************************************/
/* internal variables, cannot be set...                                 */
/* provided because they may be of use to calling application           */
/************************************************************************/
/* version  0=MPEG-2  1=MPEG-1  (2=MPEG-2.5)     */
int CDECL lame_get_version(const lame_global_flags *);

/* encoder delay */
int CDECL lame_get_encoder_delay(const lame_global_flags *);

/* size of MPEG frame */
int CDECL lame_get_framesize(const lame_global_flags *);

/* number of PCM samples buffered, but not yet encoded to mp3 data. */
int CDECL lame_get_mf_samples_to_encode( const lame_global_flags*  gfp );

/* size (bytes) of mp3 data buffered, but not yet encoded. */
/* this is the number of bytes which would be output by a call to  */
/* lame_encode_flush_nogap.  NOTE: lame_encode_flush() will return */
/* more bytes than this because it will encode the reamining buffered */
/* PCM samples before flushing the mp3 buffers. */
int CDECL lame_get_size_mp3buffer( const lame_global_flags*  gfp );

/* number of frames encoded so far */
int CDECL lame_get_frameNum(const lame_global_flags *);

/* lame's estimate of the total number of frames to be encoded */
/* only valid if calling program set num_samples  */
int CDECL lame_get_totalframes(const lame_global_flags *);








/*
 * REQUIRED:
 * sets more internal configuration based on data provided above.
 * returns -1 if something failed.
 */
int CDECL lame_init_params(lame_global_flags *);


/*
 * OPTIONAL:
 * get the version number, in a string. of the form:  
 * "3.63 (beta)" or just "3.63". 
 */
const char*  CDECL get_lame_version       ( void );
const char*  CDECL get_lame_short_version ( void );
const char*  CDECL get_psy_version        ( void );
const char*  CDECL get_mp3x_version       ( void );
const char*  CDECL get_lame_url           ( void );

/*
 * OPTIONAL:
 * get the version numbers in numerical form.
 */
typedef struct {
    /* generic LAME version */
    int major;
    int minor;
    int alpha;               /* 0 if not an alpha version                  */
    int beta;                /* 0 if not a beta version                    */

    /* version of the psy model */
    int psy_major;
    int psy_minor;
    int psy_alpha;           /* 0 if not an alpha version                  */
    int psy_beta;            /* 0 if not a beta version                    */

    /* compile time features */
    const char *features;    /* Don't make assumptions about the contents! */
} lame_version_t;
void CDECL get_lame_version_numerical ( lame_version_t *const );


/*
 * OPTIONAL:
 * print internal lame configuration to message handler
 */
void CDECL lame_print_config(const lame_global_flags*  gfp);

void CDECL lame_print_internals( const lame_global_flags *gfp);


/*
 * input pcm data, output (maybe) mp3 frames.
 * This routine handles all buffering, resampling and filtering for you.
 * 
 * return code     number of bytes output in mp3buf. Can be 0 
 *                 -1:  mp3buf was too small
 *                 -2:  malloc() problem
 *                 -3:  lame_init_params() not called
 *                 -4:  psycho acoustic problems 
 *                 -5:  ogg cleanup encoding error
 *                 -6:  ogg frame encoding error
 *
 * The required mp3buf_size can be computed from num_samples, 
 * samplerate and encoding rate, but here is a worst case estimate:
 *
 * mp3buf_size in bytes = 1.25*num_samples + 7200
 *
 * I think a tighter bound could be:  (mt, March 2000)
 * MPEG1:
 *    num_samples*(bitrate/8)/samplerate + 4*1152*(bitrate/8)/samplerate + 512
 * MPEG2:
 *    num_samples*(bitrate/8)/samplerate + 4*576*(bitrate/8)/samplerate + 256
 *
 * but test first if you use that!
 *
 * set mp3buf_size = 0 and LAME will not check if mp3buf_size is
 * large enough.
 *
 * NOTE:
 * if gfp->num_channels=2, but gfp->mode = 3 (mono), the L & R channels
 * will be averaged into the L channel before encoding only the L channel
 * This will overwrite the data in buffer_l[] and buffer_r[].
 * 
*/
int CDECL lame_encode_buffer (
        lame_global_flags*  gfp,           /* global context handle         */
        const short int     buffer_l [],   /* PCM data for left channel     */
        const short int     buffer_r [],   /* PCM data for right channel    */
        const int           nsamples,      /* number of samples per channel */
        unsigned char*      mp3buf,        /* pointer to encoded MP3 stream */
        const int           mp3buf_size ); /* number of valid octets in this
                                              stream                        */

/*
 * as above, but input has L & R channel data interleaved.
 * NOTE: 
 * num_samples = number of samples in the L (or R)
 * channel, not the total number of samples in pcm[]  
 */
int CDECL lame_encode_buffer_interleaved(
        lame_global_flags*  gfp,           /* global context handlei        */
        short int           pcm[],         /* PCM data for left and right
                                              channel, interleaved          */
        int                 num_samples,   /* number of samples per channel,
                                              _not_ number of samples in
                                              pcm[]                         */
        unsigned char*      mp3buf,        /* pointer to encoded MP3 stream */
        int                 mp3buf_size ); /* number of valid octets in this
                                              stream                        */


/* as lame_encode_buffer, but for 'float's */
int CDECL lame_encode_buffer_float(
        lame_global_flags*  gfp,           /* global context handle         */
        const float     buffer_l [],       /* PCM data for left channel     */
        const float     buffer_r [],       /* PCM data for right channel    */
        const int           nsamples,      /* number of samples per channel */
        unsigned char*      mp3buf,        /* pointer to encoded MP3 stream */
        const int           mp3buf_size ); /* number of valid octets in this
                                              stream                        */

/* as lame_encode_buffer, but for int's */
int CDECL lame_encode_buffer_int(
        lame_global_flags*  gfp,           /* global context handle         */
        const int      buffer_l [],       /* PCM data for left channel     */
        const int      buffer_r [],       /* PCM data for right channel    */
        const int           nsamples,      /* number of samples per channel */
        unsigned char*      mp3buf,        /* pointer to encoded MP3 stream */
        const int           mp3buf_size ); /* number of valid octets in this
                                              stream                        */

/* as lame_encode_buffer, but for long's */
int CDECL lame_encode_buffer_long(
        lame_global_flags*  gfp,           /* global context handle         */
        const long     buffer_l [],       /* PCM data for left channel     */
        const long     buffer_r [],       /* PCM data for right channel    */
        const int           nsamples,      /* number of samples per channel */
        unsigned char*      mp3buf,        /* pointer to encoded MP3 stream */
        const int           mp3buf_size ); /* number of valid octets in this
                                              stream                        */






/*
 * REQUIRED:
 * lame_encode_flush will flush the intenal PCM buffers, padding with 
 * 0's to make sure the final frame is complete, and then flush
 * the internal MP3 buffers, and thus may return a 
 * final few mp3 frames.  'mp3buf' should be at least 7200 bytes long
 * to hold all possible emitted data.
 *
 * will also write id3v1 tags (if any) into the bitstream       
 *
 * return code = number of bytes output to mp3buf. Can be 0
 */
int CDECL lame_encode_flush(
        lame_global_flags *  gfp,    /* global context handle                 */
        unsigned char*       mp3buf, /* pointer to encoded MP3 stream         */
        int                  size);  /* number of valid octets in this stream */

/*
 * OPTIONAL:
 * lame_encode_flush_nogap will flush the internal mp3 buffers and pad
 * the last frame with ancillary data so it is a complete mp3 frame.
 * 
 * 'mp3buf' should be at least 7200 bytes long
 * to hold all possible emitted data.
 *
 * After a call to this routine, the outputed mp3 data is complete, but
 * you may continue to encode new PCM samples and write future mp3 data
 * to a different file.  The two mp3 files will play back with no gaps
 * if they are concatenated together.
 *
 * This routine will NOT write id3v1 tags into the bitstream.
 *
 * return code = number of bytes output to mp3buf. Can be 0
 */
int CDECL lame_encode_flush_nogap(
        lame_global_flags *  gfp,    /* global context handle                 */
        unsigned char*       mp3buf, /* pointer to encoded MP3 stream         */
        int                  size);  /* number of valid octets in this stream */

/*
 * OPTIONAL:
 * Normally, this is called by lame_init_params().  It writes id3v2 and
 * Xing headers into the front of the bitstream, and sets frame counters
 * and bitrate histogram data to 0.  You can also call this after 
 * lame_encode_flush_nogap().  
 */
int CDECL lame_init_bitstream(
        lame_global_flags *  gfp);    /* global context handle                 */



/*
 * OPTIONAL:    some simple statistics
 * a bitrate histogram to visualize the distribution of used frame sizes
 * a stereo mode histogram to visualize the distribution of used stereo
 *   modes, useful in joint-stereo mode only
 *   0: LR    left-right encoded
 *   1: LR-I  left-right and intensity encoded (currently not supported)
 *   2: MS    mid-side encoded
 *   3: MS-I  mid-side and intensity encoded (currently not supported)
 *
 * attention: don't call them after lame_encode_finish
 * suggested: lame_encode_flush -> lame_*_hist -> lame_close
 */

void CDECL lame_bitrate_hist( 
        const lame_global_flags *const gfp, 
              int                      bitrate_count[14] );
void CDECL lame_bitrate_kbps( 
        const lame_global_flags *const gfp, 
              int                      bitrate_kbps [14] );
void CDECL lame_stereo_mode_hist( 
        const lame_global_flags *const gfp, 
              int                      stereo_mode_count[4] );

void CDECL lame_bitrate_stereo_mode_hist ( 
        const lame_global_flags*  gfp, 
        int  bitrate_stmode_count [14] [4] );


/*
 * OPTIONAL:
 * lame_mp3_tags_fid will append a Xing VBR tag to the mp3 file with file
 * pointer fid.  These calls perform forward and backwards seeks, so make
 * sure fid is a real file.  Make sure lame_encode_flush has been called,
 * and all mp3 data has been written to the file before calling this
 * function.
 * NOTE:
 * if VBR  tags are turned off by the user, or turned off by LAME because
 * the output is not a regular file, this call does nothing
*/
void CDECL lame_mp3_tags_fid(lame_global_flags *,FILE* fid);


/*
 * REQUIRED:
 * final call to free all remaining buffers
 */
int  CDECL lame_close (lame_global_flags *);

/*
 * OBSOLETE:
 * lame_encode_finish combines lame_encode_flush() and lame_close() in
 * one call.  However, once this call is made, the statistics routines
 * will no longer work because the data will have been cleared
 */
int CDECL lame_encode_finish(
        lame_global_flags*  gfp,
        unsigned char*      mp3buf,
        int                 size );







/*********************************************************************
 *
 * decoding 
 *
 * a simple interface to mpglib, part of mpg123, is also included if
 * libmp3lame is compiled with HAVE_MPGLIB
 *
 *********************************************************************/
typedef struct {
  int header_parsed;   /* 1 if header was parsed and following data was
                          computed                                       */
  int stereo;          /* number of channels                             */
  int samplerate;      /* sample rate                                    */
  int bitrate;         /* bitrate                                        */
  int mode;            /* mp3 frame type                                 */
  int mode_ext;        /* mp3 frame type                                 */
  int framesize;       /* number of samples per mp3 frame                */

  /* this data is only computed if mpglib detects a Xing VBR header */
  unsigned long nsamp; /* number of samples in mp3 file.                 */
  int totalframes;     /* total number of frames in mp3 file             */

  /* this data is not currently computed by the mpglib routines */
  int framenum;        /* frames decoded counter                         */
} mp3data_struct;


/* required call to initialize decoder */
int CDECL lame_decode_init(void);

/*********************************************************************
 * input 1 mp3 frame, output (maybe) pcm data.  
 * lame_decode() return code:
 *   -1: error
 *    0: need more data
 *  n>0: size of pcm output
 *********************************************************************/
int CDECL lame_decode(
        unsigned char *  mp3buf,
        int              len,
        short            pcm_l[],
        short            pcm_r[] );

/* same as lame_decode, and also returns mp3 header data */
int CDECL lame_decode_headers(
        unsigned char*   mp3buf,
        int              len,
        short            pcm_l[],
        short            pcm_r[],
        mp3data_struct*  mp3data );

/* same as lame_decode, but returns at most one frame */
int CDECL lame_decode1(
        unsigned char*  mp3buf,
        int             len,
        short           pcm_l[],
        short           pcm_r[] );

/* same as lame_decode1, but returns at most one frame and mp3 header data */
int CDECL lame_decode1_headers(
        unsigned char*   mp3buf,
        int              len,
        short            pcm_l[],
        short            pcm_r[],
        mp3data_struct*  mp3data );





/*********************************************************************
 *
 * id3tag stuff
 *
 *********************************************************************/

/*
 * id3tag.h -- Interface to write ID3 version 1 and 2 tags.
 *
 * Copyright (C) 2000 Don Melton.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

/* utility to obtain alphabetically sorted list of genre names with numbers */
extern void id3tag_genre_list(
        void (*handler)(int, const char *, void *),
        void*  cookie);

extern void id3tag_init   (lame_global_flags *gfp);

/* force addition of version 2 tag */
extern void id3tag_add_v2   (lame_global_flags *gfp);

/* add only a version 1 tag */
extern void id3tag_v1_only  (lame_global_flags *gfp);

/* add only a version 2 tag */
extern void id3tag_v2_only  (lame_global_flags *gfp);

/* pad version 1 tag with spaces instead of nulls */
extern void id3tag_space_v1 (lame_global_flags *gfp);

/* pad version 2 tag with extra 128 bytes */
extern void id3tag_pad_v2   (lame_global_flags *gfp);

extern void id3tag_set_title(
        lame_global_flags*  gfp,
        const char*         title );
extern void id3tag_set_artist(
        lame_global_flags*  gfp,
        const char*         artist );
extern void id3tag_set_album(
        lame_global_flags*  gfp,
        const char*         album );
extern void id3tag_set_year(
        lame_global_flags*  gfp,
        const char*         year );
extern void id3tag_set_comment(
        lame_global_flags*  gfp,
        const char*         comment );
extern void id3tag_set_track(
        lame_global_flags*  gfp,
        const char*         track );

/* return non-zero result if genre name or number is invalid */
extern int id3tag_set_genre(
        lame_global_flags*  gfp,
        const char*         genre );

/***********************************************************************
*
*  list of valid bitrates [kbps] & sample frequencies [Hz].
*  first index: 0: MPEG-2   values  (sample frequencies 16...24 kHz) 
*               1: MPEG-1   values  (sample frequencies 32...48 kHz)
*               2: MPEG-2.5 values  (sample frequencies  8...12 kHz)
***********************************************************************/
extern const int      bitrate_table    [3] [16];
extern const int      samplerate_table [3] [ 4];



/* maximum size of mp3buffer needed if you encode at most 1152 samples for
   each call to lame_encode_buffer.  see lame_encode_buffer() below  
   (LAME_MAXMP3BUFFER is now obsolete)  */
#define LAME_MAXMP3BUFFER   16384


#if defined(__cplusplus)
}
#endif
#endif /* LAME_LAME_H */
