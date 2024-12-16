#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

void move(Player *player, const char *direction) {
    int dir;
    if (strcmp(direction, "up") == 0) dir = UP;
    else if (strcmp(direction, "right") == 0) dir = RIGHT;
    else if (strcmp(direction, "down") == 0) dir = DOWN;
    else if (strcmp(direction, "left") == 0) dir = LEFT;
    else {
        printf("Invalid direction. Use 'up', 'right', 'down', or 'left'.\n");
        return;
    }

    Room *next_room = player->current_room->connections[dir];
    if (next_room) {
        player->current_room = next_room;
        printf("You moved to: %s\n", next_room->name);
    } else {
        printf("You can't move in that direction.\n");
    }
}

void look(Player *player) {
    Room *room = player->current_room;
    printf("You are in: %s\n", room->name);
    printf("%s\n", room->description);

    if (room->item) {
        printf("You see %s: %s\n", room->item->name, room->item->description);
    }

    if (room->enemy && room->enemy->is_alive) {
        printf("An enemy is here: %s with %d HP and %d AP.\n", room->enemy->name, room->enemy->hp, room->enemy->attack);
    }
}

void inventory(Player *player) {
    printf("Inventory:\n");
    if (player->inventory_count == 0) {
        printf("Your inventory is empty.\n");
    } else {
        for (int i = 0; i < player->inventory_count; i++) {
            printf("%d)%s: %s\n",i+1 , player->inventory[i]->name, player->inventory[i]->description);
        }
    }
}

void pickupItem(Player *player, const char *item) {
    Room *room = player->current_room;

    if (room->item && strcmp(room->item->name, item) == 0) {
        if (player->inventory_count < MAX_INVENTORY) {
            player->inventory[player->inventory_count] = room->item;
            player->inventory_count++;
            room->item = NULL;
            printf("You picked up: %s\n", item);
        } else {
            printf("Your inventory is full!\n");
        }
    } else {
        printf("There is no %s here.\n", item);
    }
}

void useItem(Player *player, const char *item_name) {
    for (int i = 0; i < player->inventory_count; i++) {
        if (strcmp(player->inventory[i]->name, item_name) == 0) {
            player->inventory[i]->use(player);

            for (int j = i; j < player->inventory_count - 1; j++) {
                player->inventory[j] = player->inventory[j + 1];
            }
            player->inventory_count--;
            return;
        }
    }
    printf("You don't have a %s.\n", item_name);
}

void dropItem(Player *player, const char *item_name) {
    for (int i = 0; i < player->inventory_count; i++) {
        if (strcmp(player->inventory[i]->name, item_name) == 0) {
            printf("You dropped: %s\n", player->inventory[i]->name);
            free_item(player->inventory[i]);

            for (int j = i; j < player->inventory_count - 1; j++) {
                player->inventory[j] = player->inventory[j + 1];
            }

            player->inventory_count--;
            return;
        }
    }
    printf("You don't have %s in your inventory.\n", item_name);
}


void attack(Player *player) {
    Room *room = player->current_room;

    if (room->enemy && room->enemy->is_alive) {
        while (room->enemy->is_alive && player->hp > 0) {
            printf("The %s attacks you for %d damage.\n",room->enemy->name, room->enemy->attack);
            player->hp -= room->enemy->attack;

            if (player->hp <= 0) {
                printf("You have been defeated.\n");
                exit(0);
            }

            printf("You attack the enemy for %d damage.\n", player->attack);
            room->enemy->hp -= player->attack;

            if (room->enemy->hp <= 0) {
                printf("You defeated the %s!\n",room->enemy->name);
                room->enemy->is_alive = 0;
                player->kill_count++;
            }
        }
    } else {
        printf("There is no enemy here to attack.\n");
    }
}

void stats(Player *player) {
    printf("Statistics:\n");
    printf("- HP: %d\n", player->hp);
    printf("- AP: %d\n", player->attack);
    printf("- Enemies killed: %d\n", player->kill_count);
}

