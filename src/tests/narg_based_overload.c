#include "wpp_test.h"

#define DEST_BUFFER_SIZE 256

int dest_buffer[DEST_BUFFER_SIZE] = {};
size_t dest_idx = 0;

void push (int val) { dest_buffer[dest_idx++] = val; }
 int pop  (void)    { return dest_buffer[dest_idx--]; }
void reset(void)    { memset(dest_buffer, (dest_idx = 0), DEST_BUFFER_SIZE*sizeof(int)); }

void static inline foo_base(int _1, int _2, int _3, int _4) {
    push(_1);
    push(_2);
    push(_3);
    push(_4);
}

#define foo_defarg_4 14
#define foo_defarg_3 13
#define foo_defarg_2 12
#define foo_defarg_1 11

static inline void foo_default_4(int _1, int _2, int _3, int _4)  {return foo_base     (_1, _2, _3, _4);}
static inline void foo_default_3(int _1, int _2, int _3)          {return foo_default_4(_1, _2, _3, foo_defarg_4);}
static inline void foo_default_2(int _1, int _2)                  {return foo_default_3(_1, _2, foo_defarg_3);}
static inline void foo_default_1(int _1)                          {return foo_default_2(_1, foo_defarg_2);}
static inline void foo_default_0()                                {return foo_default_1(foo_defarg_1);}

#define foo(...) _Generic((WPP_NARG_T(__VA_ARGS__)){}, \
    NArg_0: foo_default_0, \
    NArg_1: foo_default_1, \
    NArg_2: foo_default_2, \
    NArg_3: foo_default_3, \
    NArg_4: foo_default_4  \
)(__VA_ARGS__)

WPPT_TEST(narg_based_overload) {
    foo();
    foo(1);
    foo(1, 2);
    foo(1, 2, 3);
    foo(1, 2, 3, 4);

    CHECK(20, dest_idx);
    CHECK_ARRAY(((int[]){
        11,12,13,14,
         1,12,13,14,
         1, 2,13,14,
         1, 2, 3,14,
         1, 2, 3, 4}), dest_buffer, 20);
    reset();
}

WPPT_TEST_MAIN() {
    WPPT_TEST_SET(narg_based_overload);
    WPPT_TEST_RUN_ALL();
    return 0;
}