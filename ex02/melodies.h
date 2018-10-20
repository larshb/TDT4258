#ifndef MELODIES_H__
#define MELODIES_H__

#include <stdint.h>

typedef struct {
	uint16_t frequency;	/* Hz */
	uint16_t duration;	/* ms */
} note_t;

typedef struct {
	note_t* notes;
	uint16_t n; /* Number of notes */
	uint16_t x; /* Current note */
} melody_t;

melody_t mel_startup;
melody_t mel_1up;
melody_t mel_mushroom;

#endif /* end of include guard: MELODIES_H__ */