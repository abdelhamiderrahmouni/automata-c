#include <stdio.h>
#include <string.h>

#define MAX_STATES 5
#define ALPHABET_SIZE 2

int EF[MAX_STATES] = {4}; // Les états finaux
int TR[MAX_STATES][ALPHABET_SIZE]; // Les transitions
char TM[100]; // Le mot à tester

int AFD() {
    int e = 0; // L'état initial
    int i = 0;
    char s = TM[i]; // Convertir le caractère en indice

    while (TR[e][s] != -1 && i < strlen(TM)) {
        e = TR[e][s];
        i++;
        s = TM[i];
    }

    for (int j = 0; j < sizeof(EF) / sizeof(EF[0]); j++) {
        if (e == EF[j]) {
            return 1;
        }
    }

    return 0;
}

int main() {
    // Initialiser toutes les transitions à -1
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            TR[i][j] = -1;
        }
    }

    // Définir les transitions pour l'automate
    TR[0]['a'] = 1;
    TR[1]['a'] = 2;
    TR[2]['b'] = 3;
    TR[3]['b'] = 4;
    TR[4]['a'] = 1;

    // Lire le mot à tester
    printf("Entrez le mot à tester: ");
    scanf("%s", TM);

    // Tester le mot
    if (AFD()) {
        printf("Reconnu\n");
    } else {
        printf("Non reconnu\n");
    }

    return 0;
}
