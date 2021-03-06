/*
 * Copyright (C) 2004 The X.Org fundation
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is fur-
 * nished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the X.Org fundation
 * shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written
 * authorization from the X.Org fundation.
 */

/*
** This is a secure but NOT 100% compatible replacement for popen()
** Note:        - don't use pclose() use fclose() for closing the returned
**                filedesc.!!!
**
** Known Bugs:  - unable to use i/o-redirection like > or <
** Author:      - Thomas Biege <thomas@suse.de>
** Credits:     - Andreas Pfaller <a.pfaller@pop.gun.de> for fixing a SEGV when
**                calling strtok()
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define __SEC_POPEN_TOKEN " "

FILE *s_popen(char *cmd, const char *type)
{
  pid_t pid;
  int pfd[2];
  int rpipe = 0, wpipe = 0, i;
  char **argv;
  char *ptr;
  char *cmdcpy;


  if(cmd == NULL || cmd == "")
    return(NULL);

  if(type[0] != 'r' && type[0] != 'w')
    return(NULL);

  if ((cmdcpy = strdup(cmd)) == NULL)
    return(NULL);

  argv = NULL;
  if( (ptr = strtok(cmdcpy, __SEC_POPEN_TOKEN)) == NULL)
  {
    free(cmdcpy);
    return(NULL);
  }

  for(i = 0;; i++)
  {
    if( ( argv = (char **) realloc(argv, (i+1) * sizeof(char *)) ) == NULL)
    {
      free(cmdcpy);
      return(NULL);
    }

    if( (*(argv+i) = (char *) malloc((strlen(ptr)+1) * sizeof(char))) == NULL)
    {
      free(cmdcpy);
      return(NULL);
    }

    strcpy(argv[i], ptr);

    if( (ptr = strtok(NULL, __SEC_POPEN_TOKEN)) == NULL)
    {
      if( ( argv = (char **) realloc(argv, (i+2) * sizeof(char *))) == NULL)
      {
        free(cmdcpy);
        return(NULL);
      }
      argv[i+1] = NULL;
      break;
    }
  }


  if(type[0] == 'r')
    rpipe = 1;
  else
    wpipe = 1;

  if (pipe(pfd) < 0)
  {
    free(cmdcpy);
    return(NULL);
  }

	if((pid = fork()) < 0)
  {
    close(pfd[0]);
    close(pfd[1]);
    free(cmdcpy);
    return(NULL);
  }

	if(pid == 0)    /* child */
  {
    if((pid = fork()) < 0)
    {
      close(pfd[0]);
      close(pfd[1]);
      free(cmdcpy);
      return(NULL);
    }
    if(pid > 0)
    {
      exit(0);  /* child nr. 1 exits */
    }

    /* child nr. 2 */
    if(rpipe)
    {
      close(pfd[0]);  /* close reading end, we don't need it */
      dup2(STDOUT_FILENO, STDERR_FILENO);
      if (pfd[1] != STDOUT_FILENO)
        dup2(pfd[1], STDOUT_FILENO);  /* redirect stdout to writing end of pipe */
    }
    else
    {
      close(pfd[1]);  /* close writing end, we don't need it */
      if (pfd[0] != STDIN_FILENO)
        dup2(pfd[0], STDIN_FILENO);    /* redirect stdin to reading end of pipe */
	  }

    if(strchr(argv[0], '/') == NULL)
      execvp(argv[0], argv);  /* search in $PATH */
    else
      execv(argv[0], argv);

    close(pfd[0]);
    close(pfd[1]);
    free(cmdcpy);
    return(NULL);  /* exec failed.. ooops! */
  }
  else          /* parent */
  {
    waitpid(pid, NULL, 0); /* wait for child nr. 1 */

    if(rpipe)
    {
      close(pfd[1]);
      free(cmdcpy);
      return(fdopen(pfd[0], "r"));
    }
    else
    {
      close(pfd[0]);
      free(cmdcpy);
      return(fdopen(pfd[1], "w"));
    }

  }
}

