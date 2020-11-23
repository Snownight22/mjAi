#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mj_algorithm.h"

int g_jiang_centain_card = 0;    //是否258作将0-不是,1-是
int g_desk_cards_count = (MJ_FENG_COUNT + MJ_COMMON_CARD_COUNT) - 13 * 4 - 1;
//int g_desk_cards_count = (MJ_FENG_COUNT + MJ_COMMON_CARD_COUNT);

int check_ke(char cards[], int index, int cards_size)
{
	if (index > cards_size)
		return 0;
	if (cards[index] >= 3)
	{
		cards[index] -= 3;
		return 1;
	}

	return 0;
}

int check_shun(char cards[], int index)
{
	if (index > 6)
		return 0;
	if (cards[index] >= 1 && cards[index+1] >= 1 && cards[index+2] >= 1)
	{
		cards[index] -= 1;
		cards[index+1] -= 1;
		cards[index+2] -= 1;
		return 1;
	}

	return 0;
}

int check_jiang(char cards[], int index)
{
	if (g_jiang_centain_card == 1 && index != 1 && index != 4 && index != 7)
		return 0;
	if (cards[index] >= 2)
	{
		cards[index] -= 2;
		return 1;
	}
	return 0;
}

int judge_hu_with_jiang(char cards[], int cards_size, int hasjiang)
{
	char tmp_cards[MJ_NUMBER_COUNT] = { 0 };

	int hu = 1;
	memcpy(tmp_cards, cards, cards_size);
	for (int i = 0; i < cards_size; i++)
	{
		if (tmp_cards[i] < 1)
			continue;
		hu = 0;
		if (hasjiang == 0 && tmp_cards[i] >= 2)
		{
			for (int usedforjiang = 0; usedforjiang <= 1; usedforjiang++)
			{
				hasjiang = usedforjiang;
				if (hasjiang == 1)
				{
					if (check_jiang(tmp_cards, i) != 1)
					{
						hasjiang = 0;
						break;
					}
					else
						return judge_hu_with_jiang(tmp_cards, cards_size, hasjiang);
				}

				if (check_ke(tmp_cards, i, cards_size) == 1)
				{
					return judge_hu_with_jiang(tmp_cards, cards_size, hasjiang);
				}
				else if (check_shun(tmp_cards, i) == 1)
				{
					return judge_hu_with_jiang(tmp_cards, cards_size, hasjiang);
				}
			}
		}
		else
		{
			if (check_ke(tmp_cards, i, cards_size) == 1)
			{
				return judge_hu_with_jiang(tmp_cards, cards_size, hasjiang);
			}
			else if (check_shun(tmp_cards, i) == 1)
			{
				return judge_hu_with_jiang(tmp_cards, cards_size, hasjiang);
			}
		}
	}

	return (hu && hasjiang);
}

int judge_hu_without_jiang(char cards[], int cards_size)
{
	char tmp_cards[10] = {0};

	int hu = 1;
	memcpy(tmp_cards, cards, cards_size);
	for (int i = 0; i < cards_size; i++)
	{
		if (tmp_cards[i] < 1)
			continue;
		hu = 0;
		if (check_ke(tmp_cards, i, cards_size) == 1)
		{
			return judge_hu_without_jiang(tmp_cards, cards_size);
		}
		else if (check_shun(tmp_cards, i) == 1)
		{
			return judge_hu_without_jiang(tmp_cards, cards_size);
		}
	}

	return hu;
}

int check_hu(char cards[], int cards_size, int hasjiang, int *needcard, int iterationcount, stHuInfo *huInfo)
{
	char tmp_cards[10] = {0};
    int minneedcard = 10;
	int mincard = 10;
	long long prob = huInfo->probability;
	long long minprob = 1;
	char tmp_needcard[MJ_NUMBER_COUNT] = {0};
	char min_needcard[MJ_NUMBER_COUNT] = {0};
	int pp[4] = {3,4,6,12};

	if (++iterationcount > 6)
		return 0;
	(*needcard)++;
	memcpy(tmp_cards, cards, cards_size);
	memcpy(tmp_needcard, huInfo->needcard, sizeof(tmp_needcard));
	for (int i = 0; i < 9; i++)
	{
		huInfo->probability = prob;
		memcpy(huInfo->needcard, tmp_needcard, sizeof(tmp_needcard));
		if (tmp_cards[i] > 3)
			continue;
		tmp_cards[i] += 1;
		huInfo->needcard[i] += 1;
		huInfo->probability *= 3*9*pp[tmp_cards[i] - 1];
		if (hasjiang == 1)
		{
			if (judge_hu_with_jiang(tmp_cards, cards_size, 0) == 1)
				return 1;
		}
		else
		{
		    if (judge_hu_without_jiang(tmp_cards, cards_size) == 1)
			    return 1;
		}

		int lneedcard = 0;
		if (check_hu(tmp_cards, cards_size, hasjiang, &lneedcard, iterationcount, huInfo) == 1)
		{
			if (minprob == 1)
			{
				minprob = huInfo->probability;
				memcpy(min_needcard, huInfo->needcard, sizeof(min_needcard));
			}
			else if (minprob > huInfo->probability)
			{
				minprob = huInfo->probability;
				memcpy(min_needcard, huInfo->needcard, sizeof(min_needcard));
			}
		}

		tmp_cards[i] -= 1;
		huInfo->needcard[i] -= 1;
	}

	if (minprob != 1)
	{
		huInfo->probability = minprob;
		memcpy(huInfo->needcard, min_needcard, sizeof(min_needcard));
		return 1;
	}

	return 0;
}

int getkey(char cards[])
{
	int key = 0;
	for (int i = 0; i < MJ_NUMBER_COUNT; i++)
		key = key * 10 + cards[i];

	return key;
}

long long calc_probability(char cards[], int cards_size, int *needkey)
{
	int needcard = 0;
	long long prob = 1;
	long long total = 9 * 3;
	long long allprob = 0;
	int iterationcount = 0;
	stHuInfo huInfo;
	memset(huInfo.needcard, 0, sizeof(huInfo.needcard));
	huInfo.probability = 1;
	*needkey = 0;

	if (judge_hu_without_jiang(cards, cards_size) == 1)
		return 1;

	if (check_hu(cards, cards_size, 0, &needcard, iterationcount, &huInfo) == 1)
	{
		allprob = huInfo.probability;
		*needkey = getkey(huInfo.needcard);
	}
	else
		allprob = 0;

	return allprob;
}

long long calc_probability_withoutjiang(char cards[], int cards_size, int *needkey)
{
	int needcard = 0;
	long long total = 9 * 3;
	long long allprob = 0;
	int iterationcount = 0;
	stHuInfo huInfo;
	huInfo.probability = 1;
	memset(huInfo.needcard, 0, sizeof(huInfo.needcard));
	*needkey = 0;

	if (judge_hu_with_jiang(cards, cards_size, 0) == 1)
		return 1;

	if (check_hu(cards, cards_size, 1, &needcard, iterationcount, &huInfo) == 1)
	{
		allprob = huInfo.probability;
		*needkey = getkey(huInfo.needcard);
	}
	else
		allprob = 0;

	return allprob;
}

