#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "game.c"
#include "commands.h"
#include "commands.c"


int main() {
    Room ***map = create_map();
    Player *player = create_player("Steve", 100, 10);

    player->current_room = map[0][0];

    char command[50];
    char arg[50];

    printf("Welcome to the Minecraft Dungeons!\n");
    printf("You're Steve and your objective is to clear all the monsters in the cave.");
    printf("\n");
    while (1) {
        printf("\nEnter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        char *space = strchr(command, ' ');
        if (space) {
            *space = '\0';
            strcpy(arg, space + 1);
        } else {
            arg[0] = '\0';
        }

        if (strcmp(command, "move") == 0) {
            move(player, arg);
        } else if (strcmp(command, "look") == 0) {
            look(player);
        } else if (strcmp(command, "inventory") == 0) {
            inventory(player);
        } else if (strcmp(command, "pickup") == 0) {
            pickupItem(player, arg);
        } else if (strcmp(command, "use") == 0) {
            useItem(player, arg);
        } else if (strcmp(command, "drop") == 0){
            dropItem(player,arg);
        } else if (strcmp(command, "attack") == 0) {
            attack(player);
        } else if (strcmp(command, "stats") == 0) {
            stats(player);
        } else if (strcmp(command, "exit") == 0) {
            printf("Minecraft: Dungeons is shutting down...");
            printf("\n");
            free_map(map);
            free_player(player);
            exit(0);
        } else {
            printf("Unknown command.\n");
        }

        if (player->current_room == map[3][0] && player->kill_count == TOTAL_ENEMIES) {
            printf("\nCongratulations You have defeated all the enemies and completed the game.\n");
            printf("Thanks for playing Minecraft: Dungeons!\n");
            printf(">>>>>CREDITS<<<<<\n");
            printf("Mehmet Emin Kaya\n");
            free_map(map);
            free_player(player);
            exit(0);
        }
    }

    
    free_map(map);
    free_player(player);

    return 0;
}
