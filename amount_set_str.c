#include "amount_set_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    amountSet->length = 0;
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

static Node* linkNewNode(Node** nextPointer, const char* string, int amount){
    Node* node = (Node*) malloc(sizeof(Node));
    if(!node){
        return NULL;
    }
    char* nodeString = (char*) malloc(strlen(string) + 1);
    if(!nodeString){
        free(node);
        return NULL;
    }
    strcpy(nodeString, string);
    node->element = nodeString;
    node->amount = amount;
    node->next = *nextPointer;
    *nextPointer = node;
    return node;
}

AmountSet asCopy(AmountSet set)
{
    AmountSet copy = asCreate();
    if(!copy){
        return NULL;
    }
    Node** destination = &copy->first;
    for(Node* source = set->first; source; source = source->next){
        Node* newNode = linkNewNode(destination, source->element, source->amount);
        if(!newNode){
            asDestroy(copy);
            return NULL;
        }
        destination = &newNode->next;
    }
    copy->length = set->length;
    return copy;
}

int asGetSize(AmountSet set)
{
    if(!set){
        return -1;
    }
    return set->length;
}

static Node** findNode(AmountSet set, const char* element){
    Node** nextNodePointer = &set->first;
    for(; *nextNodePointer; nextNodePointer = &(*nextNodePointer)->next){
        if(!strcmp((*nextNodePointer)->element, element)){
            break;
        }
    }
    return nextNodePointer;
}

bool asContains(AmountSet set, const char* element)
{
    if(!set || !element){
        return false;
    }
    return *findNode(set, element);
}

AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if(!set || !element || !outAmount){
        return AS_NULL_ARGUMENT;
    }

    Node** node = findNode(set, element);
    if(!*node){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    *outAmount = (*node)->amount;
    return AS_SUCCESS;
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(!set || !element){
        return AS_NULL_ARGUMENT;
    }
    Node** nextNodePointer = &set->first;
    for(; *nextNodePointer ;nextNodePointer = &(*nextNodePointer)->next){
        int compareResult = strcmp((*nextNodePointer)->element, element);
        if(compareResult == 0){
            return AS_ITEM_ALREADY_EXISTS;
        }
        if (compareResult > 1){
            break;
        }
    }
    linkNewNode(nextNodePointer, element, 0);
    set->length++;
    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
{
    if(!set || !element){
        return AS_NULL_ARGUMENT;
    }
    Node** node = findNode(set, element);
    if(!*node){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    if(-amount > (*node)->amount){
        return AS_INSUFFICIENT_AMOUNT;
    }
    (*node)->amount += amount;
    return AS_SUCCESS;
}
AmountSetResult asDelete(AmountSet set, const char* element)
{
    if(!set || !element){
        return AS_NULL_ARGUMENT;
    }
    Node** previous = findNode(set, element);
    Node* toDelete = *previous;
    if(!toDelete) {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    *previous = toDelete->next;
    free(toDelete->element);
    free(toDelete);
    set->length--;
    return AS_SUCCESS;
}

AmountSetResult asClear(AmountSet set)
{
    if(!set){
        return AS_NULL_ARGUMENT;
    }
    while(set->first){
        asDelete(set, set->first->element);
    }
    return AS_SUCCESS;
}

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
// TODO Add undefined the current iterator for all functions that makes it undefined
// TODO Turn stuff into static methods
// TODO Add Tests
