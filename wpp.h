#ifndef _WPP_H_
#define _WPP_H_

/* Helpers for default argument based _Generic selection */

typedef struct {}  NArg_0; typedef struct {}  NArg_1; typedef struct {}  NArg_2; typedef struct {}  NArg_3; typedef struct {}  NArg_4; typedef struct {}  NArg_5; typedef struct {}  NArg_6; typedef struct {}  NArg_7; typedef struct {}  NArg_8; typedef struct {}  NArg_9;  
typedef struct {} NArg_10; typedef struct {} NArg_11; typedef struct {} NArg_12; typedef struct {} NArg_13; typedef struct {} NArg_14; typedef struct {} NArg_15; typedef struct {} NArg_16; typedef struct {} NArg_17; typedef struct {} NArg_18; typedef struct {} NArg_19;  
typedef struct {} NArg_20; typedef struct {} NArg_21; typedef struct {} NArg_22; typedef struct {} NArg_23; typedef struct {} NArg_24; typedef struct {} NArg_25; typedef struct {} NArg_26; typedef struct {} NArg_27; typedef struct {} NArg_28; typedef struct {} NArg_29;  
typedef struct {} NArg_30; typedef struct {} NArg_31; typedef struct {} NArg_32; typedef struct {} NArg_33; typedef struct {} NArg_34; typedef struct {} NArg_35; typedef struct {} NArg_36; typedef struct {} NArg_37; typedef struct {} NArg_38; typedef struct {} NArg_39;  
typedef struct {} NArg_40; typedef struct {} NArg_41; typedef struct {} NArg_42; typedef struct {} NArg_43; typedef struct {} NArg_44; typedef struct {} NArg_45; typedef struct {} NArg_46; typedef struct {} NArg_47; typedef struct {} NArg_48; typedef struct {} NArg_49;  
typedef struct {} NArg_50; typedef struct {} NArg_51; typedef struct {} NArg_52; typedef struct {} NArg_53; typedef struct {} NArg_54; typedef struct {} NArg_55; typedef struct {} NArg_56; typedef struct {} NArg_57; typedef struct {} NArg_58; typedef struct {} NArg_59;  
typedef struct {} NArg_60; typedef struct {} NArg_61; typedef struct {} NArg_62; typedef struct {} NArg_63; typedef struct {} NArg_64; typedef struct {} NArg_65; typedef struct {} NArg_66; typedef struct {} NArg_67; typedef struct {} NArg_68; typedef struct {} NArg_69;  

#define WPP_NARG_T(...) \
    WPP_NARG_(_, ##__VA_ARGS__, WPP_RSEQ_T())
#define WPP_NARG(...) \
    WPP_NARG_(_, ##__VA_ARGS__, WPP_RSEQ_N())
#define WPP_NARG_(_,...) \
    WPP_ARG_N(__VA_ARGS__)
#define WPP_ARG_N( \
     _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
    _61,_62,_63,  N, ...) N
#define WPP_RSEQ_T() \
    NArg_63,NArg_62,NArg_61,NArg_60,                   \
    NArg_59,NArg_58,NArg_57,NArg_56,NArg_55,NArg_54,NArg_53,NArg_52,NArg_51,NArg_50, \
    NArg_49,NArg_48,NArg_47,NArg_46,NArg_45,NArg_44,NArg_43,NArg_42,NArg_41,NArg_40, \
    NArg_39,NArg_38,NArg_37,NArg_36,NArg_35,NArg_34,NArg_33,NArg_32,NArg_31,NArg_30, \
    NArg_29,NArg_28,NArg_27,NArg_26,NArg_25,NArg_24,NArg_23,NArg_22,NArg_21,NArg_20, \
    NArg_19,NArg_18,NArg_17,NArg_16,NArg_15,NArg_14,NArg_13,NArg_12,NArg_11,NArg_10, \
     NArg_9, NArg_8, NArg_7, NArg_6, NArg_5, NArg_4, NArg_3, NArg_2, NArg_1, NArg_0
#define WPP_RSEQ_N() \
    63,62,61,60,                   \
    59,58,57,56,55,54,53,52,51,50, \
    49,48,47,46,45,44,43,42,41,40, \
    39,38,37,36,35,34,33,32,31,30, \
    29,28,27,26,25,24,23,22,21,20, \
    19,18,17,16,15,14,13,12,11,10, \
    9,8,7,6,5,4,3,2,1,0

/* for-loop utilities */

#ifndef countof
#define countof(arr) sizeof(arr) / sizeof((arr)[0])
#endif

#define for_array(TYPE, var_name, ...) \
for(size_t arr_idx = 0, __for_stop = 0; __for_stop++ == 0;) \
for(TYPE val_array[] = __VA_ARGS__, var_name = val_array[0] ; arr_idx < countof(val_array); var_name = val_array[++arr_idx])

#define for_each_ref_count(it, list, count) for (__auto_type it = (list); it < (list) + (count); it++)

#define for_each_val_count(el, list, count) \
    for(size_t arr_idx = 0, __for_stop = 0; __for_stop++ == 0;) \
    for(__auto_type el = (list)[0]; arr_idx < (count); el = (list)[++arr_idx])

#define WPP_GET_COUNT(list, ...) _Generic((WPP_NARG_T(__VA_ARGS__)){}, \
    NArg_0: countof(list), \
    NArg_1: ({__VA_ARGS__;}) )

#define for_each_val(it, list, ...) for_each_val_count(it, list, WPP_GET_COUNT(list, __VA_ARGS__))
#define for_each_ref(it, list, ...) for_each_ref_count(it, list, WPP_GET_COUNT(list, __VA_ARGS__))

/* lambda-like function via GCC expression statement and nested functions */

#define lambda(RET_TYPE, FUN_BODY) ({ RET_TYPE __fn__ FUN_BODY __fn__; })
// int (*max)(int, int) = lambda(int, (int x, int y) { return x > y ? x : y; });
// max(4, 5);

#endif