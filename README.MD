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

### `CHECK` and `ASSERT` macros

The macros follow the following EBNF, where:
* `c_string` represents any valid C string literal
* `c_expr` represents any valid C expression
* `c_num` represents any valid C numeric literal

```EBNF
action(type) = ("CHECK" | "ASSERT"), type, ["_FAIL"];

cmp_action      = action();
exp_action      = action("_EXPR");
cmp_action_size = action(("_MEM"|"_ARRAY"));
exp_check      = stmt_action    , "(", c_expr,                          [",", c_string], ")";
cmp_check      = expr_action    , "(", c_expr, ",", c_expr,             [",", c_string], ")";
cmp_check_size = cmp_action_size, "(", c_expr, ",", c_expr, ",", c_num, [",", c_string], ")";
```

Examples from which every macro can be derived:

| Macro | Description |
|-------|-------------|
| `CHECK_EXPR(expr)` | Check that `expr` is not `0` |
| `CHECK(exp, act)` | Check that `exp == act` is `true` |
| `CHECK_MEM(exp, act, size)` | Check that `memcmp(exp, act, size)` returns `0`  |
| `CHECK_ARRAY(exp, act, cnt)` | Check that `memcmp(exp, act, sizeof(*exp)*cnt)` returns `0`  |

* `CHECK` and `ASSERT` version of a macro gives, respectively, non-blocking and blocking checks.  
* `_FAIL` versions are expected to fail their condition.
* Every macro can accept an optional failing message that will be showed in addition to default failing message
* `CHECK_MEM` and `CHECK_ARRAY` casts the pointer/arrays to `void*`
* `CHECK` selects the type to be checked looking at its first argument and calling a function that accepts two arguments of this type. E.g.  
  ```C
  int a = 1; float b = 1.0;
  CHECK(a, b);
  CHECK(b, a);
  ```
  will call, respectively, two functions like:  
  ```C
  bool check_int(int a, int b) { return a == b; }
  bool check_float(float a, float b) { return abs(a - b) < EPS; }
  ```
  **BEWARE**: this means that:
  ```C
  CHECK_C(1  , 1.9); // call check_int -> 1.9 gets truncated to 1 -> PASS!
  CHECK_C(1.9, 1); // call check_float -> 1 is casted to float    -> FAIL!
  ```
  TODO: this behaviour needs to change!


## How do I include it in my project

Just download [wpp.h](https://raw.githubusercontent.com/slaierno/wannabe-plus-plus/master/include/wpp.h) and include it in you project.

If you want its related test environment, download and include [wpp_test.h](https://raw.githubusercontent.com/slaierno/wannabe-plus-plus/master/include/wpp_test.h).
