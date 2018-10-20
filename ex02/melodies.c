#include "melodies.h"

melody_t mel_startup = {
	.tones = (tone_t[]){{200, 100}, {300, 100}, {400, 100}},
	.n = 3,
	.x = 0
};

melody_t mel_1up = {
	.tones = (tone_t[]){{659,150}, {784,150}, {1319,150}, {1047, 150}, {1175,150}, {1568,200}},
	.n = 6,
	.x = 0
};

#define dur 35
melody_t2 mel_mushroom = {
	.tones = (tone_t2[]){
		{ C5,dur},
		{ G4,dur},
		{ C5,dur},
		{ E5,dur},
		{ G5,dur},
		{ C6,dur},
		{ G5,dur},
		{Ab4,dur},
		{ C5,dur},
		{Eb5,dur},
		{Ab5,dur},
		{Eb5,dur},
		{Ab5,dur},
		{ C6,dur},
		{Eb6,dur},
		{Ab6,dur},
		{Eb6,dur},
		{ D5,dur},
		{ F5,dur},
		{Bb5,dur},
		{ F5,dur},
		{Bb5,dur},
		{ D6,dur},
		{ F6,dur},
		{ D6,dur},
		{ F6,dur},
		{Bb6,dur}},
	.n = 27,
	.x = 0
};
