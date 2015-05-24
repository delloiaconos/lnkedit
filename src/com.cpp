#include "lnkedit.h"
#include "re.h"

int init() {
  HRESULT result = CoInitialize(0);
  if (result != S_OK) return 1;
  return 0;
}

int doit(char *filename, regex_t *re, char *replace, int options) {
  IShellLink *link;
  IPersistFile *file;
  char prog[MAX_PATH];
  char dir[MAX_PATH];
  char args[MAX_PATH];

  wchar_t widepath[MAX_PATH];
  WIN32_FIND_DATA found;
  HRESULT result;

  /* Create a link */
  result = CoCreateInstance(CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **) &link);
  if (result != S_OK) return 1;

  /* Get pointer to link's PersistFile interface */
  result = link->QueryInterface(IID_IPersistFile, (void **) &file);
  if (result != S_OK) {
    link->Release();
    return 2;
  }

  /* Convert filename to unicode */
  MultiByteToWideChar(CP_ACP, 0, filename, -1, widepath, sizeof(widepath));

  /* Load unicode path into link */
  result = file->Load(widepath, STGM_READWRITE | STGM_SHARE_DENY_NONE);
  if (result != S_OK) {
    file->Release();
    link->Release();
    return 3;
  }

  /* Get the path */
  result = link->GetPath(prog, MAX_PATH, &found, 4/*SLGP_RAWPATH*/);
  if (result != S_OK) {
    file->Release();
    link->Release();
    return 4;
  }

  /* Get the working arguments */
  result = link->GetArguments(args, MAX_PATH);
  if (result != S_OK) {
    file->Release();
    link->Release();
    return 5;
  }

  /* Get the working directory */
  result = link->GetWorkingDirectory(dir, MAX_PATH);
  if (result != S_OK) {
    file->Release();
    link->Release();
    return 6;
  }

  int need_show = 0;
  int need_write = 0;
  if (re) {
    char replaced[MAX_PATH];
    
    /* Match filename */
    if (options & OPTION_PROG) {
      do_regex(prog, re, replace, replaced, &need_show);
      if (replaced[0]) {
        result = link->SetPath(replaced);
        if (result != S_OK) {
          fprintf(stderr, "Error setting new path!\n");
          file->Release();
          link->Release();
          return 7;
        }
        need_write = 1;
        memmove(prog, replaced, strlen(replaced));
        prog[strlen(replaced)] = '\0';
      }
    }

    /* Match arguments */
    if (options & OPTION_ARGS) {
      do_regex(args, re, replace, replaced, &need_show);
      if (replaced[0]) {
        result = link->SetArguments(replaced);
        if (result != S_OK) {
          fprintf(stderr, "Error setting new arguments!\n");
          file->Release();
          link->Release();
          return 8;
        }
        need_write = 1;
        memmove(args, replaced, strlen(replaced));
        args[strlen(replaced)] = '\0';
      }
    }

    /* Match dir */
    if (options & OPTION_DIR) {
      do_regex(dir, re, replace, replaced, &need_show);
      if (replaced[0]) {
        result = link->SetWorkingDirectory(replaced);
        if (result != S_OK) {
          fprintf(stderr, "Error setting new directory!\n");
          file->Release();
          link->Release();
          return 9;
        }
        need_write = 1;
        memmove(dir, replaced, strlen(replaced));
        dir[strlen(replaced)] = '\0';
      }
    }
  }
  else need_show = 1;

  /* Display */
  printf(" Shortcut: %s", filename);
  if (need_show) {
    printf("\n");
    printf("   Target: %s\n", prog);
    printf("Arguments: %s\n", args);
    printf(" Start in: %s\n", dir);
  }
  else printf(" (no match)\n");

  /* Update the link */
  if (need_write) {
    result = file->Save(widepath, 1);
    if (result != S_OK) {
      fprintf(stderr, "Error saving shortcut!\n");
      file->Release();
      link->Release();
      return 10;
    }
  }

  /* Cleanup */
  file->Release();
  link->Release();

  return 0;
}

void cleanup() {
  CoUninitialize();
}
