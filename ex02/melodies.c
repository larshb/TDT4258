#include "melodies.h"

tone_t mel_startup_tones[] = {{200, 100}, {300, 100}, {400, 100}};
melody_t mel_startup = {
	.tones = mel_startup_tones,
	.n = 3,
	.x = 0
};

tone_t mel_1up_tones[] = {{659,150}, {784,150}, {1319,150}, {1047, 150}, {1175,150}, {1568,200}};
melody_t mel_1up = {
	.tones = mel_1up_tones,
	.n = 6,
	.x = 0
};

// C3  G2  C3  E3  G3  C4  G3  Ab2 C3  Eb3 Ab3 Eb3 Ab3 C4  Eb4 Ab4 Eb4 D3  F3  Bb3 F3  Bb3  D4  F4  D4  F4  Bb4 F4
// C3  G2  C3  E3  G3  C4  G3  G#2 C3  D#3 G#3 D#3 G#3 C4  D#4 G#4 D#4 D3  F3  A#3 F3  A#3  D4	E4	D4	E4	A#4 E4
// 131, 98,131,165,196,262,196,104,131,156,208,156,208,262,311,415,311,147,175,233,175,233,294,330,294,330,466,330
#define dur 35
#define oct 3
tone_t mel_mushroom_tones[] = {{131,dur},{oct* 98,dur},{oct* 131,dur},{oct* 165,dur},{oct* 196,dur},{oct* 262,dur},{oct* 196,dur},{oct*104,dur},{oct*131,dur},{oct*156,dur},{oct*208,dur},{oct*156,dur},{oct*208,dur},{oct*262,dur},{oct*311,dur},{oct*415,dur},{oct*311,dur},{oct*147,dur},{oct*175,dur},{oct*233,dur},{oct*175,dur},{oct*233,dur},{oct*294,dur},{oct*330,dur},{oct*294,dur},{oct*330,dur},{oct*466,dur},{oct*330,dur}};
melody_t mel_mushroom = {
	.tones = mel_mushroom_tones,
	.n = 28,
	.x = 0
};

tone_t2 mel_mushroom2_tones[] = {
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
	{Bb6,dur},
};

melody_t2 mel_mushroom2 = {
	.tones = mel_mushroom2_tones,
	.n = 27,
	.x = 0
};

#define spd 15
tone_t2 mel_mushroom2b_tones[] = {
	{ G3,3*spd},
	{ B4,1*spd},
	{ D5,1*spd},
	{ G5,3*spd},
	{ B5,4*spd},
	{Ab4,3*spd},
	{ C5,1*spd},
	{Eb5,1*spd},
	{Ab5,3*spd},
	{ C6,4*spd},
	{Bb4,3*spd},
	{ D5,1*spd},
	{ F5,1*spd},
	{Bb5,3*spd},
	{ D6,4*spd}
};

melody_t2 mel_mushroom2b = {
	.tones = mel_mushroom2b_tones,
	.n = 15,
	.x = 0
};
