#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <malloc.h>
#include <time.h>
#include "combat.h"

/*
	Author: Wojciech Prokop
	prokopw@mymacewan.ca or wojtek678@hotmail.com
	
	Program function: To simulate a round of combat in Warhammer Fantasy Role-Play (4th Ed.)
	using a computer. The aim is to be as comprehensive as possible.
*/


// Example run:
// ./a.out -w1 -b2 -s3 -t4 -n5 -a6 -d7 -i8 -p9 -f10 -W11 -B12 -S13 -T14 -N15 -A16 -D17 -I18 -P19 -F20

// What I use:

int main(int argc, char **argv)
{
	struct stats teamA;
	struct stats teamB;
	struct options opts;
	opts.optsList = calloc(3, sizeof(char) * 2);
	
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

// Determine the amount of wounds a unit has if none has been specified
void determineWounds(struct stats *teamA, struct stats *teamB)
{
	teamA->W = (teamA->S + teamA->WP + (teamA->T)*2);
	teamB->W = (teamB->S + teamB->WP + (teamB->T)*2);
}

// Get a random integer
int getRandInt(int limit)
{
	return rand() % limit +1;
}


/*
	Main combat section of the code. If the while loop is enabled, then it acts as a 
	simulator until one side wins.
	TODO: Make it a do-while loop, so that it is able to work as a one-off function.

	PLEASE DO NOT PUT THIS FUNCTION IN A LOOP UNLESS YOU WANT TO GET A STACK
	OVERFLOW!
*/
void fightCombat(struct stats *teamA, struct stats *teamB, int attacker)
{
	//seed the time
	srand(time(NULL));
	int i = 0;
	int aSize = teamA->units;
	int bSize = teamB->units;
	int smallest = 0;
	int difference = 0;
	int hitsRemaining = 0;
	int attackers = 0;
	int defenders = 0;
	char *oldRatio;
	char *newRatio;
	int *aRolls = calloc(aSize,sizeof(int));
	int *bRolls = calloc(bSize,sizeof(int));
	int aSL = 0;
	int bSL = 0;
	int killed = 0;
	int aCrits = 0;
	int bCrits = 0;
	int aFumbls = 0;
	int bFumbls = 0;
	while (teamA->units > 0 && teamB->units > 0)
		{
				 
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
				{
					printf("A is attacking B!\n");
					hitsRemaining = teamA->units;
					defenders = teamB->units;					
				}
			else
				{
					printf("B is attacking A!\n");
					hitsRemaining = teamB->units;
					defenders = teamA->units;
				}
			
			attackers = hitsRemaining;	
			oldRatio = findRatio(attackers, defenders);

			int combatBonus = 0;
			if(attackers/defenders >= 3)
				{
					printf("Combat ratio in decimal: %d\n", atoi(oldRatio));
					combatBonus += 40;
				}
			else if (attackers/defenders >= 2)
				{
					printf("Combat ratio in decimal: %d\n", atoi(oldRatio));
					combatBonus += 20;
				}
			else
				combatBonus = 0;

			printf("CombatBonus: %d\n", combatBonus);
		  do
				{
					aRolls[i] = getRandInt(100);
					bRolls[i] = getRandInt(100);

					if (attacker == 0)
						{
							aSL = ((teamA->WS+combatBonus) - aRolls[i]) / 10;
							bSL = (teamB->WS - bRolls[i]) / 10;
						}
					else
						{
							aSL = (teamA->WS -aRolls[i]) / 10;
							bSL = ((teamB->WS+combatBonus) - bRolls[i]) / 10;
						}

					
					printf("Roll of a: %d  SL of a: %d\n", aRolls[i], aSL);
					printf("Roll of b: %d  SL of b: %d\n", bRolls[i], bSL);
					if (attacker == 0)
						{
							if (aSL > bSL)
								{
									printf("A won by %d SLs\n\n", aSL-bSL);
									defenders--;
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
									defenders--;
									killed++;
								}
							else
								printf("B lost by %d SLs\n\n", bSL-aSL);
						}
					hitsRemaining--;
					combatBonus = 0;
				} while ((hitsRemaining != 0) && defenders >0);
			
			newRatio = findRatio(attackers, defenders);
			if (attacker == 0)
				{
					printf("\nCOMBAT REPORT:\nCombat Ratio(A to B)\nAt Start: %s\nAt End:   %s\n\nTeam A killed %d enemy unit(s)\n",oldRatio, newRatio, killed);
				}
			else
				printf("\nCOMBAT REPORT:\nCombat Ratio(B to A)\nAt Start: %s\nAt End:   %s\n\nTeam B killed %d enemy unit(s)\n",oldRatio, newRatio, killed);
			
			
			if (attacker == 0)
				{
					teamB->units -= killed;
					attacker = 1;
				}
			else
				{
					teamA->units -= killed;
					attacker = 0;
				}
			killed = 0;
			combatBonus = 0;
			oldRatio = NULL;
			newRatio = NULL;
			sleep(1);
		}

	// Free memory
	free(oldRatio);
	free(newRatio);
	free(aRolls);
	free(bRolls);

	// Declare winner
	if (attacker == 0)
		printf("TEAM B HAS WON THE COMBAT!\n");
	else
		printf("TEAM A HAS WON THE COMBAT!\n");
		 
}


/*
	Find a proper reduced ratio of x/y
*/
char* findRatio(int divident, int divisor)
{
	int maxSize = 0;
	int minSize = 0;
	char *output = malloc(sizeof(char)*100);
	
	if (divident > divisor)
		{
			maxSize = divident;
			minSize = divisor;
		}
	else
		{
			maxSize = divisor;
			minSize = divident;
		}

	int i = 0;
	int gcd = 0;
	
	for (i = 1; i < minSize+1; i++)
		{
			if(divident % i == 0 && divisor % i == 0)
				gcd = i;
		}
	
	if (gcd != 0 && minSize > 1)
		sprintf(output, "%d/%d", divident/gcd, divisor/gcd);
	else
		sprintf(output, "%d/%d", divident, divisor);

	return output;
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
	Q -- simulation mode. Q = 0 means run one round of combat. Q = 1 means run until
	one side runs out of combatants

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
