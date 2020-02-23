

struct stats{
	int WS, BS, S, T, I,
    Agi, Dex, Int, WP, Fel, W, units;
};

void determineWounds(struct stats *teamA, struct stats *teamB);
int getOptions(int argc, char **argv, struct stats *teamA, struct stats *teamB);
void fightCombat(struct stats *teamA, struct stats *teamB, int attacker);
int getRandInt(int limit);
