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


from gi.repository import Gtk


class MainWindow(Gtk.Window):
    """主界面"""
    def __init__(self, width=640, height=480):
        super(MainWindow, self).__init__(title='Android Device Manager')
        self.set_default_size(width, height)
        self.set_position(Gtk.WindowPosition.CENTER)
        self.connect('destroy', Gtk.main_quit)

    def run(self):
        """启动主界面"""
        self.show_all()
        Gtk.main()
