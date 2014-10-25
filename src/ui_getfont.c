#include <stdlib.h>
#include <stdio.h>
#include "ui_getfont.h"

#ifndef MAXLINE
#define MAXLINE 2048
#endif

void getfont(gchar *path) 
{
  FILE *fp;
  int status;

  /* open the command for reading*/
  fp = popen("gsettings get org.gnome.desktop.interface font-name", "r");
  if (fp == NULL) {
    exit(-1);
  }

  /* Read the output a line at a time - output it. */
  while (!feof(fp)) {
    fgets(path, MAXLINE, fp);
    path[MAXLINE] = '\n';
  }

  /* close */
  pclose(fp);
}
