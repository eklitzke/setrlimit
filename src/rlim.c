// Copyright Evan Klitzke <evan@eklitzke.org>, 2016
//
// This file is part of setrlimit.
//
// Setrlimit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Setrlimit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Setrlimit.  If not, see <http://www.gnu.org/licenses/>.

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <sys/types.h>

#include "./ulog.h"

static const char *tbl[] = {"CPU",   "FSIZE",   "DATA",  "STACK",
                            "CORE",  "RSS",     "NPROC", "BOFILE",
                            "OFILE", "MEMLOCK", "AS"};
const size_t kNrLimit = 10;

int rlimit_by_name(const char *name) {
  ulog_info("in rlimit_by_name with name = %p %s", name, name);
  if (name == NULL || name[0] == '\0') {
    return -1;
  }

  const size_t len = strlen(name);
  char *upper_name = malloc(len + 1);
  upper_name[len + 1] = '\0';
  for (size_t i = 0; i < len; i++) {
    upper_name[i] = toupper(name[i]);
  }
  for (size_t i = 0; i < kNrLimit; i++) {
    if (strcmp(upper_name, tbl[i]) == 0) {
      if (strcmp(upper_name, tbl[i]) == 0) {
        free(upper_name);
        return i;
      }
    }
  }

  free(upper_name);
  return -1;
}

void read_rlimit(pid_t pid, unsigned long where, struct rlimit *rlim) {
  const size_t sz = sizeof(struct rlimit) / sizeof(long);
  for (size_t i = 0; i < sz; i++) {
    errno = 0;
    long word = ptrace(PTRACE_PEEKTEXT, pid, where + i * sizeof(long), 0);
    ulog_info("tried to peek for pid %d at %p", pid,
              (void *)(where + i * sizeof(long)));
    if (word == -1 && errno) {
      perror("ptrace(PTRACE_PEEKTEXT, ...)");
      exit(1);
    }
    ulog_info("poking data to %p", (long *)(rlim) + i);
    memcpy((long *)(rlim) + i, &word, sizeof(word));
  }
}

void poke_rlimit(pid_t pid, unsigned long where, struct rlimit *rlim) {
  const size_t sz = sizeof(struct rlimit) / sizeof(long);
  for (size_t i = 0; i < sz; i++) {
    long word;
    memcpy(&word, (long *)(rlim) + i, sizeof(word));
    ulog_info("tried to poke %ld at %p", word, where + i * sizeof(long));
    if (ptrace(PTRACE_POKETEXT, pid, where + i * sizeof(long), word)) {
      perror("ptrace(PTRACE_POKETEXT...)");
    }
  }
}
