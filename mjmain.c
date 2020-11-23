#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mj_algorithm.h"

char handCard[27] = 
{
	1, 1, 0, 0, 2, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0
};

stListHead g_mj_head;
int g_mj_node_total = 0;

void mj_init()
{
	g_mj_node_total = 0;
	LIST_INIT(&g_mj_head);
}

void add_list(int key, char card[], long long prob)
{
	stHuInfo *huinfo = malloc(sizeof(stHuInfo));
	if (NULL == huinfo)
	{
		printf("malloc error!\n");
		return ;
	}

	huinfo->cardkey = key;
	huinfo->probability = prob;
	for (int i = 0; i < MJ_NUMBER_COUNT; i++)
	{
		huinfo->card[i] = card[i];
	}

	LIST_INSERT_TAIL(&g_mj_head, &huinfo->entry);
	g_mj_node_total++;
}

void gen_card(char card[], int index, int total)
{
	if (total == 0)
	{
		int key = 0;
		for (int i = 0; i < MJ_NUMBER_COUNT; i++)
			key = key * 10 + card[i];
		add_list(key, card, 0);
		return ;
	}

	if (index == MJ_NUMBER_COUNT - 1)
	{
		if (total > 4)
			return ;
		card[index] = total;

		int key = 0;
		for (int i = 0; i < MJ_NUMBER_COUNT; i++)
			key = key * 10 + card[i];
		add_list(key, card, 0);
		return ;
	}
	for (int i = 0; i <= 4 && i <= total; i++)
	{
		card[index] = i;
		gen_card(card, index+1, total - card[index]);
		card[index+1] = 0;
	}
}

int calcsum(int key)
{
	int tmp = key;
	int sum = 0;

	while (tmp != 0)
	{
		sum += tmp % 10;
		tmp /= 10;
	}

	return sum;
}

int main(int argc, char* argv[])
{
#ifndef TEST_DEBUG
	char card[MJ_NUMBER_COUNT];
	char output[100];
	FILE *fp;

	fp = fopen("common.tbl", "w+");
	if (NULL == fp)
	{
		printf("open file error\n");
		return -1;
	}

	mj_init();
	for (int i = 0; i < 14; i++)
	{
		memset(card, 0, sizeof(card));
		gen_card(card, 0, i);
	}

	printf("total:%d\n", g_mj_node_total);

	stListEntry *entry;
	int total = 0;
	LIST_FOREACH(&g_mj_head, entry)
	{
		total++;
		stHuInfo *node = container_of(entry, stHuInfo, entry);
		if (calcsum(node->cardkey) > 14)
		{
			printf("key:%d error\n", node->cardkey);
		}
		node->probability = calc_probability(node->card, 9, &node->needkey_nojiang);
		node->prob_jiang = calc_probability_withoutjiang(node->card, 9, &node->needkey);
		printf("index:%d,key:%d, prob:%lld, withjiang:%lld,need:%d:%d\n", total, node->cardkey, node->probability, node->prob_jiang, node->needkey_nojiang, node->needkey);
		snprintf(output, 99, "%d\t%lld\t%lld\t%d\t%d\n", node->cardkey, node->probability, node->prob_jiang, node->needkey_nojiang, node->needkey);
		fputs(output, fp);
		fflush(fp);
	}

	fclose(fp);
#else
	char *wan = &handCard[0];
	char *tong = &handCard[9];
	char *tiao = &handCard[18];
	
	//long long prob = calc_probability(wan, 9);
	long long prob = calc_probability_withoutjiang(wan, 9);
	printf("probability:%lld\n", prob);

#endif

	return 0;
}
