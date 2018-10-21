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

melody_t2 mel_startup2 = {
	.tones = (tone_t2[]){{G3,100},{G4,100},{G5,100}},
	.n = 3,
	.x = 0
};

melody_t2 mel_1up2 = {
	.tones = (tone_t2[]){
		{E5,150},
		{G5,150},
		{E6,150},
		{C6,150},
		{D6,150},
		{G6,200}},
	.n = 6,
	.x = 0
};

#define dur 35
melody_t2 mel_powerup = {
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

melody_t2 mel_powerdown = {
	.tones = (tone_t2[]){
		{A4,30},
		{G4,30},
		{E4,30},
		{C4,60},
		{A4,30},
		{G4,30},
		{E4,30},
		{C4,60},
		{A4,30},
		{G4,30},
		{E4,30},
		{C4,60}
	},
	.n = 12,
	.x = 0
};
