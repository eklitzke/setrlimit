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

#pragma once

#include <stddef.h>
#include <sys/types.h>

struct pids {
  pid_t *pids;
  size_t sz;
  size_t cap;
};

struct pids *pids_new(pid_t head);
void pids_push(struct pids *pids, pid_t value);
pid_t pids_pop(struct pids *pids);
void pids_delete(struct pids *pids);

// add all descedants
void add_processes_recursively(struct pids *pids);
