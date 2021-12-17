#include "set.h"
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
    Set orders;
    int ordersIndex;// The items are amountset
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

Product* getProductById(AmountSet amountSet, const unsigned int id){
    AS_FOREACH(Product*, iterator, amountSet){
        if(iterator->id == id){
            return iterator;
        }
    }
    return NULL;
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
    if(order==NULL){
        return;
    }
    asDestroy(order->items);
    free(order);
}

int orderCompare(Order* order1, Order* order2){
    if(order1->id == order2->id){
        return OBJECTS_EQUAL;
    }
    return order1->id > order2->id ? OBJECT1_BIGGER : OBJECT2_BIGGER;
}

Order* getOrderById(Set orders, const unsigned int orderId){
    SET_FOREACH(Order*, iterator, orders){
        if(iterator->id == orderId){
            return iterator;
        }
    }
    return NULL;
}

bool isAmountInvalid(const double amount, MatamikyaAmountType set){
    return false;
}
// --- end order function----

Matamikya matamikyaCreate(){
    Matamikya matamikya = (Matamikya) malloc(sizeof(Matamikya));
    if(!matamikya){
        return NULL;
    }
    matamikya->items = asCreate((void* (*)(void *))copyProduct, (void (*)(void *))freeProduct,
                                (int (*)(void*, void*))compareProducts);
    matamikya->orders = setCreate((void* (*)(void *))copyOrder, (void (*)(void *))orderFree,
                                                                                (int (*)(void*, void*))orderCompare);
    matamikya->orders = 0;
    return matamikya;
}

void matamikyaDestroy(Matamikya matamikya){
    if(!matamikya){
        return;
    }
    setDestroy(matamikya->orders);
    asDestroy(matamikya->items);
    free(matamikya);
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice){

    if(!(matamikya && name && customData && copyData && freeData)){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(!(('a' <= name[0] && name[0] <= 'z') || ('A' <= name[0] && name[0] <= 'Z')
                                                                            || ('0' <= name[0] && name[0] <= '9'))){
        return MATAMIKYA_INVALID_NAME;
    }
    if(amount < 0 || isAmountInvalid(amount, amountType)){ //TODO check if amount type is compatible with amount
        return MATAMIKYA_INVALID_AMOUNT;
    }
    Product* newProduct = (Product*) malloc(sizeof(Product));
    if (newProduct == NULL){
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    newProduct->id = id;
    strcpy(newProduct->name, name);
    newProduct->productData = copyData(customData);
    newProduct->freeDataFunction = freeData;
    newProduct->copyDataFunction = copyData;
    newProduct->amountType = amountType;
    newProduct->prodPriceFunction = prodPrice;
    AmountSetResult registerResult = asRegister(matamikya->items, newProduct);
    if(registerResult == AS_ITEM_ALREADY_EXISTS){
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    AmountSetResult changeAmountResult = asChangeAmount(matamikya->items, newProduct, amount);
    if(changeAmountResult == AS_INSUFFICIENT_AMOUNT){
        return MATAMIKYA_INVALID_AMOUNT;
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount){
    if(!matamikya){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product* product = getProductById(matamikya->items, id);
    if (product == NULL){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if (isAmountInvalid(amount, product->amountType)){
        return MATAMIKYA_INVALID_AMOUNT;
    }
    AmountSetResult result = asChangeAmount(matamikya->items, product, amount);
    if(result == AS_INSUFFICIENT_AMOUNT){
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id){
    if(!matamikya){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product* product = getProductById(matamikya->items, id);
    if(product == NULL){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    asDelete(matamikya->items,product);
    return MATAMIKYA_SUCCESS;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya){
    if(!matamikya){
        return 0;
    }
    Order* order = (Order*) malloc(sizeof(order));
    if (order == NULL){
        return 0;
    }
    order->id = ++matamikya->ordersIndex;
    order->revenue = 0;
    order->items = asCreate((void* (*)(void *))copyProduct, (void (*)(void *))freeProduct,
                            (int (*)(void*, void*))compareProducts);
    if(!order->items){
        return 0;
    }
    SetResult result = setAdd(matamikya->orders, order);
    if(result != SET_SUCCESS){
        return 0;
    }
    return matamikya->ordersIndex;
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                              const unsigned int productId, const double amount){
    if(!matamikya){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order* order = getOrderById(matamikya->orders, orderId);
    if (!order){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    Product* product = getProductById(matamikya->items, productId);
    if(!product){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if(isAmountInvalid(amount, product->amountType)){
        return MATAMIKYA_INVALID_AMOUNT;
    }

    product = getProductById(order->items, productId);
    if(product){ // Product exists in the order
        AmountSetResult result = asChangeAmount(order->items, product, amount);
        if (result == AS_INSUFFICIENT_AMOUNT){
            asDelete(order->items, product);
        }
    } else { // product not in the order
        product = getProductById(matamikya->items, productId);
        if (amount < 0){
            return MATAMIKYA_INSUFFICIENT_AMOUNT;
        }
        asRegister(order->items, product);
        asChangeAmount(order->items, product, amount);
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId){
    if(!matamikya){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order* order = getOrderById(matamikya->orders, orderId);
    if (!order){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    AS_FOREACH(Product*, iterator, order->items){
        double amountInOrder;
        double amountInMatamikya;
        asGetAmount(order->items, iterator, &amountInOrder);
        asGetAmount(matamikya->items, iterator, &amountInMatamikya);
        if(amountInOrder > amountInMatamikya){
            return MATAMIKYA_INSUFFICIENT_AMOUNT;
        }
    }
    AS_FOREACH(Product*, iterator, order->items){
        double amountInOrder;
        asGetAmount(order->items, iterator, &amountInOrder);
        asChangeAmount(matamikya->items, iterator, -amountInOrder);
        //TODO ADD THE REVENUE FROM THE PRODUCTS
    }
    setRemove(matamikya->orders, getOrderById(matamikya->orders, orderId));

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId){
    if (!matamikya){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order* order = getOrderById(matamikya->orders,orderId);
    if (order == NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    setRemove(matamikya->orders, order);
    return MATAMIKYA_SUCCESS;
}

int main(int argc, char** argv){
    Set s = setCreate((void* (*)(void *))copyOrder, (void (*)(void *))orderFree,
              (int (*)(void*, void*))orderCompare);

    setDestroy(s);
    return 0;
}
