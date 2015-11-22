/*
 * Copyright (C) 2015 Wiky L
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */


#include "util.h"

#if defined(HAVE_SYS_PRCTL_H)
#include <sys/prctl.h>
#include <linux/prctl.h>
#endif


void setproctitle (const char *title) {
#if defined(HAVE_SYS_PRCTL_H) && defined(PR_SET_NAME)
    prctl(PR_SET_NAME, title);
#endif
}
