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

#include <locations.h>

// Initializes a Locs object with all x and y as -1
void initialize(Locs *locs){
  int i;
  for (i = 0; i < MAX_LOCS; i++){
    locs->l[i].x = -1;
    locs->l[i].y = -1;
  }
}

// Gets the next free slot in the Locs vector
// returns -1 in case there are already 50 locations
int getNextFree(Locs locs){
  int i;
  for (i = 0; i < MAX_LOCS; i++){
    if (locs.l[i].x == -1 && locs.l[i].y == -1){
      return i;
    }
  }

  return -1; // Locations list is full
}

// Verifies if a location already exists
// returns the index if it exists and -1 otherwise
int locAlreadyExists(Locs locs, int x, int y){
  int i;
  for (i = 0; i < MAX_LOCS; i++){
    if (locs.l[i].x == x && locs.l[i].y == y){
      return i;
    }
  }

  return -1;
}

// Tries to add a new location to the list
// Returns FALSE in case something went wrong and TRUE otherwise.
int add(Locs *locs, int x, int y){
  Loc l;
  l.x = x;
  l.y = y;

  if (x < 0 || y < 0) {
    // //printf("bad input\n");
    return FALSE;
  }

  int locExists = locAlreadyExists(*locs, x, y);

  if (locExists < 0){
    int next = getNextFree(*locs);
    if (next >= 0) {
      locs->l[next] = l;
      // //printf("%d %d added\n", x, y);
    } else {
      //printf("limit exceeded\n");
      return LIMIT_EXCEEDED;
    }
  } else {
    //printf("%d %d already exists\n", x, y);
    return ALREADY_EXISTS;
  }

  return ADDED;
}

// Tries to remove a location from the list
// Returns FALSE in case something went wrong and TRUE otherwise.
int remove_loc(Locs *locs, int x, int y){
  Loc l;
  l.x = -1;
  l.y = -1;

  int locExists = locAlreadyExists(*locs, x, y);

  if (locExists >= 0){
    locs->l[locExists] = l;
    //printf("%d %d removed\n", x, y);
  } else {
    //printf("%d %d does not exist\n", x, y);
    return DOES_NOT_EXIST;
  }

  return REMOVED;
}

const char* list(Locs locs){
  int i;
  char loc[10];
  char *list = (char*)  malloc(MAX_LOCS_TO_STRING * sizeof(char));
  char *final_list = (char*)  malloc(MAX_LOCS_TO_STRING * sizeof(char) - 1);

  if (isEmpty(locs))
    list = "none";

  for (i = 0; i < MAX_LOCS; i++){
    if (locs.l[i].x != -1 && locs.l[i].y != -1){
      sprintf(loc, "%d %d ", locs.l[i].x, locs.l[i].y);
      strcat(list, loc);
    }
  }

  if (isEmpty(locs)) {
    final_list = "none";
  } else {
    strncpy(final_list, list, strlen(list) -1);
  }

  return final_list;
}

int isEmpty(Locs locs){
  int i, c = 0;
  for (i = 0; i < MAX_LOCS; i++){
    if (locs.l[i].x != -1 && locs.l[i].y != -1){
      c++;
    }
  }

  if (c == 0)
    return TRUE;

  return FALSE;
}

Loc get_nearest(Locs locs, int x, int y){
  Loc l;
  float min_d = GREAT_DISTANCE;
  float d;
  int i;

  if (isEmpty(locs)) {
    l.x = -1;
    l.y = -1;
    return l;
  }

  for (i = 0; i < MAX_LOCS; i++){
    if (locs.l[i].x != -1 && locs.l[i].y != -1){
      d = sqrt( pow((x - locs.l[i].x),2) + pow((y - locs.l[i].y),2));
      if (d < min_d) {
        min_d = d;
        l.x = locs.l[i].x;
        l.y = locs.l[i].y;
      }
    }
  }

  return l;
}

// Queries for nearest location
Loc query(Locs locs, int x, int y){
  Loc nearest;

  nearest = get_nearest(locs, x, y);
  //if (nearest.x == -1 && nearest.y == -1)
    //printf("none\n");

  //printf("Nearest: (%d,%d)\n", nearest.x, nearest.y);
  return nearest;
}
