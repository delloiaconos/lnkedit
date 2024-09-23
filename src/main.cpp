#include "lnkedit.h"

static void usage(char *progname) {
  fprintf(stderr, "Usage: %s [-options] filename.lnk [regex [regex]]\n", progname);
  fprintf(stderr, "Notes: With two regexes, link will be retargeted.\n");
  fprintf(stderr, "Use options to specify retargetting in :\n");
  fprintf(stderr, "\t t - target field;\n" );
  fprintf(stderr, "\t a - argument fields;\n" );
  fprintf(stderr, "\t d - working directory field;\n" );
  fprintf(stderr, "\t i - icon field.\n\n" );
  fprintf(stderr, "For example:\n  %s -td filename.lnk 'C:\\Program Files' '%%programfiles%%\n\n", progname);
  fprintf(stderr, "The above example would replace C:\\Program Files with the raw environment\n");
  fprintf(stderr, "variable %%programfiles%% only in the link target and working directory fields.\n");
}

int main(int argc, char **argv) {
  regex_t regex, *re = 0;
  char *replace = 0;
  int options = OPTION_ALL;
  int ret;

  if (argc == 1) {
    usage(argv[0]);
    exit(1);
  }

  /* Check for options */
  if (*argv[1] == '-') {
    options = 0;

    for (char *s = argv[1] + 1; *s; s++) {
      switch (*s) {
        case 't':
        case 'T':
          options |= OPTION_PROG;
          break;
        case 'a':
        case 'A':
          options |= OPTION_ARGS;
          break;
        case 'd':
        case 'D':
          options |= OPTION_DIR;
          break;
        case 'i':
        case 'I':
          options |= OPTION_ICON;
          break;
        default:
          fprintf(stderr, "bogus option character '%c'\n", *s);
          usage(argv[0]);
          exit(3);
      }
    }

    for (int i = 1; i < argc - 1; i++) argv[i] = argv[i + 1];
    argc--;
  }

  /* Compile re */
  if (argc > 2) {
    //ret = regcomp(&regex, argv[2], REG_EXTENDED | REG_ICASE);
    if (ret) {
      fprintf(stderr, "Failed to parse regular expression: error code %d\n", ret);
      exit(2);
    }

    re = &regex;
    /* Check for replacement string */
    if (argc > 3) replace = argv[3];

  }

  init();

  IShellLink *link;
  IPersistFile *file;

  ret = doit(argv[1], re, replace, options);
  //if (re) {regfree(re);} 
  if (ret) {
    fprintf(stderr, "doit() returned %d\n", ret);
    cleanup();
    exit(4);
  }

  cleanup();
  exit(0);
}
