#define RUN_TEST(func) \
    printf("Running test : " #func "... "); \
    func();

void testAsCreate();
void testAsDestroy();
void testAsRegisterNullArg();
void testAsRegisterAsItemAlreadyExist();
void  testAsRegisterFewItems();
void testAsCopy();
void testAsSize();
void testAsContains();
void tesAstGetAmount();
void testAsChangeAmount();
void testAsDelete();
void testAsClear();
void testAsGetFirst();
void testAsGetNext();