# encoding=utf8
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

import gi
gi.require_version('Gtk', '3.0')

from pyadb import pdb_init
from pydroid.mainwindow import MainWindow
from pydroid.log import *


def pydroid_main():
    if pdb_init():
        D('pdb init!')
    else:
        E('fail to init pdb!')

    win = MainWindow()
    win.run()
