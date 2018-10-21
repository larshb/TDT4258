#ifndef MELODIES_H__
#define MELODIES_H__

#include "notes.h"

#include <stdint.h>

typedef struct {
	note_t note;	/* Hz */
	uint16_t duration;	/* ms */
} tone_t;

typedef struct {
	tone_t* tones;
	uint16_t n; /* Number of notes */
	uint16_t x; /* Current note */
} melody_t;

melody_t mel_startup;
melody_t mel_pitchfork;
melody_t mel_1up;
melody_t mel_powerup;
melody_t mel_powerdown;

#endif /* end of include guard: MELODIES_H__ */