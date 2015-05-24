#ifndef COM_H
#define COM_H

#include <objbase.h>
#include <shlobj.h>
#include <objidl.h>
#include <regex.h>

int init();
int doit(char *, regex_t *, char *, int);
void cleanup();

#endif
