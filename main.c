// Markus Vaikjärv
#define _GNU_SOURCE // for asprintf (implicit declaration of asprintf would
                    // otherwhise be invalid according to C99 std)

#include "jsmn.h"
#include <curl/curl.h> //sudo apt install libcurl4-gnutls-dev
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int http_get_response(void *buffer, int size, int rxed, char **wiki_response) {
  char *c;

  if (asprintf(&c, "%s%.*s", *wiki_response, size * rxed, buffer) == -1) {
    free(*wiki_response);
    wiki_response = NULL;
    return -1;
  }

  free(*wiki_response);
  *wiki_response = c;

  return size * rxed;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int main(int argc, char **argv) {
  CURL *curl;
  CURLcode res;
  char *wiki_response = calloc(1, sizeof(char));
  int i;
  int keys; //number of keys in the json root object
  jsmn_parser p;
  jsmntok_t t[128]; // successful response from the API has around 93 tokens
  char subject[128];
  char url[512];
  subject[0] =
      '\0'; // necessary when running strcat() on an empty string. By default
            // the first null terminator is on fifth index, resulting in the
            // first five characters being garbage values

  if (argc < 2) {
    goto PARAMS_MISSING;
  }

  for (i = 1; i < argc; i++) {
    strcat(subject, argv[i]); // segfaults when user supplied argument is > 512
    strcat(subject, " ");     // adding whitespace between words
  }

  sprintf(url, "https://en.wikipedia.org/api/rest_v1/page/summary/%s", subject);

  curl = curl_easy_init();

  if (curl) {

    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_get_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wiki_response);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    // parse the response
    jsmn_init(&p);
    keys = jsmn_parse(&p, wiki_response, strlen(wiki_response), t,
                   sizeof(t) / sizeof(t[0]));

    if (keys < 0) {
      printf("Failed to parse JSON: %d\n", keys);
      return 1;
    }

    // Loop over all keys of the root object
    for (i = 1; i < keys; i++) {

      if (jsoneq(wiki_response, &t[i], "title") == 0) {
        char title_buffer[128]; // creating title_buffer to check ensure that
                                // the title is not "Not found."
        sprintf(title_buffer, "%.*s", t[i + 1].end - t[i + 1].start,
                wiki_response + t[i + 1].start);

        if (strcmp(title_buffer, "Not found.") == 0) {
          goto NOT_FOUND;
        }

        printf("\n\x1B[32m%.*s\x1B[0m", t[i + 1].end - t[i + 1].start,
               wiki_response + t[i + 1].start);
        i++;
      } else if (jsoneq(wiki_response, &t[i], "description") == 0) {

        printf("\x1B[32m (%.*s) \x1B[0m\n", t[i + 1].end - t[i + 1].start,
               wiki_response + t[i + 1].start);
        i++;
      } else if (jsoneq(wiki_response, &t[i], "extract") == 0) {
        printf("\n%.*s\n\n", t[i + 1].end - t[i + 1].start,
               wiki_response + t[i + 1].start);
        i++;
      }
    }

    // resource cleanup
    curl_easy_cleanup(curl);
    free(wiki_response);
    wiki_response = NULL;
  }

  return 0;

NOT_FOUND:
  printf("\x1b[31m Could not find any information about the subject \x1b[0m\n");
  exit(EXIT_FAILURE);
PARAMS_MISSING:
  printf("\x1b[31m Missing parameter \x1b[0m\n");
  exit(EXIT_FAILURE);
}
