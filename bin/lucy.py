#!/usr/bin/env python3
# encoding=utf8

# The lucy executable script.
#
# Copyright (C) 2015 Wiky L
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.3, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#

import os
import sys

if __name__ != '__main__':
    sys.exit(1)


try:
    from pdb import *
    from pydroid.main import pydroid_main
except ImportError:
    sys.path.append('.')
    sys.path.append('./pdb/')
    print(sys.path)
    from pdb import *
    print(dir())
    from pydroid.main import pydroid_main

pydroid_main()
