#include "melodies.h"

melody_t mel_startup = {
	.tones = (tone_t[]){
		{G3,100},
		{G4,100},
		{G5,100}},
	.n = 3,
	.x = 0
};

melody_t mel_pitchfork = {
	.tones = (tone_t[]){{A4,1000}},
	.n = 1,
	.x = 0
};

melody_t mel_1up = {
	.tones = (tone_t[]){
		{E5,150},
		{G5,150},
		{E6,150},
		{C6,150},
		{D6,150},
		{G6,200}},
	.n = 6,
	.x = 0
};

melody_t mel_powerup = {
	.tones = (tone_t[]){
		{ C5,35},
		{ G4,35},
		{ C5,35},
		{ E5,35},
		{ G5,35},
		{ C6,35},
		{ G5,35},
		{Ab4,35},
		{ C5,35},
		{Eb5,35},
		{Ab5,35},
		{Eb5,35},
		{Ab5,35},
		{ C6,35},
		{Eb6,35},
		{Ab6,35},
		{Eb6,35},
		{ D5,35},
		{ F5,35},
		{Bb5,35},
		{ F5,35},
		{Bb5,35},
		{ D6,35},
		{ F6,35},
		{ D6,35},
		{ F6,35},
		{Bb6,35}},
	.n = 27,
	.x = 0
};

melody_t mel_powerdown = {
	.tones = (tone_t[]){
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
