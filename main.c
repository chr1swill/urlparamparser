#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

enum url_protocol {
  url_protocol_unknown = 0x0,
  url_protocol_insecure = 0x7, //protocol len
  url_protocol_secure = 0x8, // protocol len
};

static inline
enum url_protocol check_protocol(const unsigned char *url, size_t len)
{
  assert(len > 8);

  if (memcmp(url, "http://", 7) == 0)
    return url_protocol_insecure;
  else if (memcmp(url, "https://", 8) == 0)
    return url_protocol_secure;
  else
    return url_protocol_unknown;
}

struct url_query_param {
  const unsigned char *key;
  const unsigned char *value;
  size_t keylen;
  size_t valuelen;
};

enum url_token {
  url_token_question_mark = '?',
  url_token_and = '&',
  url_token_equal = '=',
  url_token_quote = '"',
};

/**
 * @return -1 in case of error or 0 if all good
 */
static inline
int consume_query_param(
    struct url_query_param *query_param,
    const unsigned char *url,
    size_t urllen,
    size_t *idx)
{
  assert(query_param->keylen == 0);
  assert(query_param->valuelen == 0);
  assert(url[*idx] == url_token_and ||
      url[*idx] == url_token_question_mark);

  ++(*idx);
  if (*idx == urllen) return -1;
  assert(isalpha(url[*idx]));

  switch (url[*idx])
  {
    case url_token_question_mark:
    case url_token_and:
    case url_token_equal:
    case url_token_quote: return -1;
    default: break;
  }

  if (*idx == urllen) return -1;

  query_param->key = &url[*idx];
  while (*idx < urllen)
  {
    switch (url[*idx])
    {
      case url_token_question_mark:
      case url_token_quote:
      case url_token_and: return -1;
      case url_token_equal: goto found_equal;
    }
    ++(*idx);
    ++query_param->keylen;
  }

found_equal:
  assert(url[*idx] == url_token_equal);
  if (*idx == urllen) return -1;

  ++(*idx);
  if (*idx == urllen) return -1;

  if (*idx == url_token_quote)
  {
    ++(*idx);
    if (*idx == urllen) return -1;
    query_param->value = &url[*idx];
    while (*idx < urllen)
   {
      if (url[*idx] == url_token_quote)
        goto found_end_quote;

      ++(*idx);
      ++query_param->valuelen;
    }

found_end_quote:
    assert(url[*idx] != url_token_quote);
    if (*idx < urllen)
      ++(*idx);
    
    switch (url[*idx])
    {
      case url_token_quote: // we didn't increment *idx
      case url_token_and:
      case url_token_question_mark:
        return 0;
      default:
        return -1;
    }
  }
  else
  {
    if (*idx == urllen) return -1;
    assert(isalnum(url[*idx]));

    // look parse to ? &
    query_param->value = &url[*idx];
    while (*idx < urllen)
    {
      switch (url[*idx])
      {
        case url_token_and:
        case url_token_question_mark: return 0;
        case url_token_quote:
        case url_token_equal: return -1;
      }
      ++(*idx);
      ++query_param->valuelen;
    }

    return 0;
  }
}

/**
 * @note THIS FUNCTION DOES NOT ALLOCATE 
 *
 * @param url - should be at the index and the start of the domain,
 *   directly after the protocol
 * @param urllen - (original len of the url) - (the len of the protocol)
 * @param query_params - should be 0 initialized
 * @param n_query_params - the number of filled query params in array
 * @param query_params_max - the capacity of the query params array
 *
 * @return -1 means error 0 means success 
 */
static inline
int parse_query_params(
    const unsigned char *url,
    size_t urllen,
    struct url_query_param query_params[],
    size_t *n_query_params,
    const size_t query_params_max) 
{
  assert(urllen > 0);
  assert(query_params_max > 0);
  assert(query_params_max > *n_query_params);

  size_t i;

  i = 0;
  while ( i < urllen &&
      *n_query_params < query_params_max )
  {
    switch(url[i])
    {
      case url_token_question_mark:
      case url_token_and:
        if (consume_query_param(
              &query_params[*n_query_params],
              url, urllen, &i) == 0) 
          ++(*n_query_params);
        else
          return -1;
        break;
      default: ++i;
    }
  }

  return 0;
}

int main(void)
{
  // does to work for quoted values just yet, whomp, whomp
  // const unsigned char url[] =
  //   "http://localhost:8080/login?username=chris&password=password123&water=melon&yourdadis=\"bald!\"";
  const unsigned char url[] =
    "http://localhost:8080/login?username=chris&password=password123&water=melon&yourdadis=bald";
  size_t i, n_query_params, urllen = (sizeof url) - 1; // those good only invisible '\0', classic
  struct url_query_param query_params[4] = {0};
  enum url_protocol protocol;
  int rc;

  protocol = check_protocol(url, urllen);
  n_query_params = 0;

  rc = parse_query_params(
      &url[protocol],
      (urllen - protocol),
      query_params,
      &n_query_params,
      sizeof query_params / sizeof query_params[0]);

  // printf("n_query_params=%zu\n", n_query_params);

  i = 0;
  for ( ; i < n_query_params; ++i)
  {
    write(STDOUT_FILENO, query_params[i].key, query_params[i].keylen);
    putchar('\n');
    fflush(stdout);
    write(STDOUT_FILENO, query_params[i].value, query_params[i].valuelen);
    putchar('\n');
    putchar('\n');
    fflush(stdout);
  }

  return 0;
}
