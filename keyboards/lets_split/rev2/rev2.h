#ifndef REV2_H
#define REV2_H

#include "../lets_split.h"

//void promicro_bootloader_jmp(bool program);
#include "quantum.h"

//void promicro_bootloader_jmp(bool program);

#define KEYMAP( \
	k00, k01, k02, k03, k04, k05, k06, k07, k57, k56, k55, k54, k53, k52, k51, k50, \
	k10, k11, k12, k13, k14, k15, k16, k17, k67, k66, k65, k64, k63, k62, k61, k60, \
	k20, k21, k22, k23, k24, k25, k26, k27, k77, k76, k75, k74, k73, k72, k71, k70, \
	k30, k31, k32, k33, k34, k35, k36, k37, k87, k86, k85, k84, k83, k82, k81, k80, \
	k40, k41, k42, k43, k44, k45, k46, k47, k97, k96, k95, k94, k93, k92, k91, k90  ) \
	{ \
		{ k00, k10, k20, k30, k40 }, \
		{ k01, k11, k21, k31, k41 }, \
		{ k02, k12, k22, k32, k42 }, \
		{ k03, k13, k23, k33, k43 }, \
		{ k04, k14, k24, k34, k44 }, \
		{ k05, k15, k25, k35, k45 }, \
		{ k06, k16, k26, k36, k46 }, \
		{ k07, k17, k27, k37, k47 }, \
		{ k50, k60, k70, k80, k90 }, \
		{ k51, k61, k71, k81, k91 }, \
		{ k52, k62, k72, k82, k92 }, \
		{ k53, k63, k73, k83, k93 }, \
		{ k54, k64, k74, k84, k94 }, \
		{ k55, k65, k75, k85, k95 }, \
		{ k56, k66, k76, k86, k96 }, \
		{ k57, k67, k77, k87, k97 } \
	}
#endif
