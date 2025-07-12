#include <assert.h>
#include <stdio.h>
#define URLPARAMPARSER_IMPLEMENTATION
#include "urlparamparser.h"

int main()
{
  ssize_t idx_or_not;
  struct url_query_param qp[2];

  qp[0].key = "username";
  qp[0].keylen = (sizeof "username") - 1;
  qp[0].value = "whatisyourname123";
  qp[0].valuelen = (sizeof "whatisyourname123") - 1;

  qp[1].key = "password";
  qp[1].keylen = (sizeof "password") -1;
  qp[1].value = "supersecure";
  qp[1].valuelen = (sizeof "supersecure") -1;

  idx_or_not = url_query_param_get_value(
      qp, 2,"username", (sizeof "username") - 1);
  assert( idx_or_not == 0 );
  puts("test 1 passed");

  idx_or_not = url_query_param_get_value(
      qp, 2,"password", (sizeof "password") - 1);
  assert( idx_or_not == 1 );
  puts("test 2 passed");

  puts("gReAt SuCcEsS");
  return 0;
}
