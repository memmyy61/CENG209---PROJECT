#include "game.h"


Player *create_player(const char *name, int hp, int attack) {
    Player *player = (Player *)malloc(sizeof(Player));
    if (!player) {
        perror("Failed to allocate memory for player");
        exit(EXIT_FAILURE);
    }

    strncpy(player->name, name, MAX_NAME_LENGTH);
    player->hp = hp;
    player->attack = attack;
    player->kill_count = 0; 
    player->inventory_count = 0;
    player->current_room = NULL;

    return player;
}

void free_player(Player *player) {
    if (player) {
        for (int i = 0; i < player->inventory_count; i++) {
            free_item(player->inventory[i]);
        }
        free(player);
    }
}


Creature *create_creature(const char *name, int hp, int attack) {
    Creature *creature = (Creature *)malloc(sizeof(Creature));
    if (!creature) {
        perror("Failed to allocate memory for creature");
        exit(EXIT_FAILURE);
    }

    strncpy(creature->name, name, MAX_NAME_LENGTH);
    creature->hp = hp;
    creature->attack = attack;
    creature->is_alive = 1;

    return creature;
}

void free_creature(Creature *creature) {
    if (creature) {
        free(creature);
    }
}

Item *create_item(const char *name, const char *description, void (*use)(Player *player)) {
    Item *item = (Item *)malloc(sizeof(Item));
    if (!item) {
        perror("Failed to allocate memory for item");
        exit(EXIT_FAILURE);
    }

    strncpy(item->name, name, MAX_NAME_LENGTH);
    strncpy(item->description, description, MAX_DESCRIPTION_LENGTH);
    item->use = use;

    return item;
}

void free_item(Item *item) {
    if (item) {
        free(item);
    }
}


Room *create_room(const char *name, const char *description, Item *item, Creature *enemy) {
    Room *room = (Room *)malloc(sizeof(Room));
    if (!room) {
        perror("Failed to allocate memory for room");
        exit(EXIT_FAILURE);
    }

    strncpy(room->name, name, MAX_NAME_LENGTH);
    strncpy(room->description, description, MAX_DESCRIPTION_LENGTH);
    room->item = item;
    room->enemy = enemy;

    for (int i = 0; i < 4; i++) {
        room->connections[i] = NULL;
    }

    return room;
}

void set_connection(Room *room, Room *connected_room, int direction) {
    if (direction >= 0 && direction < 4) {
        room->connections[direction] = connected_room;
    }
}

void free_room(Room *room) {
    if (room) {
        if (room->item) {
            free_item(room->item);
        }
        if (room->enemy) {
            free_creature(room->enemy);
        }
        free(room);
    }
}

void half_health(Player *player) {
    player->hp += 50;
    printf("Your health point increased by 50. Current HP: %d\n", player->hp);
}

void full_health(Player *player) {
    player->hp += 100;
    printf("Your health point increased by 100. Current HP: %d\n", player->hp);
}

void half_attack(Player *player) {
    player->attack += 10;
    printf("Your attack point increased by 10. Current Attack: %d\n", player->attack);
}

void full_attack(Player *player) {
    player->attack += 20;
    printf("Your attack point increased by 20. Current Attack: %d\n", player->attack);
}
void notUsable(Player *Player) {
    printf("This item is not usable.");
}

Room ***create_map() {
    Room ***map = (Room ***)malloc(MAP_SIZE * sizeof(Room **));
    if (!map) {
        perror("Failed to allocate memory for the map");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        map[i] = (Room **)malloc(MAP_SIZE * sizeof(Room *));
        if (!map[i]) {
            perror("Failed to allocate memory for the map row");
            exit(EXIT_FAILURE);
        }
    }

    Item *health_potion = create_item("Health Potion", "Restores 50 HP.", half_health);
    Item *big_health_potion = create_item("Big Health Potion", "Restores 100 HP.", full_health);
    Item *iron_sword = create_item("Iron Sword", "Increases AP by 10.", half_attack);
    Item *diamond_sword = create_item("Diamond Sword", "Increases AP by 20.", full_attack);
    Item *stick = create_item("Stick","It seems useless.", notUsable);
    Item *cobblestone = create_item("Cobblestone","It seems useless.", notUsable);


    map[0][0] = create_room("Start Point", "You are in the top left corner.", NULL, NULL);
    map[0][1] = create_room("Cave Entry", "There's too many cobwebs inside.", stick, create_creature("Spider", 30, 10));
    map[0][2] = create_room("Cave Middle", "There is a mine entrance down", cobblestone, create_creature("Poisonous Spider", 40, 15));
    map[0][3] = create_room("Cave Corner", "You're in the top right corner of the cave.", health_potion, NULL);
    map[1][1] = create_room("Left Side", "There's no danger down there.", NULL, NULL);
    map[1][2] = create_room("Mine Entrance", "You heard a new monster sound.", big_health_potion, create_creature("Skeleton", 50, 20));
    map[1][3] = create_room("Right Side", "A miner left you his old sword.", iron_sword, create_creature("Baby Zombie", 40, 15));
    map[2][1] = create_room("Abandoned Mine", "It is not recommended to go down.", health_potion, NULL);
    map[2][3] = create_room("Chest Room", "Strange noises coming from down.", health_potion, NULL);
    map[3][0] = create_room("Cave Exit", "Congratulations! You have defeated all the enemies and completed the game.", NULL, NULL);
    map[3][1] = create_room("Boss Area", "you faced the last enemy.", big_health_potion, create_creature("Enderman", 120, 40));
    map[3][2] = create_room("Zombie Area II", "The end of the tunnel is starting to show.", diamond_sword, create_creature("Piglin", 80, 30));
    map[3][3] = create_room("Zombie Area I", "Sounds continue to come from the left", big_health_potion, create_creature("Zombie", 60, 25));
    

    set_connection(map[0][0], map[0][1], 1); 

    set_connection(map[0][1], map[0][0], 3); 
    set_connection(map[0][1], map[0][2], 1); 
    set_connection(map[0][1], map[1][1], 2); 

    set_connection(map[0][2], map[0][1], 3); 
    set_connection(map[0][2], map[0][3], 1); 
    set_connection(map[0][2], map[1][2], 2); 

    set_connection(map[0][3], map[0][2], 3); 
    set_connection(map[0][3], map[1][3], 2); 

    set_connection(map[1][1], map[0][1], 0); 
    set_connection(map[1][1], map[1][2], 1); 
    set_connection(map[1][1], map[2][1], 2); 

    set_connection(map[1][2], map[1][1], 3); 
    set_connection(map[1][2], map[0][2], 0); 
    set_connection(map[1][2], map[1][3], 1); 

    set_connection(map[1][3], map[1][2], 3); 
    set_connection(map[1][3], map[0][3], 0); 
    set_connection(map[1][3], map[2][3], 2); 

    set_connection(map[2][1], map[1][1], 0); 
    set_connection(map[2][1], map[3][1], 2); 

    set_connection(map[2][3], map[1][3], 0); 
    set_connection(map[2][3], map[3][3], 2); 

    set_connection(map[3][0], map[3][1], 1); 

    set_connection(map[3][1], map[3][0], 3); 
    set_connection(map[3][1], map[2][1], 0); 
    set_connection(map[3][1], map[3][2], 1); 

    set_connection(map[3][2], map[3][1], 3); 
    set_connection(map[3][2], map[3][3], 1); 

    set_connection(map[3][3], map[3][2], 3); 
    set_connection(map[3][3], map[2][3], 0); 
    return map;
}

void free_map(Room ***map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j]) {
                free_room(map[i][j]);
            }
        }
        free(map[i]);
    }
    free(map);
}
