#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define URLPARAMPARSER_IMPLEMENTATION
#include "../urlparamparser.h"

int main(void)
{
  // does to work for quoted values just yet, whomp, whomp
  // eg:
  // const unsigned char url[] =
  //   "http://localhost:8080/login?username=chris&password=password123&water=melon&yourdadis=\"bald!\"";
  //   or
  // const unsigned char url[] =
  //   "http://localhost:8080/login?username=chris&password=password123&water=melon&yourdadis='bald!'";
  const unsigned char url[] =
    "http://localhost:8080/login?username=chris&password=password123&water=melon&yourdadis=bald";
  size_t i, n_query_params, urllen = (sizeof url) - 1; // those good only invisible '\0', classic
  struct url_query_param query_params[4] = {0};
  enum url_protocol protocol;
  int rc;

  check_protocol(url, urllen, &protocol);
  assert(protocol != url_protocol_unknown);

  n_query_params = 0;
  rc = parse_query_params(
      &url[protocol],
      (urllen - protocol),
      query_params,
      &n_query_params,
      sizeof query_params / sizeof query_params[0]);

  i = 0;
  for ( ; i < n_query_params; ++i)
  {
    write(STDOUT_FILENO,
        query_params[i].key, query_params[i].keylen);
    putchar('\n');
    fflush(stdout);
    write(STDOUT_FILENO,
        query_params[i].value, query_params[i].valuelen);
    putchar('\n');
    putchar('\n');
    fflush(stdout);
  }

  return 0;
}
