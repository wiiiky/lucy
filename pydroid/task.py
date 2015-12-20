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

from gi.repository import GLib
from pyadb import *
import threading


class TaskThread(threading.Thread):
    """在线程中执行特定任务，然后使用idle_add在主线程中回调"""

    def __init__(self, callback, user_data):
        self.callback = callback
        self.user_data = user_data
        super(TaskThread, self).__init__()

    def run(self):
        GLib.idle_add(self._idle_callback, None)

    def _idle_callback(self, data):
        """回调函数会在主线程回调"""
        self.callback(self.result, self.user_data)
        return False


class StartLilyThread(TaskThread):
    """启动LILY"""

    def run(self):
        self.result = pdb_start_activity('org.wiky.lily')
        super(StartLilyThread, self).run()


def start_lily(cb, data=None):
    """启动lily"""
    StartLilyThread(cb, data).start()
