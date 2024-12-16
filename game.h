#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 200
#define MAX_INVENTORY 3
#define MAP_SIZE 4
#define TOTAL_ENEMIES 7


typedef struct Player Player;
typedef struct Creature Creature;
typedef struct Item Item;
typedef struct Room Room;

struct Player {
    char name[MAX_NAME_LENGTH];
    int hp;
    int attack;
    int kill_count; 
    Item *inventory[MAX_INVENTORY];
    int inventory_count;
    Room *current_room;
};

struct Creature {
    char name[MAX_NAME_LENGTH];
    int hp;
    int attack;
    int is_alive;
};

struct Item {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    void (*use)(Player *player);
};

struct Room {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    Item *item;
    Creature *enemy;
    Room *connections[4]; 
};

Player *create_player(const char *name, int hp, int attack);
void free_player(Player *player);


Creature *create_creature(const char *name, int hp, int attack);
void free_creature(Creature *creature);


Item *create_item(const char *name, const char *description, void (*use)(Player *player));
void free_item(Item *item);


Room *create_room(const char *name, const char *description, Item *item, Creature *enemy);
void set_connection(Room *room, Room *connected_room, int direction);
void free_room(Room *room);


Room ***create_map();
void free_map(Room ***map);

#endif // GAME_H


