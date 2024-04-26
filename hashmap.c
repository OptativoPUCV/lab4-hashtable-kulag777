#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  if (map == NULL || key == NULL) {
    return;
  }
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL &&
         map->buckets[posicion]->key != NULL) {
    if (is_equal(map->buckets[posicion]->key, key)) {
      map->buckets[posicion]->value = value;
      return;
    }
    posicion = (posicion + 1) % map->capacity;
  }
  if (map->size == map->capacity) {
    enlarge(map);
  }
  map->buckets[posicion] = createPair(key, value);
  map->size++;
}

void enlarge(HashMap *map) {
  enlarge_called = 1; // no borrar (testing purposes)
}

HashMap *createMap(long capacity) {
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  if (map == NULL)
    return NULL;
  map->buckets = (Pair **)malloc(capacity * sizeof(Pair *));
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;
  return map;
}
void eraseMap(HashMap *map, char *key) {
  if (map == NULL || key == NULL) {
    return;
  }
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL &&
         map->buckets[posicion]->key != NULL) {
    if (is_equal(map->buckets[posicion]->key, key)) {
      map->buckets[posicion]->key = NULL;
      map->buckets[posicion]->value = NULL;
      map->size--;
      return;
    }
    posicion = (posicion + 1) % map->capacity;
  }
}

Pair *searchMap(HashMap *map, char *key) {
  if (map == NULL || key == NULL) {
    return NULL;
  }
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL &&
         map->buckets[posicion]->key != NULL) {
    if (is_equal(map->buckets[posicion]->key, key)) {
      map->current = posicion;
      return map->buckets[posicion];
    }
    posicion = (posicion + 1) % map->capacity;
  }
  return NULL;
}

Pair *firstMap(HashMap *map) {
  if (map == NULL) {
    return NULL;
  }
  while (map->current + 1 < map->capacity) {
    if (map->buckets[map->current] != NULL &&
        map->buckets[map->current]->key != NULL) {
      return map->buckets[map->current];
    }
    map->current++;
  }
  map->current = -1;
  return NULL;
}

Pair *nextMap(HashMap *map) { return NULL; }
