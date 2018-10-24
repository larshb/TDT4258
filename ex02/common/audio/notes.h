#ifndef NOTES_H__
#define NOTES_H__

#include <stdint.h>

/* Look-up table for notes and their nearest integer frequency */

typedef enum {
	C0,Db0,D0,Eb0,E0,F0,Gb0,G0,Ab0,A0,Bb0,B0,
	C1,Db1,D1,Eb1,E1,F1,Gb1,G1,Ab1,A1,Bb1,B1,
	C2,Db2,D2,Eb2,E2,F2,Gb2,G2,Ab2,A2,Bb2,B2,
	C3,Db3,D3,Eb3,E3,F3,Gb3,G3,Ab3,A3,Bb3,B3,
	C4,Db4,D4,Eb4,E4,F4,Gb4,G4,Ab4,A4,Bb4,B4,
	C5,Db5,D5,Eb5,E5,F5,Gb5,G5,Ab5,A5,Bb5,B5,
	C6,Db6,D6,Eb6,E6,F6,Gb6,G6,Ab6,A6,Bb6,B6,
	C7,Db7,D7,Eb7,E7,F7,Gb7,G7,Ab7,A7,Bb7,B7,
	C8,Db8,D8,Eb8,E8,F8,Gb8,G8,Ab8,A8,Bb8,B8}
	note_t;

uint16_t note_frequencies[108];

uint16_t note2freq(note_t note);

#endif /* end of include guard: NOTES_H__ */
