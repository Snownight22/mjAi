#ifndef _MJ_ALGORITHM_H_
#define _MJ_ALGORITHM_H_
#include "list.h"

#define MJ_COLOR_COUNT    (3)
#define MJ_NUMBER_COUNT    (9)

#define MJ_SINGLE_COLOR_COUNT    (36)
#define MJ_COMMON_CARD_COUNT    (MJ_SINGLE_COLOR_COUNT * MJ_COLOR_COUNT)
#define MJ_FENG_COUNT    (4*7)

typedef struct hu_info
{
	stListEntry entry;
	int cardkey;
	int needkey_nojiang;
	int needkey;
	long long probability;
	long long prob_jiang;
	char card[MJ_NUMBER_COUNT];
	char needcard[MJ_NUMBER_COUNT];
}stHuInfo;

void mj_init();
long long calc_probability(char cards[], int cards_size, int *needkey);
long long calc_probability_withoutjiang(char cards[], int cards_size, int *needkey);

#endif
