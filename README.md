# Url Param Parser

Super simple and lightweight http + https header only url query param parser. Inspired by the [picohttpparser library](https://github.com/h2o/picohttpparser) functions explictly do not allocate any memory on the heap.

## Usage

To have access to the function defintion #include the header as follows:
```c
#define URLPARAMPARSER_IMPLEMENTATION
#include "urlparamparser"
```

If only the definition are needed:
```c
#include "urlparamparser"
```

## Notice

Currently only works for unquoted parameter value for example:
```c
const unsigned char *url =
    "https://example.com/login/?search=fastest+car+in+the+world"
```

Will be into a struct perfectly fine but:

```c
const unsigned char *url =
    "https://example.com/login/?search=\"fastest+car+in+the+world\""
// or this
const unsigned char *url =
    "https://example.com/login/?search='fastest+car+in+the+world'"
```

Will cause an assertion to fail, still need to fix that. For now, no quotes in your params values.
