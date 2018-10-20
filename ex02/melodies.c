#include "melodies.h"

note_t mel_startup_notes[] = {{200, 100}, {300, 100}, {400, 100}};
melody_t mel_startup = {
	.notes = &mel_startup_notes,
	.n = 3,
	.x = 0
};

note_t mel_1up_notes[] = {{659,150}, {784,150}, {1319,150}, {1047, 150}, {1175,150}, {1568,200}};
melody_t mel_1up = {
	.notes = &mel_1up_notes,
	.n = 6,
	.x = 0
};

// C3  G2  C3  E3  G3  C4  G3  G#2 C3  D#3 G#3 D#3 G#3 C4  D#4 G#4 D#4 D3  F3  A#3 F3  A#3  D4	E4	D4	E4	A#4 E4
// 131, 98,131,165,196,262,196,104,131,156,208,156,208,262,311,415,311,147,175,233,175,233,294,330,294,330,466,330

#define dur 200
#define oct 3
note_t mel_mushroom_notes[] = {{131,dur},{oct* 98,dur},{oct* 131,dur},{oct* 165,dur},{oct* 196,dur},{oct* 262,dur},{oct* 196,dur},{oct*104,dur},{oct*131,dur},{oct*156,dur},{oct*208,dur},{oct*156,dur},{oct*208,dur},{oct*262,dur},{oct*311,dur},{oct*415,dur},{oct*311,dur},{oct*147,dur},{oct*175,dur},{oct*233,dur},{oct*175,dur},{oct*233,dur},{oct*294,dur},{oct*330,dur},{oct*294,dur},{oct*330,dur},{oct*466,dur},{oct*330,dur}};
melody_t mel_mushroom = {
	.notes = &mel_mushroom_notes,
	.n = 28,
	.x = 0
};