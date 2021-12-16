#include "set.h"
#include "list.h"
#include "matamikya.h"
#include "amount_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P1_BIGGER 1
#define P2_BIGGER -1
#define P1_EQUAL_P2 0

struct Matamikya_t{
    AmountSet items;
    Set orders; // The items are amountset
};

typedef struct {
    AmountSet items;
    unsigned int id;
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
    copy = product;
    copy->productData = product->copyDataFunction(product->productData);
    return copy;
}

void freeProduct(Product* product){
    product->freeDataFunction(product->productData);
    free(product);
}

int compareProducts(Product* product1, Product* product2){
    if(product1->id == product2->id){
        return P1_EQUAL_P2;
    }
    return product1->id > product2->id ? P1_BIGGER : P2_BIGGER;

}
// ---end---


Matamikya matamikyaCreate(){
    Matamikya matamikya = (Matamikya) malloc(sizeof(Matamikya));
    matamikya->items = asCreate((void* (*)(void *))copyProduct, (void (*)(void *))freeProduct,
                                (int (*)(void*, void*))compareProducts); // How do I use this??
    matamikya->orders = //set of orders
}
int main(int argc, char** argv){
    return 0;
}
