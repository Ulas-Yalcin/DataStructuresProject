#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CityNode {
  int cityID;
  char cityName[50];
  struct CityNode *firstChild;
  struct CityNode *nextSibling;
} CityNode;
CityNode *city_insert(int cityID, const char *cityName) {
  CityNode *newCity = (CityNode *)malloc(sizeof(CityNode));
  newCity->cityID = cityID;
  strcpy(newCity->cityName, cityName);
  newCity->firstChild = NULL;
  newCity->nextSibling = NULL;
  return newCity;
}

void city_connect(CityNode *parent, CityNode *newCity) {
  if (parent->firstChild == NULL) {
    parent->firstChild = newCity;
  } else {
    CityNode *current = parent->firstChild;
    while (current->nextSibling != NULL) {
      current = current->nextSibling;
    }
    current->nextSibling = newCity;
  }
}

int findShortestPath(CityNode *root, int targetCityID) {
  if (root == NULL)
    return -1;
  if (root->cityID == targetCityID)
    return 0;

  int minDepth = -1;
  CityNode *child = root->firstChild;

  while (child != NULL) {
    int depth = findShortestPath(child, targetCityID);
    if (depth != -1) {
      if (minDepth == -1 || depth + 1 < minDepth) {
        minDepth = depth + 1;
      }
    }
    child = child->nextSibling;
  }

  return minDepth;
}

void printRoutesHelper(CityNode *node, int depth) {
  if (node == NULL)
    return;

  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  printf("└─ %s (ID: %d)\n", node->cityName, node->cityID);

  printRoutesHelper(node->firstChild, depth + 1);
  printRoutesHelper(node->nextSibling, depth);
}

void routesPrint(CityNode *root) {
  printf("Delivery Routes:\n");
  printRoutesHelper(root, 0);
}

CityNode *routesLoad() {

  // Delivery Tree example
  CityNode *headquarters = city_insert(1, "ankara-merkez");
  CityNode *city1 = city_insert(2, "istanbul");
  CityNode *city2 = city_insert(3, "izmir");
  CityNode *city3 = city_insert(4, "antalya");
  CityNode *city4 = city_insert(5, "trabzon");
  CityNode *city5 = city_insert(6, "erzurum");

  CityNode *city_alt1 = city_insert(8, "kocaeli");
  CityNode *city_alt2 = city_insert(9, "edirne");
  CityNode *city_alt3 = city_insert(10, "malatya");
  CityNode *city_alt4 = city_insert(11, "balikesir");
  CityNode *city_alt5 = city_insert(12, "adana");
  CityNode *city_alt6 = city_insert(13, "nigde");
  CityNode *city_alt7 = city_insert(14, "balikesir");
  CityNode *city_alt8 = city_insert(15, "ordu");
  CityNode *city_alt9 = city_insert(16, "agri");
  CityNode *city_alt10 = city_insert(17, "adiyaman");

  city_connect(headquarters, city1);
  city_connect(headquarters, city2);
  city_connect(headquarters, city3);
  city_connect(headquarters, city4);
  city_connect(headquarters, city5);
  city_connect(city1, city_alt1);
  city_connect(city1, city_alt2);
  city_connect(city2, city_alt3);
  city_connect(city2, city_alt4);
  city_connect(city3, city_alt5);
  city_connect(city3, city_alt6);
  city_connect(city4, city_alt7);
  city_connect(city4, city_alt8);
  city_connect(city5, city_alt9);
  city_connect(city5, city_alt10);

  return headquarters;
}

int cityExists(CityNode *root, const char *cityName) {
  if (root == NULL) {
    return 0;
  }
  if (strcasecmp(root->cityName, cityName) == 0) {
    return 1;
  }
  if (cityExists(root->nextSibling, cityName)) {
    return 1;
  }
  if (cityExists(root->firstChild, cityName)) {
    return 1;
  }
  return 0;
}

int find_city_id(CityNode *root, const char *cityName) {
  if (root == NULL) {
    return -1;
  }
  if (strcasecmp(root->cityName, cityName) == 0) {
    return root->cityID;
  }
  int cityID = find_city_id(root->nextSibling, cityName);
  if (cityID != -1) {
    return cityID;
  }
  return find_city_id(root->firstChild, cityName);
};
