#include "../wpp_test.h"

#define DEST_BUFFER_SIZE 256

int dest_buffer[DEST_BUFFER_SIZE] = {};
size_t dest_idx = 0;

void push (int val) { dest_buffer[dest_idx++] = val; }
 int pop  (void)    { return dest_buffer[dest_idx--]; }
void reset(void)    { memset(dest_buffer, (dest_idx = 0), DEST_BUFFER_SIZE*sizeof(int)); }

WPPT_TEST(for_loops) {
    for_array(int, i, {1, 2, 3, 5, 7, 11, 13, 17})
        push(i);

    CHECK(8, dest_idx);
    CHECK_ARRAY(((int[]){1, 2, 3, 5, 7, 11, 13, 17}), dest_buffer, 8);
    reset();

    int arr[] = {0,2,4,6};
    for_each_val(i, arr, 2)
        push(i);
    CHECK(2, dest_idx);
    CHECK_ARRAY(((int[]){0, 2}), dest_buffer, 2);
    reset();

    for_each_val(i, arr)
        push(i);
    CHECK(4, dest_idx);
    CHECK_ARRAY(((int[]){0, 2, 4, 6}), dest_buffer, 4);
    reset();

    int arr2[] = {1,3,5,7};
    for_each_ref(i, arr2, 2)
        push((*i)++);
    CHECK(2, dest_idx);
    CHECK_ARRAY(((int[]){1, 3}), dest_buffer, 2);
    reset();

    for_each_ref(i, arr2)
        push((*i));
    CHECK(4, dest_idx);
    CHECK_ARRAY(((int[]){2, 4, 5, 7}), dest_buffer, 4);
    reset();
}

WPPT_TEST_MAIN() {
    WPPT_TEST_SET(for_loops);
    WPPT_TEST_RUN_ALL();
    return 0;
}