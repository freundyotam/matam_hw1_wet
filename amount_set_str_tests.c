#include <assert.h>
#include <string.h>
#include "amount_set_str.h"
#include "amount_set_str_tests.h"

#define TEMPLATE_SIZE 4

static AmountSet getSet(){
    return asCreate();
}
void testAsCreate(){
    AmountSet set = getSet();
    assert(set!=NULL);
    asDestroy(set);
}

void testAsDestroy(){
    AmountSet set = getSet();
    assert(set!=NULL);
    asDestroy(set);
    assert(set==NULL);
}

void testAsRegisterNullArg(){
    AmountSet set = getSet();
    AmountSetResult result = asRegister(set,NULL);
    assert(result == AS_NULL_ARGUMENT);
    asDestroy(set);
}
void testAsRegisterAsItemAlreadyExist(){
    AmountSet set = getSet();
    assert(asRegister(set,"a") == AS_SUCCESS);
    assert(asRegister(set,"a") == AS_ITEM_ALREADY_EXISTS);
    asDestroy(set);
}
void testAsRegisterFewItems(){
    AmountSet set = getSet();
    assert(asRegister(set,"asdf") == AS_SUCCESS);
    assert(asRegister(set,"234123") == AS_SUCCESS);
    assert(asRegister(set,"slfloa") == AS_SUCCESS);
    asDestroy(set);
}

static AmountSet createTemplateSet()
{
    AmountSet as = asCreate();
    asRegister(as,"a1");
    asRegister(as,"a2");
    asRegister(as,"a3");
    asRegister(as,"a4");
    return as;
}

void testAsCopy(){
    AmountSet set1 = createTemplateSet();
    AmountSet set2 = asCopy(set1);
    char* iter1 = asGetFirst(set1);
    char* iter2 = asGetFirst(set2);
    assert(!strcmp(iter1, iter2));
    while((iter1 = asGetNext(set1)) && (iter2 = asGetNext(set2))){
        assert(!strcmp(iter2, iter1));
    }
    assert(!iter1 && !iter2);
}

void testAsSize(){
    AmountSet set1 = createTemplateSet();
    assert(asGetSize(set1) == TEMPLATE_SIZE);
}

void testAsContains(){
    AmountSet set1 = createTemplateSet();
    assert(asContains(set1, "a1"));
    assert(asContains(set1, "a2"));
    assert(asContains(set1, "a3"));
    assert(asContains(set1, "a4"));
    assert(!asContains(set1, "not_registered"));
}

void tesAstGetAmount(){
    AmountSet set = createTemplateSet();
    double amount;
    double newAmount1 = 4.3;
    asGetAmount(set,"a1", &amount);
    assert(amount == 0);
    asGetAmount(set,"a2", &amount);
    assert(amount == 0);
    asChangeAmount(set,"a2", newAmount1);
    asGetAmount(set,"a2", &amount);
    assert(amount == newAmount1);
}

void testAsChangeAmount(){
    AmountSet set = createTemplateSet();
    double amount = 4;
    double newAmount;
    asChangeAmount(set,"a1", amount);
    asGetAmount(set, "a1", &newAmount);
    assert(newAmount == amount);
}

void testAsDelete(){
    AmountSet set = createTemplateSet();
    assert(AS_SUCCESS == asDelete(set,"a4"));
    assert(AS_ITEM_DOES_NOT_EXIST == asDelete(set, "a5"));
    assert(!asContains(set, "a4"));
}

void testAsClear(){
    AmountSet set = createTemplateSet();
    asClear(set);
    assert(asGetFirst(set) == NULL);
}

void testAsGetFirst(){
    AmountSet set = createTemplateSet();
    assert(strcmp("a1", asGetFirst(set)));
}

void testAsGetNext(){
    AmountSet set = createTemplateSet();
    assert(strcmp("a1", asGetFirst(set)));
    assert(strcmp("a2", asGetNext(set)));
}