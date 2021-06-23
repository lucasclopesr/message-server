/*
 * Esse arquivo contém as funções para adicionar, remover e consultar as localizações
 * adicionadas via requisições do cliente.
 * Struct loc: int x, int y: (descreve uma localização)
 * Struct locs: loc[50]: array de localizações
 * add(Struct locs l, int x, int y): Adiciona a localização (x,y) no vetor de localizações 
 * remove(Struct locs l, int x, int y): Remove a localização (x,y) do vetor de localizações
 * list(Struct locs l): Lista as localizações do vetor de localizações
 * Além disso, esse arquivo possui a seguinte função para exibir o estado atual do servidor:
 * print_locs(Struct locs l): lista as localizações no vetor de localizações
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h> 

#define MAX_LOCS 50
#define MAX_LOCS_TO_STRING 1500
#define GREAT_DISTANCE 1000000
#define TRUE 1
#define FALSE 0
#define ADDED 2
#define ALREADY_EXISTS 3
#define REMOVED 4
#define DOES_NOT_EXIST 5
#define FOUND 6
#define LIMIT_EXCEEDED 7

typedef struct {
    int x;
      int y;
} Loc;

typedef struct {
    Loc l[MAX_LOCS];
} Locs;

// Initializes a Locs object with all x and y as -1
void initialize(Locs *locs);

// Gets the next free slot in the Locs vector
// returns 0 in case there are already 50 locations
int getNextFree(Locs locs);

// Verifies if a location already exists
// returns the index if it exists and false otherwise
int locAlreadyExists(Locs locs, int x, int y);

// Tries to add a new location to the list
// Returns FALSE in case something went wrong and TRUE otherwise.
int add(Locs *locs, int x, int y);

// Tries to remove a location from the list
// Returns FALSE in case something went wrong and TRUE otherwise.
int remove_loc(Locs *locs, int x, int y);

const char* list(Locs locs);

// Queries for nearest location
int isEmpty(Locs locs);
Loc query(Locs locs, int x, int y);
Loc get_nearest(Locs locs, int x, int y);
