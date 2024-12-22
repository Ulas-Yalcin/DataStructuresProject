#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.c"

#include "2priority.c"
#include "4gonderi_gecmisi.c"

void menu(void) {
  printf("Yapmak istediğiniz işlemi aşağıdaki menüden seçiniz lütfen! \n");
  printf("............................................................. \n");
  printf("Yeni müşteri ekle..................1 \n");
  printf("Kargo gönderimi ekle...............2 \n");
  printf("Kargo durumu sorgula...............3 \n");
  printf("Gönderim geçmişini görüntüle.......4 \n");
  printf("Tüm kargoları listele..............5 \n");
  printf("Teslimat rotalarını göster.........6 \n");
  printf("Çıkış yap..........................7 \n");
  printf(".................................... \n");
}
int menu_input_read(void) {
  int a;
  scanf("%d", &a);
  return a;
}

PriorityQueue *shipment_queue;

int main() {

  CityNode *headquarters = routesLoad();

  int i;
  int choice;
  printf("Hosgeldiniz \n");

  st *kargo_stack = (st *)malloc(sizeof(st));
  struct Musteri *musteriler = NULL; // Initialize to NULL
  shipment_queue = createPriorityQueue();

  do {
    menu();
    choice = menu_input_read();

    switch (choice) {
    case 1: {
      char musteri_adi_soyadi[50];
      printf("Müşteri adı soyadı: ");
      getchar();
      fgets(musteri_adi_soyadi, sizeof(musteri_adi_soyadi), stdin);
      size_t len = strlen(musteri_adi_soyadi);
      if (len > 0 && musteri_adi_soyadi[len - 1] == '\n') {
        musteri_adi_soyadi[len - 1] = '\0';
      }

      musteri_ekle(&musteriler, musteri_adi_soyadi);
      break;
    }

    case 2: { // kargo gönderimi ekle;
      char aranan_isim[50];
      struct Musteri *musteri = NULL;

      while (musteri == NULL) {
        printf("\nMüşteri adı soyadı giriniz: ");
        getchar();
        fgets(aranan_isim, sizeof(aranan_isim), stdin);
        size_t len = strlen(aranan_isim);
        if (len > 0 && aranan_isim[len - 1] == '\n') {
          aranan_isim[len - 1] = '\0';
        }
        musteri = musteri_bul_isim(musteriler, aranan_isim);
      }

      struct Kargo kargo = kargo_veri_al(headquarters);
      kargo_ekle(musteri, kargo);

      // stack'e pushla
      push(kargo_stack, kargo);

      // shipment olarak ekle
      Shipment shipment;
      shipment.shipmentID = kargo.kargo_id;
      shipment.deliveryTime = kargo.kargo_suresi;
      strncpy(shipment.status, kargo.kargo_durum, sizeof(shipment.status) - 1);
      shipment.status[sizeof(shipment.status) - 1] = '\0';
      enqueueShipment(shipment_queue, shipment);
      break;
    }
    case 3: {
      printf("Kargo ID girin: ");
      int kargo_id;
      scanf("%d", &kargo_id);

      // Find customer's cargo
      struct Musteri *current = musteriler;
      struct Kargo *found_kargo = NULL;

      while (current != NULL) {
        struct Kargo *kargo = current->kargolar;
        while (kargo != NULL) {
          if (kargo->kargo_id == kargo_id) {
            found_kargo = kargo;
            break;
          }
          kargo = kargo->next;
        }
        if (found_kargo)
          break;
        current = current->next;
      }

      if (found_kargo) {
        printf("Kargo Detayları:\n");
        printf("ID: %d\n", found_kargo->kargo_id);
        printf("Teslim Tarihi: %s\n", found_kargo->kargo_teslim_tarihi);
        printf("Durum: %s\n", found_kargo->kargo_durum);
        printf("Süre: %d\n", found_kargo->kargo_suresi);
        printf("Rota: %s\n", found_kargo->rota.sehir_adi);
      } else {
        printf("Kargo bulunamadı\n");
      }
      break;
    }
    case 4: {
      printf("En son 5 gonderiminiz: \n");
      printStack(kargo_stack);
      break;
    }
    case 5: {

      // bütün kargoları tek structta topla
      struct Kargo *all_shipments = NULL;
      int total_shipments = 0;

      struct Musteri *current = musteriler;
      while (current != NULL) {
        struct Kargo *kargo = current->kargolar;
        while (kargo != NULL) {
          total_shipments++;
          kargo = kargo->next;
        }
        current = current->next;
      }

      all_shipments =
          (struct Kargo *)malloc(total_shipments * sizeof(struct Kargo));
      int index = 0;
      current = musteriler;
      while (current != NULL) {
        struct Kargo *kargo = current->kargolar;
        while (kargo != NULL) {
          all_shipments[index++] = *kargo;
          kargo = kargo->next;
        }
        current = current->next;
      }
      // sırala
      mergeSort(all_shipments, 0, total_shipments - 1);
      printf("\n-- Tüm Kargolar (Original Order) --\n");
      current = musteriler;
      while (current != NULL) {
        printf("Müşteri ID: %d, Adı Soyadı: %s\n", current->musteri_id,
               current->musteri_adi_soyadi);
        struct Kargo *kargo = current->kargolar;
        while (kargo != NULL) {
          printf("  Kargo ID: %d, Teslim Tarihi: %s, Durum: %s, Süre: %d, "
                 "Rota: %s\n",
                 kargo->kargo_id, kargo->kargo_teslim_tarihi,
                 kargo->kargo_durum, kargo->kargo_suresi,
                 kargo->rota.sehir_adi);
          kargo = kargo->next;
        }
        current = current->next;
      }
      printf("\n-- Tüm Kargolar (Tahmini süreye göre sıralı) --\n");
      for (int i = 0; i < total_shipments; i++) {
        printf("Kargo ID: %d, Teslim Tarihi: %s, Süre: %d, Rota: %s\n",
               all_shipments[i].kargo_id, all_shipments[i].kargo_teslim_tarihi,
               all_shipments[i].kargo_suresi, all_shipments[i].rota.sehir_adi);
      }
      printf("\n-- Öncelikli Kargolar --\n");
      PQNode *temp = shipment_queue->front;
      while (temp != NULL) {
        printf("Kargo ID: %d, Teslimat Süresi: %d, Durum: %s\n",
               temp->data.shipmentID, temp->data.deliveryTime,
               temp->data.status);
        temp = temp->next;
      }
      free(all_shipments);
      break;
    }
    case 6: {
      routesPrint(headquarters);
      break;
    }
    }

  } while (choice != 7);
  printf("Bizi tercih ettiginiz icin tesekkur ederiz, iyi gunler dileriz.");

  return 0;
}
