#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Max 25
#include <time.h>

int b = 0;              // kargo_id iC'in sayaC'
int musteri_sayaci = 1; // musteri_id iC'in sayaC'

struct Rota {
  int sehir_id;
  char sehir_adi[50];
};

struct Kargo {
  int kargo_id;
  char kargo_teslim_tarihi[9]; // 20240505
  char kargo_durum[50];        // Teslim edildi, Teslim edilemedi
  int kargo_suresi;
  struct Rota rota;
  struct Kargo *next;
};

struct Musteri {
  int musteri_id;
  char musteri_adi_soyadi[50];
  struct Kargo *kargo_listesi;
  struct Musteri *next;
};

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
typedef struct stack {
  int top;
  struct Kargo *item[Max];
} st;

void createEmptyStack(st *s) { s->top = -1; }

int isfull(st *s) { return s->top == Max - 1; }

int isempty(st *s) { return s->top == -1; }

void push(st *s, struct Kargo *kargo) {
  if (isfull(s)) {
    printf("Stack dolu, daha fazla kargo gC6nderemezsiniz.\n");
  } else {
    s->top++;
    s->item[s->top] = kargo;
    printf("Kargo baEarD1yla eklendi.\n");
  }
}

void pop(st *s) {
  if (isempty(s)) {
    printf("\nStack boE.\n");
  } else {
    printf("Kargo ID: %d C'D1kartD1ldD1.\n", s->item[s->top]->kargo_id);
    s->top--;
  }
}

void printLastFive(st *s) {
  printf("\nSon 5 Kargo:\n");
  int start = s->top - 4 >= 0 ? s->top - 4 : 0;
  for (int i = s->top; i >= start; i--) {
    printf("Kargo ID: %d, Gönderi6 Tarihi: %s, Durum: %s, Eehir: %s\n",
           s->item[i]->kargo_id, s->item[i]->kargo_teslim_tarihi,
           s->item[i]->kargo_durum, s->item[i]->rota.sehir_adi);
  }
}

struct Musteri *yeni_musteri(const char *musteri_adi_soyadi) {
  struct Musteri *musteri = (struct Musteri *)malloc(sizeof(struct Musteri));
  musteri->musteri_id = musteri_sayaci++;
  strcpy(musteri->musteri_adi_soyadi, musteri_adi_soyadi);
  musteri->kargo_listesi = NULL;
  musteri->next = NULL;
  return musteri;
}

struct Kargo *merge(struct Kargo *first, struct Kargo *second) {
  if (first == NULL)
    return second;
  if (second == NULL)
    return first;

  struct Kargo *result = NULL;

  if (first->kargo_suresi <= second->kargo_suresi) {
    result = first;
    result->next = merge(first->next, second);
  } else {
    result = second;
    result->next = merge(first, second->next);
  }
  return result;
}

void split(struct Kargo *source, struct Kargo **front, struct Kargo **back) {
  struct Kargo *fast;
  struct Kargo *slow;
  slow = source;
  fast = source->next;

  while (fast != NULL) {
    fast = fast->next;
    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  *front = source;
  *back = slow->next;
  slow->next = NULL;
}

void mergeSort(struct Kargo **headRef) {
  struct Kargo *head = *headRef;
  struct Kargo *a;
  struct Kargo *b;

  if ((head == NULL) || (head->next == NULL)) {
    return;
  }

  split(head, &a, &b);
  mergeSort(&a);
  mergeSort(&b);

  *headRef = merge(a, b);
}

struct Kargo *yeni_kargo(const char *teslim_tarihi, const char *durum,
                         int suresi, int sehir_id, const char *sehir_adi) {
  struct Kargo *kargo = (struct Kargo *)malloc(sizeof(struct Kargo));
  kargo->kargo_id = b++;
  strcpy(kargo->kargo_teslim_tarihi, teslim_tarihi);
  strcpy(kargo->kargo_durum, durum);
  kargo->kargo_suresi = suresi;
  kargo->rota.sehir_id = sehir_id;
  strcpy(kargo->rota.sehir_adi, sehir_adi);
  kargo->next = NULL;
  return kargo;
}

void Menu(void) {
  printf("\nYapmak istediDiniz iElemi aEaDD1daki menC<den seC'iniz:\n");
  printf("1. Yeni mC<Eteri ekle\n");
  printf("2. Kargo gC6nderimi ekle\n");
  printf("3. Kargo durumu sorgula\n");
  printf("4. GC6nderim geC'miEini gC6rC<ntC<le\n");
  printf("5. TC<m kargolarD1 listele\n");
  printf("6. Teslimat rotalarD1nD1 gC6ster\n");
  printf("7. C D1kD1E yap\n");
};

struct Musteri *musteri_bul_isim(struct Musteri *head,
                                 const char *aranan_isim) {
  struct Musteri *current = head;
  while (current != NULL) {
    if (strcasecmp(current->musteri_adi_soyadi, aranan_isim) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

struct Musteri *musteri_bul(struct Musteri *musteriler,
                            const char *musteri_adi_soyadi) {
  struct Musteri *current = musteriler;
  while (current != NULL) {
    if (strcmp(current->musteri_adi_soyadi, musteri_adi_soyadi) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
};

int main() {
  int choice;
  struct Musteri *musteriler = NULL;
  st kargo_stack;
  createEmptyStack(&kargo_stack);
  CityNode *routes = routesLoad();

  printf("HoE geldiniz!\n");
  do {
    Menu();
    printf("SeC'iminiz: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1: {
      char musteri_adi_soyadi[50];
      printf("MC<Eteri adD1 soyadD1: ");
      scanf(" %[^\n]", musteri_adi_soyadi);
      struct Musteri *yeni = yeni_musteri(musteri_adi_soyadi);
      yeni->next = musteriler;
      musteriler = yeni;
      printf("MC<Eteri baEarD1yla eklendi!\n");
      break;
    }
    case 2: {
      char musteri_adi_soyadi[50];
      printf("MC<Eteri adD1 soyadD1: ");
      scanf(" %[^\n]", musteri_adi_soyadi);
      struct Musteri *musteri = musteri_bul(musteriler, musteri_adi_soyadi);
      if (musteri == NULL) {
        printf("musteri bulunamadı");
        break;
      }

      char teslim_tarihi[9], durum[50], sehir_adi[50];
      int suresi, sehir_id;
      printf("Kargo teslim tarihi (YYYYMMDD): ");
      scanf("%s", teslim_tarihi);
      srand(time(NULL));                  // Initialize random seed
      int durum_secim = (rand() % 3) + 1; // Random number between 1-3
      if (durum_secim == 1) {
        strcpy(durum, "İşleme Alındı");
      } else if (durum_secim == 2) {
        strcpy(durum, "Teslimatta");
      } else {
        strcpy(durum, "Teslim Edildi");
      }

      while (1) {
        printf("Kargo RotasD1 (Eehir): ");
        scanf("%s", sehir_adi);

        if (cityExists(routes, sehir_adi)) {
          sehir_id = find_city_id(routes, sehir_adi);
          break;
        } else {
          printf("Hata: Girilen Eehir rota aDacD1nda bulunamadD1!\n");
          printf("Mevcut Eehirler:\n");
          routesPrint(routes); // Show available cities
          printf("LC<tfen listeden geC'erli bir Eehir giriniz.\n");
        }
      }

      suresi = findShortestPath(routes, sehir_id);

      struct Kargo *yeni_k =
          yeni_kargo(teslim_tarihi, durum, suresi, sehir_id, sehir_adi);
      push(&kargo_stack, yeni_k);
      if (musteri->kargo_listesi == NULL) {
        musteri->kargo_listesi = yeni_k;
      } else {
        // Add to beginning of list
        yeni_k->next = musteri->kargo_listesi;
        musteri->kargo_listesi = yeni_k;
      }

      break;
    }
    case 3: {
      char musteri_adi_soyadi[50];
      int kargo_id;
      int found = 0;

      printf("Müşteri adı soyadı: ");
      scanf(" %[^\n]", musteri_adi_soyadi);

      // First check if customer exists
      struct Musteri *musteri = musteri_bul(musteriler, musteri_adi_soyadi);
      if (musteri == NULL) {
        printf("Müşteri bulunamadı.\n");
        break;
      }

      printf("Kargo ID: ");
      scanf("%d", &kargo_id);

      if (isempty(&kargo_stack)) {
        printf("Hiç kargo bulunmamaktadır.\n");
      } else {
        for (int i = 0; i <= kargo_stack.top; i++) {
          struct Kargo *current = kargo_stack.item[i];
          if (current->kargo_id == kargo_id) {
            found = 1;
            printf("\nKargo Durumu:\n");
            printf("Müşteri: %s\n", musteri_adi_soyadi);
            printf("Kargo ID: %d\n", current->kargo_id);
            printf("Gönderi Tarihi: %s\n", current->kargo_teslim_tarihi);
            printf("Durum: %s\n", current->kargo_durum);
            printf("Şehir: %s\n", current->rota.sehir_adi);
            printf("Teslimat Süresi: %d gün\n", current->kargo_suresi);
            break;
          }
        }
        if (!found) {
          printf("Belirtilen ID'ye sahip kargo bulunamadı.\n");
        }
      }
      break;
    }
    case 4: {
      char musteri_adi_soyadi[50];
      printf("MC<Eteri adD1 soyadD1: ");
      scanf(" %[^\n]", musteri_adi_soyadi);
      struct Musteri *musteri = musteri_bul(musteriler, musteri_adi_soyadi);
      if (musteri == NULL) {
        printf("MC<Eteri bulunamadD1.\n");
      } else {
        printLastFive(&kargo_stack);
      }
      break;
    }
    case 5: {
      struct Musteri *current_musteri = musteriler;
      if (current_musteri == NULL) {
        printf("Hiç müşteri bulunmamaktadır.\n");
        break;
      }

      // Create two linked lists for delivered and undelivered cargos
      struct Kargo *teslim_edilmis = NULL;
      struct Kargo *teslim_edilmemis = NULL;
      int kargo_sayisi = 0;

      // Iterate through all customers and their cargos
      while (current_musteri != NULL) {
        struct Kargo *musteri_kargosu = current_musteri->kargo_listesi;

        while (musteri_kargosu != NULL) {
          kargo_sayisi++;
          struct Kargo *current = (struct Kargo *)malloc(sizeof(struct Kargo));
          memcpy(current, musteri_kargosu, sizeof(struct Kargo));
          current->next = NULL;

          if (strcmp(current->kargo_durum, "Teslim Edildi") == 0) {
            // Add to delivered list (sorted by kargo_id)
            if (teslim_edilmis == NULL ||
                current->kargo_id < teslim_edilmis->kargo_id) {
              current->next = teslim_edilmis;
              teslim_edilmis = current;
            } else {
              struct Kargo *temp = teslim_edilmis;
              while (temp->next != NULL &&
                     temp->next->kargo_id < current->kargo_id) {
                temp = temp->next;
              }
              current->next = temp->next;
              temp->next = current;
            }
          } else {
            // Add to undelivered list (will be sorted by delivery time)
            if (teslim_edilmemis == NULL) {
              teslim_edilmemis = current;
            } else {
              current->next = teslim_edilmemis;
              teslim_edilmemis = current;
            }
          }

          musteri_kargosu = musteri_kargosu->next;
        }
        current_musteri = current_musteri->next;
      }

      if (kargo_sayisi == 0) {
        printf("Hiç kargo bulunmamaktadır.\n");
        break;
      }

      // Sort undelivered cargos by delivery time using merge sort
      if (teslim_edilmemis != NULL) {
        mergeSort(&teslim_edilmemis);
      }

      // Print delivered cargos
      printf("\nTeslim Edilmiş Kargolar (Kargo ID'ye göre sıralı):\n");
      printf("------------------------------------------------\n");
      struct Kargo *current = teslim_edilmis;
      while (current != NULL) {
        printf("Kargo ID: %d\n", current->kargo_id);
        printf("Gönderi Tarihi: %s\n", current->kargo_teslim_tarihi);
        printf("Şehir: %s\n", current->rota.sehir_adi);
        printf("Teslimat Süresi: %d gün\n", current->kargo_suresi);
        printf("------------------------------------------------\n");
        current = current->next;
      }

      // Print undelivered cargos
      printf("\nTeslim Edilmemiş Kargolar (Teslimat süresine göre sıralı):\n");
      printf("------------------------------------------------\n");
      current = teslim_edilmemis;
      while (current != NULL) {
        printf("Kargo ID: %d\n", current->kargo_id);
        printf("Gönderi Tarihi: %s\n", current->kargo_teslim_tarihi);
        printf("Durum: %s\n", current->kargo_durum);
        printf("Şehir: %s\n", current->rota.sehir_adi);
        printf("Teslimat Süresi: %d gün\n", current->kargo_suresi);
        printf("------------------------------------------------\n");
        current = current->next;
      }

      // Free allocated memory
      while (teslim_edilmis != NULL) {
        struct Kargo *temp = teslim_edilmis;
        teslim_edilmis = teslim_edilmis->next;
        free(temp);
      }
      while (teslim_edilmemis != NULL) {
        struct Kargo *temp = teslim_edilmemis;
        teslim_edilmemis = teslim_edilmemis->next;
        free(temp);
      }
      break;
    }
    case 6:
      routesPrint(routes);
      break;
    case 7:
      printf("Bizi tercih ettiDiniz iC'in teEekkC<r ederiz, iyi gC<nler!\n");
      break;
    default:
      printf("GeC'ersiz seC'im. LC<tfen tekrar deneyin.\n");
    }

  } while (choice != 7);

  return 0;
}
