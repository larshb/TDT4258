#ifndef MELODIES_H__
#define MELODIES_H__

#include "notes.h"

#include <stdint.h>

typedef struct {
	uint16_t frequency;	/* Hz */
	uint16_t duration;	/* ms */
} tone_t;

typedef struct {
	tone_t* tones;
	uint16_t n; /* Number of notes */
	uint16_t x; /* Current note */
} melody_t;

typedef struct {
	note_t note;	/* Hz */
	uint16_t duration;	/* ms */
} tone_t2;

typedef struct {
	tone_t2* tones;
	uint16_t n; /* Number of notes */
	uint16_t x; /* Current note */
} melody_t2;

melody_t mel_startup;
melody_t mel_1up;
melody_t2 mel_mushroom;

#endif /* end of include guard: MELODIES_H__ */