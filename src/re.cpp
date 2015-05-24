#include "lnkedit.h"

int do_regex(char *path, regex_t *re, char *replace, char *replaced, int *need_show) {
  int ret;
  size_t num_matches = 16;
  regmatch_t *matches = new regmatch_t[num_matches];

  replaced[0] = '\0';
  ret = regexec(re, path, num_matches, matches, 0);

  /* Not found? */
  if (ret == REG_NOMATCH) {
    delete[] matches;
    return 0;
  }

  /* Error */
  if (ret) {
    delete[] matches;
    return 1;
  }

  /* Matched so we'd better show it */
  *need_show = 1;

  /* Replacement string? */
  if (! replace) {
    delete[] matches;
    return 0;
  }

  /* Now let's replace */
  size_t i = 0;
  regmatch_t *match;
  do {
    match = &matches[i];
    if (match->rm_so == -1) break;

    int pathlen = strlen(path);
    int replacelen = strlen(replace);

    size_t j;
    for (j = 0; j < match->rm_so; j++) replaced[j] = path[j];

    size_t k = replacelen + j;

    /* Check for overrun of replaced */
    if (k + pathlen - match->rm_eo >= MAX_PATH) {
      fprintf(stderr, "Replacement buffer would overrun!\n");
      delete[] matches;
      return 2;
    }

    memmove(replaced + j, replace, replacelen);
    for (j = match->rm_eo; j < pathlen; j++) replaced[k++] = path[j];
    replaced[k] = '\0';

  } while (++i < num_matches);

  delete[] matches;
  return 0;
}
