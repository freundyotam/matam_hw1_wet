#include "set.h"
#include "list.h"
#include "matamikya.h"
#include "amount_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBJECT1_BIGGER 1
#define OBJECT2_BIGGER -1
#define OBJECTS_EQUAL 0

struct Matamikya_t{
    AmountSet items;
    Set orders; // The items are amountset
};

typedef struct {
    AmountSet items;
    unsigned int id;
    double revenue;
} Order;

typedef struct {
    unsigned int id;
    char* name;
    MatamikyaAmountType amountType;
    MtmProductData productData;
    MtmCopyData copyDataFunction;
    MtmFreeData freeDataFunction;
    MtmGetProductPrice prodPriceFunction;
} Product;


// Product functions for amount set
Product* copyProduct(Product* product){
    Product* copy = (Product*) malloc(sizeof(Product));
    *copy = *product;
    copy->productData = product->copyDataFunction(product->productData);
    strcpy(copy->name, product->name);
    return copy;
}

void freeProduct(Product* product){
    product->freeDataFunction(product->productData);
    free(product);
}

int compareProducts(Product* product1, Product* product2){
    if(product1->id == product2->id){
        return OBJECTS_EQUAL;
    }
    return product1->id > product2->id ? OBJECT1_BIGGER : OBJECT2_BIGGER;
}

// ---end---
// ---Order functions---
Order* copyOrder(Order* order){
    Order* copy = (Order*) malloc(sizeof(Order));
    *copy = *order;
    copy->items = asCopy(order->items);
    return copy;
}

void orderFree(Order* order){
    asDestroy(order->items);
    free(order);
}

int orderCompare(Order* order1, Order* order2){
    if(order1->id == order2->id){
        return OBJECTS_EQUAL;
    }
    return order1->id > order2->id ? OBJECT1_BIGGER : OBJECT2_BIGGER;
}
// --- end order function----

Matamikya matamikyaCreate(){
    Matamikya matamikya = (Matamikya) malloc(sizeof(Matamikya));
    matamikya->items = asCreate((void* (*)(void *))copyProduct, (void (*)(void *))freeProduct,
                                (int (*)(void*, void*))compareProducts); // How do I use this??
    matamikya->orders = setCreate((void* (*)(void *))copyOrder, (void (*)(void *))orderFree,
                                                                                (int (*)(void*, void*))orderCompare);
    return matamikya;
}
int main(int argc, char** argv){
    return 0;
}
