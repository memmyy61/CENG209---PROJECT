#ifndef COMMANDS_H
#define COMMANDS_H

#include "game.h"

void move(Player *player, const char *direction);
void look(Player *player);
void inventory(Player *player);
void pickupItem(Player *player, const char *item);
void useItem(Player *player, const char *item_name);
void dropItem(Player *player, const char *item_name);
void attack(Player *player);
void stats(Player *player); 

#endif // COMMANDS_H

