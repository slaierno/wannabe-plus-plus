#ifndef _TEST_FRAMEWORK_H_
#define _TEST_FRAMEWORK_H_

#include "wpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <complex.h>
#include <string.h>
#include <assert.h>

bool wppt_test_result = true;

#define WPPT_UPDATE_TEST_RESULT(result) ({ wppt_test_result = wppt_test_result && (result); })
/* EQUALITY FUNCTIONS */
#define WPPT_EQ_FUN_NAME(suffix) wppt_is_equal_ ## suffix

#define WPPT_FP_EPS(x) _Generic((x+I),\
    float complex      : 0x2p-23F, \
    double complex     : 0x2p-52,   \
    long double complex: 0x2p-52L    )
#define WPPT_ABS(x) ((x) > 0 ? (x) : -(x))
#define WPPT_DST(x, y) WPPT_ABS((x) - (y))
#define WPPT_CHECK_FLOAT_EQ(e, a) (WPPT_DST(e, a) < WPPT_FP_EPS(e))
#define WPPT_EQ_FUN_EXACT_TEMPLATE(suffix, TYPE) bool WPPT_EQ_FUN_NAME(suffix)(TYPE e, TYPE a) {return (e) == (a);}
#define WPPT_EQ_FUN_FLOAT_TEMPLATE(suffix, TYPE) \
    bool WPPT_EQ_FUN_NAME(suffix)(TYPE const e, TYPE const a) {return WPPT_CHECK_FLOAT_EQ(e,a);} \
    bool WPPT_EQ_FUN_NAME(suffix ## c)(TYPE complex const e, TYPE complex const a) {return (WPPT_CHECK_FLOAT_EQ(creal(e), creal(a)) && WPPT_CHECK_FLOAT_EQ(cimag(e), cimag(a)));}

    WPPT_EQ_FUN_EXACT_TEMPLATE(ull, unsigned long long)
    WPPT_EQ_FUN_EXACT_TEMPLATE( ll,          long long)
    WPPT_EQ_FUN_FLOAT_TEMPLATE(  f,              float)
    WPPT_EQ_FUN_FLOAT_TEMPLATE(  d,             double)
    WPPT_EQ_FUN_FLOAT_TEMPLATE( ld,        long double)

bool WPPT_EQ_FUN_NAME(ptr)(void* e, void* a, size_t size) {return size ? !memcmp(e, a, size) : (e == a);}

#undef WPPT_EQ_FUN_FLOAT_TEMPLATE
#undef WPPT_EQ_FUN_EXACT_TEMPLATE
#undef WPPT_CHECK_FLOAT_EQ
#undef WPPT_DST
#undef WPPT_ABS
#undef WPPT_FP_EPS

/* PRINTER HELPERS */
#define WPPT_PRINT_FUN_NAME(suffix) wppt_print_value_ ## suffix

#define WPPT_PRINT_FUN_TEMPLATE(suffix, TYPE, fmt_code) void WPPT_PRINT_FUN_NAME(suffix)(const char* prefix, TYPE value) {printf("%s" fmt_code "\n", prefix, value);}
#define WPPT_PRINT_FUN_TEMPLATE_CMPLX(suffix, TYPE, fmt_code) void WPPT_PRINT_FUN_NAME(suffix ## c)(const char* prefix, TYPE complex const value) {printf("%s" fmt_code "\n", prefix, creal(value), cimag(value));}

    WPPT_PRINT_FUN_TEMPLATE      (ull, unsigned long long, "%llu")
    WPPT_PRINT_FUN_TEMPLATE      ( ll,          long long, "%lld")
    WPPT_PRINT_FUN_TEMPLATE      (ptr,              void*, "0x%p")
    WPPT_PRINT_FUN_TEMPLATE      (  f,              float, "%f")
    WPPT_PRINT_FUN_TEMPLATE      (  d,             double, "%f")
    WPPT_PRINT_FUN_TEMPLATE      ( ld,        long double, "%Lf")
    WPPT_PRINT_FUN_TEMPLATE_CMPLX(  f,              float, "%f%+f*I")
    WPPT_PRINT_FUN_TEMPLATE_CMPLX(  d,             double, "%f%+f*I")
    // WPPT_PRINT_FUN_TEMPLATE_CMPLX(ldc,  long double, "%Lf%+Lf*I")
    // TODO why creal does not call creall with a long double complex argument?
    void WPPT_PRINT_FUN_NAME(ldc)(const char* prefix, long double complex value) {printf("%s" "%Lf%+Lf*I" "\n", prefix, creall(value), cimagl(value));}

#undef PRINT_FUN_TEMPLATE
#undef PRINT_FUN_TEMPLATE_CMPLX

#define WPPT_PRINT_VALUE(prefix, val) _Generic((val), \
    unsigned long long: WPPT_PRINT_FUN_NAME(ull), \
             long long: WPPT_PRINT_FUN_NAME(ll),  \
                 void*: WPPT_PRINT_FUN_NAME(ptr), \
                 float: WPPT_PRINT_FUN_NAME(f), \
                double: WPPT_PRINT_FUN_NAME(d), \
           long double: WPPT_PRINT_FUN_NAME(ld), \
         float complex: WPPT_PRINT_FUN_NAME(fc), \
        double complex: WPPT_PRINT_FUN_NAME(dc), \
   long double complex: WPPT_PRINT_FUN_NAME(ldc))(prefix, val)

/* CHECK FUNCTIONS */
#define WPPT_CHECK_FUN_NAME(suffix) wppt_check_ ## suffix

void wppt_print_ptr_diff(void* a, void* e, size_t size)
{
    if(size == 0) {
        printf("  Difference: %td\n", a - e);
    } else {
        char* a0 = a, *e0 = e;
        bool excess = (size > 64);
        if(excess) size = 64;
        for(int i = 0; i < size; i++) {
            if(!(i%8)) printf("\n");
            printf("%02x ", e0[i]);
        }
        printf("\n");
        printf("------------------------");
        for(int i = 0; i < size; i++) {
            if(!(i%8)) printf("\n");
            printf("%02x ", a0[i]);
        }
        printf("\n");
    }
}
void wppt_print_ldc_diff(long double complex a, long double complex e, size_t size){return;}

#define WPPT_TEMPLATE_CHECK_FUNCTION(suffix, TYPE, ...) \
bool WPPT_CHECK_FUN_NAME(suffix)(bool result, bool assert, TYPE const e, TYPE const a, size_t size, const char* F, const int L, const char* f, const char* stmt, const int argc, ...) \
{ \
    if (WPPT_EQ_FUN_NAME(suffix)(e,a,##__VA_ARGS__) != result) { \
        printf("%s:%d %s - failed check \"%s\"\n", F, L, f, stmt); \
        WPPT_PRINT_VALUE("    Expected: ", e); \
        WPPT_PRINT_VALUE("      Actual: ", a); \
        _Generic((TYPE)(0), void*: wppt_print_ptr_diff, default: wppt_print_ldc_diff)(a, e, size); \
        if (argc) { \
            va_list arglist; \
            va_start (arglist, argc); \
            const char* fmt = va_arg(arglist, const char*); \
            vprintf(fmt, arglist); \
            va_end(arglist); \
        } \
        WPPT_UPDATE_TEST_RESULT(false); \
        if (assert) exit(-1); \
        return false; \
    } \
    return WPPT_UPDATE_TEST_RESULT(true); \
}

    WPPT_TEMPLATE_CHECK_FUNCTION(ull,  unsigned long long)
    WPPT_TEMPLATE_CHECK_FUNCTION( ll,           long long)
    WPPT_TEMPLATE_CHECK_FUNCTION(ptr,               void*, size)
    WPPT_TEMPLATE_CHECK_FUNCTION(  f,               float)
    WPPT_TEMPLATE_CHECK_FUNCTION(  d,              double)
    WPPT_TEMPLATE_CHECK_FUNCTION( ld,         long double)
    WPPT_TEMPLATE_CHECK_FUNCTION( fc,       float complex)
    WPPT_TEMPLATE_CHECK_FUNCTION( dc,      double complex)
    WPPT_TEMPLATE_CHECK_FUNCTION(ldc, long double complex)

#undef WPPT_TEMPLATE_CHECK_FUNCTION
#undef WPPT_PRINT_VALUE
#undef WPPT_PRINT_FUN_NAME
#undef WPPT_EQ_FUN_NAME

#define WPPT_CHECK_BASE(result, assert, e, a, size, stmt, ...) \
    _Generic(((e) + _Generic((e),                    \
                            unsigned char     : 0ULL, \
                            unsigned short    : 0ULL,  \
                            unsigned int      : 0ULL,   \
                            unsigned long     : 0ULL,    \
                            unsigned long long: 0ULL,     \
                            default           : 0LL        )),\
    unsigned long long: WPPT_CHECK_FUN_NAME(ull), \
             long long: WPPT_CHECK_FUN_NAME(ll),  \
                 float: WPPT_CHECK_FUN_NAME(f),   \
                double: WPPT_CHECK_FUN_NAME(d),   \
           long double: WPPT_CHECK_FUN_NAME(ld),  \
         float complex: WPPT_CHECK_FUN_NAME(fc),  \
        double complex: WPPT_CHECK_FUN_NAME(dc),  \
   long double complex: WPPT_CHECK_FUN_NAME(ldc), \
               default: WPPT_CHECK_FUN_NAME(ptr)  \
    )(result, assert, e, a, size, __FILE__, __LINE__, __func__, stmt, WPP_NARG(__VA_ARGS__), ##__VA_ARGS__)

#define WPPT_MACRO_STR(NAME, ...) #NAME "(" #__VA_ARGS__ ")"
#define  FAIL_MSG(msg) ({printf("%s:%d %s - FAIL: %s\n" , __FILE__, __LINE__, __func__, msg);         false;})
#define ABORT_MSG(msg) ({printf("%s:%d %s - ABORT: %s\n", __FILE__, __LINE__, __func__, msg);exit(-1);false;})

/*                       NAME(args      , ...)                                           WPPT_CHECK_BASE(result, assert,  exp,      act, size, WPPT_MACRO_STR(            MACRO_NAME               ), ##__VA_ARGS__) */
#define            CHECK_STMT(stmt,       ...) ({                                        WPPT_CHECK_BASE(  true,  false, true, !!(stmt),    0, WPPT_MACRO_STR(CHECK_STMT, stmt, ##__VA_ARGS__)            , ##__VA_ARGS__);})
#define       CHECK_STMT_FAIL(stmt,       ...) ({                                        WPPT_CHECK_BASE( false,  false, true, !!(stmt),    0, WPPT_MACRO_STR(CHECK_STMT_FAIL, stmt, ##__VA_ARGS__)       , ##__VA_ARGS__);})
#define           ASSERT_STMT(stmt,       ...) ({                                        WPPT_CHECK_BASE(  true,   true, true, !!(stmt),    0, WPPT_MACRO_STR(ASSERT_STMT, stmt, ##__VA_ARGS__)           , ##__VA_ARGS__);})
#define      ASSERT_STMT_FAIL(stmt,       ...) ({                                        WPPT_CHECK_BASE( false,   true, true, !!(stmt),    0, WPPT_MACRO_STR(ASSERT_STMT_FAIL, stmt, ##__VA_ARGS__)      , ##__VA_ARGS__);})
#define                 CHECK(e, a,       ...) ({                                        WPPT_CHECK_BASE(  true,  false,    e,        a,    0, WPPT_MACRO_STR(CHECK, e, a, ##__VA_ARGS__)                 , ##__VA_ARGS__);})
#define                ASSERT(e, a,       ...) ({                                        WPPT_CHECK_BASE(  true,   true,    e,        a,    0, WPPT_MACRO_STR(ASSERT, e, a, ##__VA_ARGS__)                , ##__VA_ARGS__);})
#define            CHECK_FAIL(e, a,       ...) ({                                        WPPT_CHECK_BASE( false,  false,    e,        a,    0, WPPT_MACRO_STR(CHECK_FAIL, e, a, ##__VA_ARGS__)            , ##__VA_ARGS__);})
#define           ASSERT_FAIL(e, a,       ...) ({                                        WPPT_CHECK_BASE( false,  true ,    e,        a,    0, WPPT_MACRO_STR(ASSERT_FAIL, e, a, ##__VA_ARGS__)           , ##__VA_ARGS__);})
#define             CHECK_MEM(e, a, size, ...) ({ size_t s =               size; if(s>0) WPPT_CHECK_BASE(  true,  false,    e,        a,    s, WPPT_MACRO_STR(CHECK_MEM, e, a, size, ##__VA_ARGS__)       , ##__VA_ARGS__);})
#define          CHECK_STRUCT(e, a,       ...) ({ size_t s =       sizeof(*(e)); if(s>0) WPPT_CHECK_BASE(  true,  false,    e,        a,    s, WPPT_MACRO_STR(CHECK_STRUCT, e, a, ##__VA_ARGS__)          , ##__VA_ARGS__);})
#define           CHECK_ARRAY(e, a,  cnt, ...) ({ size_t s = (cnt)*sizeof(*(e)); if(s>0) WPPT_CHECK_BASE(  true,  false,    e,        a,    s, WPPT_MACRO_STR(CHECK_ARRAY, e, a, cnt, ##__VA_ARGS__)      , ##__VA_ARGS__);})
#define            ASSERT_MEM(e, a, size, ...) ({ size_t s =               size; if(s>0) WPPT_CHECK_BASE(  true,   true,    e,        a,    s, WPPT_MACRO_STR(ASSERT_MEM, e, a, size, ##__VA_ARGS__)      , ##__VA_ARGS__);})
#define         ASSERT_STRUCT(e, a,       ...) ({ size_t s =       sizeof(*(e)); if(s>0) WPPT_CHECK_BASE(  true,   true,    e,        a,    s, WPPT_MACRO_STR(ASSERT_STRUCT, e, a, ##__VA_ARGS__)         , ##__VA_ARGS__);})
#define          ASSERT_ARRAY(e, a,  cnt, ...) ({ size_t s = (cnt)*sizeof(*(e)); if(s>0) WPPT_CHECK_BASE(  true,   true,    e,        a,    s, WPPT_MACRO_STR(ASSERT_ARRAY, e, a, cnt, ##__VA_ARGS__)     , ##__VA_ARGS__);})
#define        CHECK_MEM_FAIL(e, a, size, ...) ({ size_t s =               size; if(s>0) WPPT_CHECK_BASE( false,  false,    e,        a,    s, WPPT_MACRO_STR(CHECK_MEM_FAIL, e, a, size, ##__VA_ARGS__)  , ##__VA_ARGS__); else{ FAIL_MSG(WPPT_MACRO_STR(CHECK_MEM_FAIL, e, a, size, ##__VA_ARGS__)  " - Memory check automatically leads to true with size == 0");}})
#define     CHECK_STRUCT_FAIL(e, a,       ...) ({ size_t s =       sizeof(*(e)); if(s>0) WPPT_CHECK_BASE( false,  false,    e,        a,    s, WPPT_MACRO_STR(CHECK_STRUCT_FAIL, e, a, ##__VA_ARGS__)     , ##__VA_ARGS__); else{ FAIL_MSG(WPPT_MACRO_STR(CHECK_STRUCT_FAIL, e, a, ##__VA_ARGS__)     " - Memory check automatically leads to true with size == 0");}})
#define      CHECK_ARRAY_FAIL(e, a,  cnt, ...) ({ size_t s = (cnt)*sizeof(*(e)); if(s>0) WPPT_CHECK_BASE( false,  false,    e,        a,    s, WPPT_MACRO_STR(CHECK_ARRAY_FAIL, e, a, cnt, ##__VA_ARGS__) , ##__VA_ARGS__); else{ FAIL_MSG(WPPT_MACRO_STR(CHECK_ARRAY_FAIL, e, a, cnt, ##__VA_ARGS__) " - Memory check automatically leads to true with size == 0");}})
#define       ASSERT_MEM_FAIL(e, a, size, ...) ({ size_t s =               size; if(s>0) WPPT_CHECK_BASE( false,   true,    e,        a,    s, WPPT_MACRO_STR(ASSERT_MEM_FAIL, e, a, size, ##__VA_ARGS__) , ##__VA_ARGS__); else{ABORT_MSG(WPPT_MACRO_STR(ASSERT_MEM_FAIL, e, a, size, ##__VA_ARGS__) " - Memory check automatically leads to true with size == 0");}})
#define    ASSERT_STRUCT_FAIL(e, a,       ...) ({ size_t s =       sizeof(*(e)); if(s>0) WPPT_CHECK_BASE( false,   true,    e,        a,    s, WPPT_MACRO_STR(ASSERT_STRUCT_FAIL, e, a, ##__VA_ARGS__)    , ##__VA_ARGS__); else{ABORT_MSG(WPPT_MACRO_STR(ASSERT_STRUCT_FAIL, e, a, ##__VA_ARGS__)    " - Memory check automatically leads to true with size == 0");}})
#define     ASSERT_ARRAY_FAIL(e, a,  cnt, ...) ({ size_t s = (cnt)*sizeof(*(e)); if(s>0) WPPT_CHECK_BASE( false,   true,    e,        a,    s, WPPT_MACRO_STR(ASSERT_ARRAY_FAIL, e, a, cnt, ##__VA_ARGS__), ##__VA_ARGS__); else{ABORT_MSG(WPPT_MACRO_STR(ASSERT_ARRAY_FAIL, e, a, cnt, ##__VA_ARGS__)" - Memory check automatically leads to true with size == 0");}})

/* TEST FUNCTIONS, TEST CASES, ETC */

typedef void (*WPPT_TEST_T)(void);
typedef struct {
    WPPT_TEST_T test_function;
    char test_name[128];
} WPPT_TEST_CASE_T;

#define WPPT_TEST_PREFIX wppt_test_function_
#define WPPT_TEST(name) \
const char * WPPT_TEST_PREFIX ## name ## _str = #name; \
void WPPT_TEST_PREFIX ## name () 

#define WPPT_TEST_MAIN() \
WPPT_TEST_CASE_T* wppt_test_list; \
          size_t  wppt_test_count; \
void __attribute__((constructor)) wppt_test_begin(void) { wppt_test_list = malloc(0); wppt_test_count = 0; } \
void __attribute__((destructor))    wppt_test_end(void) { if(wppt_test_list) free(wppt_test_list); } \
int main(int argc, char** argv)

#define WPPT_TEST_SET(name) \
wppt_test_list = realloc(wppt_test_list, (wppt_test_count+1)*sizeof(*wppt_test_list)); \
wppt_test_list[wppt_test_count].test_function = WPPT_TEST_PREFIX ## name; \
strcpy(wppt_test_list[wppt_test_count].test_name, #name); \
wppt_test_count++;

#define WPPT_TEST_RUN_BASE(fun, name) \
wppt_test_result = true; \
printf("Executing test \"%s\"\n", name); \
fun(); \
printf("...done! Result: %s\n", wppt_test_result ? "OK" : "FAIL");

#define WPPT_TEST_RUN(name) WPPT_TEST_RUN_BASE(WPPT_TEST_PREFIX ## name, #name);

#ifndef WPP_NO_FOR_LOOP_UTILS
#define WPPT_TEST_RUN_ALL() ({ \
    for_each_ref_count(test, wppt_test_list, wppt_test_count) { \
        WPPT_TEST_RUN_BASE((test->test_function), test->test_name) \
    } \
})
#else
#define WPPT_TEST_RUN_ALL() ({ \
    for(int i = 0; i < wppt_test_count; i++) { \
        WPPT_TEST_RUN_BASE((wppt_test_list[i].test_function), wppt_test_list[i].test_name) \
    } \
})
#endif
#endif