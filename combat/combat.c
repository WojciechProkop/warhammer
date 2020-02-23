#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <malloc.h>
#include <time.h>
#include "combat.h"
// ./a.out -w1 -b2 -s3 -t4 -n5 -a6 -d7 -i8 -p9 -f10 -W11 -B12 -S13 -T14 -N15 -A16 -D17 -I18 -P19 -F20





int main(int argc, char **argv)
{
	struct stats teamA;
	struct stats teamB;
	int attacker = 0;

	// Set the stats of both teams to equal 0, so there is no undefined behaviour
	teamA.WS = 0; teamA.BS = 0; teamA.S = 0; teamA.T = 0; teamA.I = 0;
	teamA.Agi = 0; teamA.Dex = 0; teamA.Int = 0; teamA.WP = 0; teamA.Fel = 0;
	teamA.W = 0; teamA.units = 0;
	
	teamB.WS = 0; teamB.BS = 0; teamB.S = 0; teamB.T = 0; teamB.I = 0;
  teamB.Agi = 0; teamB.Dex = 0; teamB.Int = 0; teamB.WP = 0; teamB.Fel = 0;
	teamB.W = 0; teamB.units = 0;
	
	attacker = getOptions(argc, argv, &teamA, &teamB);
	determineWounds(&teamA, &teamB);
	fightCombat(&teamA, &teamB, attacker);
}

void determineWounds(struct stats *teamA, struct stats *teamB)
{
	teamA->W = (teamA->S + teamA->WP + (teamA->T)*2);
	teamB->W = (teamB->S + teamB->WP + (teamB->T)*2);
}
int getRandInt(int limit)
{
	return rand() % limit +1;
}

void fightCombat(struct stats *teamA, struct stats *teamB, int attacker)
{
	//seed the time
	srand(time(NULL));
	int i = 0;
	int aSize = teamA->units;
	int bSize = teamB->units;
	int smallest = 0;
	int difference = 0;
	int *aRolls = calloc(aSize,sizeof(int));
	int *bRolls = calloc(bSize,sizeof(int));
	int aSL = 0;
	int bSL = 0;
	int killed = 0;
	int aCrits = 0;
	int bCrits = 0;
	int aFumbls = 0;
	int bFumbls = 0;
	
	if (aSize > bSize)
		{
			smallest = bSize;
			difference = aSize-bSize;
		}
	else
		{
			smallest = aSize;
			difference = bSize-aSize;
		}

	if (attacker == 0)
		printf("A is attacking B!\n");
	else
		printf("B is attacking A!\n");
	for (i = 0; i < smallest; i++)
		{
			aRolls[i] = getRandInt(100);
			bRolls[i] = getRandInt(100);

			aSL = (teamA->WS - aRolls[i]) / 10;
			bSL = (teamB->WS - bRolls[i]) / 10;
			printf("Roll of a: %d  SL of a: %d\n", aRolls[i], aSL);
			printf("Roll of b: %d  SL of b: %d\n", bRolls[i], bSL);
			if (attacker == 0)
				{
					if (aSL > bSL)
						{
							printf("A won by %d SLs\n\n", aSL-bSL);
							killed++;
						}
					else
						printf("A lost by %d SLs\n\n", aSL-bSL);
				}
			else
				{
					if (bSL > aSL)
						{
							printf("B won by %d SLs\n\n", bSL-aSL);
							killed++;
						}
					else
						printf("B lost by %d SLs\n\n", bSL-aSL);
				}
		}

	if (attacker == 0)
		printf("\nCOMBAT REPORT:\nTeam A killed %d enemy unit(s)\n", killed);
	else
		printf("\nCOMBAT REPORT:\nTeam B killed %d enemy unit(s)\n", killed);

	killed = 0;
}


/*
	Get values for stats for both teams, lowercase is team A, uppercase is team B.
	Letters mean same thing in both cases:

	w - weapon skill
	b - ballistic skill
	s - strength
	t - toughness
	n - initiative
	a - agility
	d - dexterity
	i - intelligence
	p - willpower
	f - fellowship
	u - units // how many soldiers there are on this team
	h - custom wounds amount. If nothing is entered, default calculation for
	    medium sized creature will be used (SB+WPB+TB*2)
			Strength Bonus + Willpower Bonus + 2*Toughness Bonus

	The numbers can be any size

	Neutral flags:
	q -- who is attacking. q = 0 A attacks B; q = 1 B attacks A
 */
int getOptions(int argc, char **argv, struct stats *teamA, struct stats *teamB)
{
	 int c = 0;
	 int qValue = 0;
	 
	 while ((c = getopt (argc, argv, ":w:b:s:t:n:a:d:i:p:f:u:h:W:B:S:T:N:A:D:I:P:F:U:H:q:")) != -1)
		 switch (c)
			 {
			 case 'w':
				 teamA->WS = atoi(optarg);
				 break;
			 case 'b':
				 teamA->BS  = atoi(optarg);
				 break;
			 case 's':
				 teamA->S = atoi(optarg);
				 break;
			 case 't':
         teamA->T = atoi(optarg);
         break;
			 case 'n':
				 teamA->I = atoi(optarg);
				 break;
       case 'a':
				 teamA->Agi = atoi(optarg);
         break;
			 case 'd':
				 teamA->Dex = atoi(optarg);
         break;
       case 'i':
				 teamA->Int  = atoi(optarg);
         break;
       case 'p':
				 teamA->WP = atoi(optarg);
         break;
			 case 'f':
				 teamA->Fel = atoi(optarg);
         break;
			 case 'u':
         teamA->units = atoi(optarg);
         break;
       case 'h':
         teamA->W = atoi(optarg);
         break;
       case 'W':
				 teamB->WS  = atoi(optarg);
         break;
       case 'B':
				 teamB->BS = atoi(optarg);
         break;
       case 'S':
				 teamB->S = atoi(optarg);
         break;
       case 'T':
				 teamB->T  = atoi(optarg);
         break;
       case 'N':
				 teamB->I = atoi(optarg);
         break;
			 case 'A':
				 teamB->Agi = atoi(optarg);
         break;
       case 'D':
				 teamB->Dex  = atoi(optarg);
         break;
       case 'I':
				 teamB->Int = atoi(optarg);
         break;
			 case 'P':
				 teamB->WP  = atoi(optarg);
         break;
       case 'F':
				 teamB->Fel = atoi(optarg);
         break;
			 case 'U':
         teamB->units = atoi(optarg);
         break;
       case 'H':
         teamB->W = atoi(optarg);
				 break;
			 case 'q':
				 qValue = atoi(optarg);
				 break;
			 }
	 return qValue;
}
