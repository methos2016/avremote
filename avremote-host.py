#!/usr/bin/python

import sys
import serial
import alsaaudio
from Xlib import display, X
from Xlib.ext.xtest import fake_input
import subprocess
import time

SERIAL_PATH = "/dev/ttyUSB0"

def main(argv):
    try:
        s = serial.Serial(port = SERIAL_PATH)
    except:
        print 'exception opening serial port'
        exit(0)

    # for audio control
    m = alsaaudio.Mixer()

    while True:
        try:
            cmd = long(s.readline()) & 0x0000FFFF

            if cmd == 254:
                try:
                    print '[254] volume increase'
                    m.setvolume(int(m.getvolume()[0] + 5))
                except:
                    pass	

            elif cmd == 8414:
                try:
                    print '[8414] volume decrease'
                    m.setvolume(int(m.getvolume()[0] - 5))
                except:
                    pass

            elif cmd == 24734:
                print '[24734] netflix continue'
                
                d = display.Display()
                screen  = d.screen()
                root_screen = screen.root
                root_screen.warp_pointer(831, 460)
                d.sync()
                fake_input(d, X.ButtonPress, 1)
                d.sync()
                fake_input(d, X.ButtonRelease, 1)
                d.sync()
                fake_input(d, X.ButtonPress, 1)
                d.sync()
                fake_input(d, X.ButtonRelease, 1)
                d.sync()
                
            # netflix pause button
            elif cmd == 24990:
                print '[24990] netflix pause/resume'
                
                d = display.Display()
                screen  = d.screen()
                
                # move cursor to position of pause/resume button
                root_screen = screen.root
                root_screen.warp_pointer(199, 896)
                d.sync()
                
                # click twice, once for focus and once for actual pause/resume
                fake_input(d, X.ButtonPress, 1)
                d.sync()
                fake_input(d, X.ButtonRelease, 1)
                d.sync()
                root_screen.warp_pointer(0, 0)
                d.sync()
                
                # move cursor off screen
                root_screen.warp_pointer(9999, 0)
                d.sync()
                fake_input(d, X.ButtonPress, 1)
                d.sync()
                fake_input(d, X.ButtonRelease, 1)
                d.sync()
                
            elif cmd == 510:
                print '[510] sleep displays'
                subprocess.call('xset dpms force off'.split())
                p = subprocess.Popen('gnome-screensaver-command -i'.split())
                time.sleep(1)
                print display.next_event()
                p.terminate()


        except Exception, e:
            print 'exception while reading data from serial port'

if __name__ == "__main__":
    main(sys.argv)

