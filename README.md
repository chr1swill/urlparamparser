# Url Param Parser

Super simple and lightweight http + https url query param parser. Inspired my the picohttpparser library so no function allocate, library users provide the memory which we set pointer to.

## Notice

Currently only works for unquoted parameter value for example:
```c
const unsigned char *url =
    "https://example.com/login/?donot=dothis&username=myname&password=password1234"
```

Will be into a struct perfectly fine but:

```c
const unsigned char *url =
    "https://example.com/login/?yoursmarter=thanthis&username=\"myname\"&password=\"password1234\""
```

Will cause an assertion to fail, still need to fix that. For now, no quotes in your params values.
