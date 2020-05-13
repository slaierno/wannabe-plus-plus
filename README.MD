# wannabe-plus-plus
Set of C11 macros and function for some C11 code that really want to be a little bit of C++.

## Requirements
* A C11 compiler 
* A compiler that supports statement expressions
* [Optional] A compiler that supports `__auto_type` or `typeof`  
  * If your compiler does not support `__auto_type` but suppports `typeof`, use `-DWPP_NO_AUTO_TYPE`.
  * If your compiler does not support either `__auto_type` or `typeof` or you do not want the `for_each`-like utilities, use `-DWPP_NO_FOR_LOOP_UTILS`.

Tested with:
 * gcc 4.9 and onwards
 * clang 3.8 and onwards
 * icc 16.0.4 and onwards with `-DWPP_NO_AUTO_TYPE`

`wpp.h` does not require `wpp_test.h`, but the latter requires the former.

## `wpp.h`
The main core of this project. A header-only libary which helps achieving number of arguments-based overload via C11 `_Generic` selection.

A couple of `for_each`-like utilites are given using `typeof` or `__auto_type`. They can be disabled by defining `WPP_NO_FOR_LOOP_UTILS` (`make test -DWPP_NO_FOR_LOOP_UTILS`) or removing them manually.

## `wpp_test.h`

A header-only test framework.

It provides check/assert macros with failure messages and test cases via function-defining macros.

### `CHECK/ASSERT_*(...)`



## How to include it in my project

Just download [wpp.h](https://raw.githubusercontent.com/slaierno/wannabe-plus-plus/master/include/wpp.h) and include it in you project.

If you want its related test environment, download and include [wpp_test.h](https://raw.githubusercontent.com/slaierno/wannabe-plus-plus/master/include/wpp_test.h).
