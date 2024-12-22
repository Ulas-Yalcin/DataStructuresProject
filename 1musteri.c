#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3rota.c"

int sayac_musteri = 0;
int sayac_kargo = 0;

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
  struct Kargo *kargolar;

  struct Musteri *next;
};

// Yeni müşteri oluşturma
struct Musteri *yeni_musteri(int musteri_id, const char *musteri_adi_soyadi) {
  struct Musteri *musteri = (struct Musteri *)malloc(sizeof(struct Musteri));
  musteri->musteri_id = musteri_id;
  strcpy(musteri->musteri_adi_soyadi, musteri_adi_soyadi);
  musteri->kargolar = NULL;
  musteri->next = NULL;
  return musteri;
}

void musteri_ekle(struct Musteri **head, const char *ad_soyad) {
  struct Musteri *yeni = yeni_musteri(sayac_musteri++, ad_soyad);

  if (*head == NULL) {
    *head = yeni; // If list is empty
  } else {
    // Add to end of list
    struct Musteri *current = *head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = yeni;
  }
}

void kargo_ekle(struct Musteri *musteri, struct Kargo kargo) {
  struct Kargo *yeni_kargo = (struct Kargo *)malloc(sizeof(struct Kargo));
  *yeni_kargo = kargo;
  yeni_kargo->next = NULL;

  if (musteri->kargolar == NULL) {
    // If this is the first cargo
    musteri->kargolar = yeni_kargo;
  } else {
    // Add to the end of the list
    struct Kargo *temp = musteri->kargolar;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = yeni_kargo;
  }
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

struct Kargo kargo_veri_al(CityNode *routes) {
  struct Kargo kargo;
  kargo.kargo_id = sayac_kargo++;
  printf("Teslim Tarihi (YYYYMMDD): ");
  scanf("%s", kargo.kargo_teslim_tarihi);
  // Kargo durumu validation
  while (1) {
    printf("Kargo Durumu (1: Teslim Edildi, 2: Teslim Edilmedi): ");
    int durum_secim;
    if (scanf("%d", &durum_secim) == 1) {
      if (durum_secim == 1) {
        strcpy(kargo.kargo_durum, "Teslim Edildi");
        break;
      } else if (durum_secim == 2) {
        strcpy(kargo.kargo_durum, "Teslim Edilmedi");
        break;
      }
    }
    // Clear input buffer
    while (getchar() != '\n')
      ;

    printf("Hatalı giriş! Lütfen 1 veya 2 seçiniz.\n");
  }
  printf("Kargo Rotası: ");
  while (1) {
    printf("Kargo Rotası (Şehir): ");
    scanf("%s", kargo.rota.sehir_adi);

    if (cityExists(routes, kargo.rota.sehir_adi)) {
      kargo.rota.sehir_id = find_city_id(routes, kargo.rota.sehir_adi);
      break;
    } else {
      printf("Hata: Girilen şehir rota ağacında bulunamadı!\n");
      printf("Mevcut şehirler:\n");
      routesPrint(routes); // Show available cities
      printf("Lütfen listeden geçerli bir şehir giriniz.\n");
    }
  }

  kargo.kargo_suresi = findShortestPath(routes, kargo.rota.sehir_id);

  return kargo;
}

struct Musteri *musteri_bul(struct Musteri *head, int aranan_id) {
  struct Musteri *current = head;

  while (current != NULL) {
    if (current->musteri_id == aranan_id) {
      return current; // Found the customer
    }
    current = current->next;
  }

  return NULL; // Customer not found
}

void merge(struct Kargo arr[], int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  struct Kargo L[n1], R[n2];

  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  i = 0;
  j = 0;
  k = left;

  while (i < n1 && j < n2) {
    if (L[i].kargo_suresi <= R[j].kargo_suresi) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(struct Kargo arr[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

int binarySearch(struct Kargo arr[], int l, int r, int kargo_id) {
  if (r >= l) {
    int mid = l + (r - l) / 2;
    if (arr[mid].kargo_id == kargo_id)
      return mid;
    if (arr[mid].kargo_id > kargo_id)
      return binarySearch(arr, l, mid - 1, kargo_id);
    return binarySearch(arr, mid + 1, r, kargo_id);
  }
  return -1;
}
