#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definitions
typedef struct Shipment {
  int shipmentID;
  int deliveryTime;
  char status[50]; // "Processing", "In Transit", "Delivered"
} Shipment;

// priority queue
typedef struct PQNode {
  Shipment data;
  struct PQNode *next;
} PQNode;

typedef struct {
  PQNode *front;
} PriorityQueue;

// --- priority queue ---
PriorityQueue *createPriorityQueue() {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  pq->front = NULL;
  return pq;
}

void enqueueShipment(PriorityQueue *pq, Shipment shipment) {
  PQNode *newNode = (PQNode *)malloc(sizeof(PQNode));
  newNode->data = shipment;

  if (pq->front == NULL ||
      shipment.deliveryTime < pq->front->data.deliveryTime) {
    newNode->next = pq->front;
    pq->front = newNode;
  } else {
    PQNode *current = pq->front;
    while (current->next != NULL &&
           current->next->data.deliveryTime <= shipment.deliveryTime) {
      current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
  }
}

Shipment dequeueShipment(PriorityQueue *pq) {
  Shipment empty = {-1, -1, ""};
  if (pq->front == NULL) {
    return empty;
  }

  PQNode *temp = pq->front;
  Shipment shipment = temp->data;
  pq->front = pq->front->next;
  free(temp);
  return shipment;
}

Shipment peekPriorityShipment(PriorityQueue *pq) {
  Shipment empty = {-1, -1, ""};
  if (pq->front == NULL) {
    return empty;
  }
  return pq->front->data;
}

void test() {
  PriorityQueue *pq = createPriorityQueue();

  Shipment s1 = {1, 2, "Processing"};
  Shipment s2 = {2, 1, "Processing"};
  Shipment s3 = {3, 3, "Processing"};

  enqueueShipment(pq, s1);
  enqueueShipment(pq, s2);
  enqueueShipment(pq, s3);

  printf("PQ test:\n");
  while (1) {
    Shipment s = peekPriorityShipment(pq);
    if (s.shipmentID == -1)
      break;
    printf("shipment_id: %d, time: %d\n", s.shipmentID, s.deliveryTime);
    dequeueShipment(pq);
  }
}
