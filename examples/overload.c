#include "../wpp.h"
#include <stdio.h>
#include <stdbool.h>

/* EXAMPLE #1: default argument and argument-number-based overload.
 * 
 * Overload can be made based on the number of arguments passed to a _Generic selection.
 * The selection is still based on a type, but there is a workaround based on creating
 *  a different type for each number of argument.
 * Calling the macro WPP_NARG_T(...) results in returning a type based on the number
 *  of comma-separated argument passed to the macro itself.
 * The type returned are named NArg_N, where *N* is the number of argument.
 * 
 * E.g.: WPP_NARG_T(pippo, pluto, paperino) => NArg_3
 * E.g.: WPP_NARG_T(pippo)                  => NArg_1
 * E.g.: WPP_NARG_T()                       => NArg_0
 * 
 * The overload-like behaviour is then obtained by declaring a different
 *  function for each number of parameter obtained and making each one of
 *  those calling a "base" function with a different number of default
 *  arguments.
 * Each function can call the next one that accepts an increasing number
 *  of arguments, like in this example. This will give the opportunity to
 *  declare only once a default argument (directly into the function call)
 *  or via a macro definition.
 * Since _Generic needs to select based on an "existing variable" type, 
 *  we need to pass a NArg_N-type variable. That can be easily done
 *  with an empty compound literal (Type){}.
 * E.g. (NArg_0){}
 * 
 * Differently from C++, it is NOT mandatory to have default arguments
 *  only as last arguments, since this overload-like behaviour is just
 *  name-mangling under a thin hood.
 * 
 * NOTE: NArg_N types are EMPTY, and they shall not be actively used outside
 *       the _Generic selection or other similar selections.
 */

void static inline printargs_base(int _1, int _2, int _3, int _4) {
    printf("Calling %s...\n", __func__);
    printf("    Parameter #1 is: %d\n", _1);
    printf("    Parameter #2 is: %d\n", _2);
    printf("    Parameter #3 is: %d\n", _3);
    printf("    Parameter #4 is: %d\n", _4);
}

#define printargs_defarg_4 14
#define printargs_defarg_3 13
#define printargs_defarg_2 12
#define printargs_defarg_1 11

static inline void printargs_default_4(int _1, int _2, int _3, int _4)  {return printargs_base     (_1, _2, _3, _4);}
static inline void printargs_default_3(int _1, int _2, int _3)          {return printargs_default_4(_1, _2, _3, printargs_defarg_4);}
static inline void printargs_default_2(int _1, int _2)                  {return printargs_default_3(_1, _2, printargs_defarg_3);}
static inline void printargs_default_1(int _1)                          {return printargs_default_2(_1, printargs_defarg_2);}
static inline void printargs_default_0()                                {return printargs_default_1(printargs_defarg_1);}

#define printargs(...) _Generic((WPP_NARG_T(__VA_ARGS__)){}, \
    NArg_0: printargs_default_0, \
    NArg_1: printargs_default_1, \
    NArg_2: printargs_default_2, \
    NArg_3: printargs_default_3, \
    NArg_4: printargs_default_4  \
)(__VA_ARGS__)

void example_defarg_overload()
{
    printf("EXAMPLE: %s\n", __func__);
    printargs();
    printargs(1);
    printargs(1, 2);
    printargs(1, 2, 3);
    printargs(1, 2, 3, 4);
    printf("\n\n");
}

/* EXAMPLE #2
 *
 * Combining default argument and type-based _Generic selection IS possible.
 * Although, this can lead to a quite long code, but it will eventually give
 *  us a C++-like overload experience.
 * 
 * Selection need to be based separately on the number of arguments and
 *  type-by-type. It is not mandatory to do the selection in a given order,
 *  but in order to keep things "tidy", it is better to select on type first.
 * 
 * Here is given an overly complicated example with a conversely simple function
 *  (a print function!) to show many ways of exploiting _Generic selections 
 *  for the aforementioned purposes.
 * 
 * Our print_values() function will two arguments from any combination of
 *  int or double numbers and an optional argument is_hex that will show
 *  the int values (if present) in a hex format.
 */

static inline void print_values_ii(int x,    int y,    bool is_hex) { is_hex ? printf("x=0x%x" ", y=0x%x" "\n", x, y) : printf("x=%d" ", y=%d" "\n", x, y); }
static inline void print_values_id(int x,    double y, bool is_hex) { is_hex ? printf("x=0x%x" ", y=%f"   "\n", x, y) : printf("x=%d" ", y=%f" "\n", x, y); }
static inline void print_values_di(double x, int y,    bool is_hex) { is_hex ? printf("x=%f"   ", y=0x%x" "\n", x, y) : printf("x=%f" ", y=%d" "\n", x, y); }
static inline void print_values_dd(double x, double y             ) {                                                   printf("x=%f" ", y=%f" "\n", x, y); }

static inline void print_values_ii_default_0(int x,    int y   ) { print_values_ii(x, y, false); }
static inline void print_values_id_default_0(int x,    double y) { print_values_id(x, y, false); }
static inline void print_values_di_default_0(double x, int y   ) { print_values_di(x, y, false); }

#define print_values(x, y, ...) _Generic((x), \
    int: _Generic((y), \
            int: _Generic((WPP_NARG_T(__VA_ARGS__)){}, \
                    NArg_0: print_values_ii_default_0, \
                    NArg_1: print_values_ii ), \
            double: _Generic((WPP_NARG_T(__VA_ARGS__)){}, \
                    NArg_0: print_values_id_default_0, \
                    NArg_1: print_values_id ) \
        ), \
    double: _Generic((y), \
            int: _Generic((WPP_NARG_T(__VA_ARGS__)){}, \
                    NArg_0: print_values_di_default_0, \
                    NArg_1: print_values_di ), \
            double: print_values_dd) \
)(x, y, ##__VA_ARGS__)

void example_defarg_type_overload() {
    printf("EXAMPLE: %s\n", __func__);
    print_values(8, 16, true);
    print_values(8, 16, false);
    print_values(8, 16.0, true);
    print_values(8, 16.0, false);
    print_values(8.0, 16, true);
    print_values(8.0, 16, true);
    print_values(8.0, 16.0);
    print_values(32, 64);
    print_values(32, 64.0);
    print_values(32.0, 64);
    print_values(32.0, 64.0);
    printf("\n\n");
}

/* EXAMPLE #2bis
 *
 * Same as #2, but with helping macros.
 */

#define _GT(name, ...) _Generic((WPP_NARG_T(__VA_ARGS__)){}, NArg_0: name ## _default_0, \
                                                             NArg_1: name)
#define print_values2(x, y, ...) _Generic((x), \
       int: _Generic((y),    int: _GT(print_values_ii, __VA_ARGS__), \
                          double: _GT(print_values_id, __VA_ARGS__)), \
    double: _Generic((y),    int: _GT(print_values_di, __VA_ARGS__), \
                          double:     print_values_dd) \
)(x, y, ##__VA_ARGS__)

void example_defarg_type_overload_helper_macro() {
    printf("EXAMPLE: %s\n", __func__);
    print_values2(8, 16, true);
    print_values2(8, 16, false);
    print_values2(8, 16.0, true);
    print_values2(8, 16.0, false);
    print_values2(8.0, 16, true);
    print_values2(8.0, 16, true);
    print_values2(8.0, 16.0);
    print_values2(32, 64);
    print_values2(32, 64.0);
    print_values2(32.0, 64);
    print_values2(32.0, 64.0);
    printf("\n\n");
}

int main(int argc, char** argv)
{
    example_defarg_overload();
    example_defarg_type_overload();
    example_defarg_type_overload_helper_macro();
    return 0;
}