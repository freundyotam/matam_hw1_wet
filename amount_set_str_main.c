#include "amount_set_str_tests.h"
#include <stdio.h>

int main(int argc, char** argv){
    RUN_TEST(testAsCreate);
    RUN_TEST(testAsDestroy);
    RUN_TEST(testAsRegisterNullArg);
    RUN_TEST(testAsRegisterAsItemAlreadyExist);
    RUN_TEST(testAsRegisterFewItems);
    RUN_TEST(testAsCopy);
    RUN_TEST(testAsSize);
    RUN_TEST(testAsContains);
    RUN_TEST(tesAstGetAmount);
    RUN_TEST(testAsChangeAmount);
    RUN_TEST(testAsDelete);
    RUN_TEST(testAsClear);
    RUN_TEST(testAsGetFirst);
    RUN_TEST(testAsGetNext);
    return 0;
}

