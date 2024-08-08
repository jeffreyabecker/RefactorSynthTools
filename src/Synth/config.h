#define WAVEFORM_BIT_LENGTH 1024
#define USE_STATIC_WAVEFORM_SINE 1
#define USE_STATIC_WAVEFORM_SAW_TOOTH 1
#define USE_STATIC_WAVEFORM_SQUARE 1
#define USE_STATIC_WAVEFORM_PULSE 1
#define USE_STATIC_WAVEFORM_TRIANGLE 1
#define USE_STATIC_WAVEFORM_NOISE 1


#define MAX_POLY_VOICE  8  /* max single voices, can use multiple osc */
#define MAX_POLY_VOICES_PER_OSC 3
#define MAX_POLY_OSC    (MAX_POLY_VOICES_PER_OSC*MAX_POLY_VOICE) /* osc polyphony, always active reduces single voices max poly */


#define SAMPLE_RATE 44100
#define SAMPLE_BUFFER_SIZE 48