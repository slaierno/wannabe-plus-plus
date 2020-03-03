#include "../wpp_test.h"

typedef struct {
    int a;
    int b;
} TestStruct;

WPPT_TEST(test_framework_primitive) {
    /* TEST PRIMITIVE TYPES (no pointers) */
#define TEST_PRIMITIVE_TYPE(Type, val) { Type i = val, j = val; ASSERT(i,j); }
    TEST_PRIMITIVE_TYPE(bool, true);
    TEST_PRIMITIVE_TYPE(bool, false);
    TEST_PRIMITIVE_TYPE(char, 'a');
    TEST_PRIMITIVE_TYPE(signed char, -'a');
    TEST_PRIMITIVE_TYPE(short, 0);
    TEST_PRIMITIVE_TYPE(unsigned short, 0);
    TEST_PRIMITIVE_TYPE(int, 0);
    TEST_PRIMITIVE_TYPE(unsigned int, 0);
    TEST_PRIMITIVE_TYPE(long, 0);
    TEST_PRIMITIVE_TYPE(unsigned long, 0);
    TEST_PRIMITIVE_TYPE(long long, 0);
    TEST_PRIMITIVE_TYPE(unsigned long long, 0);
    TEST_PRIMITIVE_TYPE(float, 0.0);
    TEST_PRIMITIVE_TYPE(double, 0.0);
    TEST_PRIMITIVE_TYPE(long double, 0.0);
    TEST_PRIMITIVE_TYPE(float complex, 1.0 + I);
    TEST_PRIMITIVE_TYPE(double complex, 1.0 + I);
    TEST_PRIMITIVE_TYPE(long double complex, 1.0 + I);
#undef TEST_PRIMITIVE_TYPE

    ASSERT_FAIL(1, 0, "Unexpected!\n");
    ASSERT_FAIL(1+I, 1+2*I, "Unexpected!\n");
    ASSERT_FAIL(0, 1);
}

int test_function(void) {return 0;}

WPPT_TEST(test_framework_pointers) {

    /* TEST POINTER */
    {
        int i = 0, *i_p = &i, *j_p = &i; 
        ASSERT( &i, &i);
        ASSERT( &i, i_p);
        ASSERT( &i, j_p);
        ASSERT(i_p, j_p);
    }

    /* TEST FUNCTION POINTER */
    {
        int (*fptr)(void) = test_function;
        ASSERT(fptr, fptr);
        ASSERT(test_function, fptr);
        ASSERT(test_function, test_function);
    }
}

WPPT_TEST(test_framework_memory) {
    TestStruct t1 = {10, 12};
    TestStruct t2 = {10, 12};
    TestStruct t3 = {10, 13};
    TestStruct ta1[] = {{10, 13}, {10,15}};
    TestStruct ta2[] = {{10, 13}, {10,15}};
    TestStruct ta3[] = {{10, 13}, {10,17}};

    ASSERT_STMT( !memcmp(&t1, &t2, sizeof(TestStruct)));
    ASSERT_STMT(!!memcmp(&t1, &t3, sizeof(TestStruct)));
    ASSERT_STMT(!!memcmp(&t2, &t3, sizeof(TestStruct)));
    ASSERT_STMT( !memcmp(&ta1, &ta2, sizeof(TestStruct)*countof(ta1)));
    ASSERT_STMT(!!memcmp(&ta1, &ta3, sizeof(TestStruct)*countof(ta1)));
    ASSERT_STMT(!!memcmp(&ta2, &ta3, sizeof(TestStruct)*countof(ta2)));
    ASSERT_STMT( !memcmp(&ta1, &ta3, sizeof(TestStruct)));
    ASSERT_STMT( !memcmp(&ta2, &ta3, sizeof(TestStruct)));
    ASSERT_STRUCT(&t1, &t2);
    ASSERT_STRUCT_FAIL(&t1, &t3);
    ASSERT_STRUCT_FAIL(&t2, &t3);
    ASSERT_MEM(&t1, &t2, sizeof(t1));
    ASSERT_MEM_FAIL(&t1, &t3, sizeof(t1));
    ASSERT_MEM_FAIL(&t2, &t3, sizeof(t1));
    ASSERT_MEM(&ta1, &ta2, sizeof(*ta1));
    ASSERT_MEM(&ta1, &ta2, sizeof(*ta1)*countof(ta1));
    ASSERT_ARRAY(ta1, ta2, countof(ta1));
    ASSERT_MEM(ta1, ta3, sizeof(*ta1));
    ASSERT_MEM_FAIL(ta1, ta3, sizeof(ta1[0])*countof(ta1));
    ASSERT_ARRAY(ta1, ta3, 1);
    ASSERT_ARRAY_FAIL(ta1, ta3, 2);
    ASSERT_MEM(ta2, ta3, sizeof(*ta1));
    ASSERT_MEM(ta2, ta3, sizeof(*ta1));
    ASSERT_MEM_FAIL(ta2, ta3, sizeof(ta1[0])*countof(ta2));
    ASSERT_ARRAY(ta2, ta3, 1);
}

WPPT_TEST_MAIN() {
    WPPT_TEST_SET(test_framework_primitive);
    WPPT_TEST_SET(test_framework_pointers);
    WPPT_TEST_SET(test_framework_memory);
    WPPT_TEST_RUN_ALL();
    return 0;
}