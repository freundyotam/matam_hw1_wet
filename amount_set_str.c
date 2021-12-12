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


struct AmountSet_t {
       Node* first;
       Node* currentIteration;
       int length;
};

AmountSet asCreate()
{
    AmountSet amountSet = (AmountSet) malloc(sizeof(AmountSet));
    if(amountSet == NULL){
        return NULL;
    }
    amountSet->first = NULL;
    return amountSet;
}

void asDestroy(AmountSet set)
{
    if(!set){
        return;
    }
    Node* ptr = set->first;
    while(ptr){
        Node* toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
    free(set);
}

AmountSet asCopy(AmountSet set)
{
    AmountSet copy = (AmountSet) malloc(sizeof(AmountSet));
    if(!copy){
        return NULL;
    }
    Node* source = set->first;
    Node* destination;
    while(source){
        destination = (Node*) malloc(sizeof(Node));
        if(!destination){
            return NULL;
        }
        destination->amount = source->amount;
        destination->element = strcpy(destination->element, source->element);
        destination = destination->next;
        source = source->next;
    }
    copy->first = destination;
    copy->length = set->length;
    copy->currentIteration = NULL;
    set->currentIteration = NULL;
    return copy;
}

int asGetSize(AmountSet set)
{
    if(!set){
        return -1;
    }
    return set->length;
}

bool asContains(AmountSet set, const char* element)
{
    if(!set){
        return false;
    }
    AS_FOREACH(char*, setElement, set){
        if(!strcmp(element, setElement)){
            return true;
        }
    }
    return false;
}

AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if(!set || !element || !outAmount){
        return AS_NULL_ARGUMENT;
    }
    int numberOfOccurrences = 0;
    AS_FOREACH(char*, setElement, set){
        if(!strcmp(setElement, element)){
            numberOfOccurrences++;
        }
    }
    if(numberOfOccurrences == 0){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    *outAmount = numberOfOccurrences;
    return AS_SUCCESS;

}
AmountSetResult addAtInit(AmountSet set, const char* element){
    set->first = (Node*) malloc(sizeof(Node));
    if(!set->first){
        return AS_OUT_OF_MEMORY;
    }
    strcpy(set->first->element, element);
    set->first->amount = 0;
    return AS_SUCCESS;
}

AmountSetResult addAtBeginning(AmountSet set, const char* element){
    Node* newFirst = (Node*) malloc(sizeof(Node));
    if(!newFirst){
        return AS_OUT_OF_MEMORY;
    }
    strcpy(newFirst->element, element);
    newFirst->next = set->first;
    newFirst->amount = 0;
    set->first = newFirst;
    return AS_SUCCESS;
}

AmountSetResult addAtEnd(Node iterator, const char* element)
{
    iterator.next = (Node*) malloc(sizeof(Node));
    if(!iterator.next){
        return AS_OUT_OF_MEMORY;
    }
    strcpy(iterator.next->element, element);
    iterator.next->amount = 0;
    return AS_SUCCESS;
}

AmountSetResult addAfterCurrentNode(Node current, const char* element){
    Node* new = (Node*) malloc(sizeof(Node));
    if(!new){
        return AS_OUT_OF_MEMORY;
    }
    strcpy(new->element, element);
    new->next = current.next;
    current.next = new;
    return AS_SUCCESS;
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(!set || !element){
        return AS_NULL_ARGUMENT;
    }
    if(!set->first) {
        return addAtInit(set, element);
    }
    if(strcmp(element, set->first->element) < 0){
        return addAtBeginning(set, element);
    }
    Node iterator = *set->first;
    for(; iterator.next && strcmp(element, iterator.next->element) > 0; iterator = *iterator.next);
    if(!iterator.next){
        return addAtEnd(iterator, element);
    }
    return addAfterCurrentNode(iterator, element);

}

//AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
//{
//
//}
//
//AmountSetResult asDelete(AmountSet set, const char* element)
//{
//
//}
//
//AmountSetResult asClear(AmountSet set)
//{
//
//}
//
char* asGetFirst(AmountSet set)
{
    if(!set || !set->first){
        return NULL;
    }
    set->currentIteration = set->first;
    return set->first->element;
}

char* asGetNext(AmountSet set)
{
    if(!set || !set->currentIteration || !set->currentIteration->next){
        return NULL;
    }
    set->currentIteration = set->currentIteration->next;
    return set->currentIteration->element;

}

int main(int argc, char** a){
    return 0;
}