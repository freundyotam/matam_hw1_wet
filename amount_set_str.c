#include "amount_set_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -1
#define FOUND 0


typedef struct Node {
    struct Node* next;
    char* element;
    int amount;
} Node;


typedef struct {
       Node first;
       Node currentIteration;
       int length;
} AmountSet_t;

AmountSet asCreate()
{
    AmountSet amountSet = (*AmountSet) malloc(sizeof(AmountSet));
    if(amountSet == NULL){
        return NULL;
    }
    amountSet->first = NULL;
    return *amountSet;
}

void asDestroy(AmountSet set)
{
    if(!set){
        return;
    }
    Node ptr = set.first;
    while(ptr){
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
    if(set){
        free(set);
    }
}

AmountSet asCopy(AmountSet set)
{
    AmountSet copy = (*AmountSet) malloc(sizeof(AmountSet));
    if(!copy){
        return NULL;
    }
    Node* source = set.first;
    Node* destination;
    while(source){
        destination = (*Node) malloc(sizeof(Node));
        if(!destination){
            return NULL;
        }
        destination->amount = source->amount;
        destination->element = strcpy(destination->element, source->element);
        destination = destination->next;
        source = source->next;
    }
    copy.first = destination;
    copy.length = set.length;
    copy.currentIteration = NULL;
    set.currentIteration = NULL
}

int asGetSize(AmountSet set)
{
    if(!set){
        return -1;
    }
    return set.length;
}

bool asContains(AmountSet set, const char* element)
{
    if(!set){
        return false;
    }
    int isFound = NOT_FOUND;
    for(char* iterator = asGetFirst(set) ; isFound = strcmp(iterator, element) != FOUND
            && !iterator; iterator = asGetNext(set)); // Iterate until strcmp give equal strings or reached end of list
    return isFound == FOUND;
}

AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount)
{

}

AmountSetResult asRegister(AmountSet set, ASElement element)
{

}

AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount)
{

}

AmountSetResult asDelete(AmountSet set, ASElement element)
{

}

AmountSetResult asClear(AmountSet set)
{

}

char* asGetFirst(AmountSet set)
{

}

char* asGetNext(AmountSet set)
{

}