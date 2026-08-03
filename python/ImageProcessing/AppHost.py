import msvcrt
import threading
import time

from can_receiver import HUSimuationApp, setStop_HU, stop_hu_flag
from can_test import starttestapplication, setSendMsg, setStop, send_flag, stop_flag

if __name__ == "__main__":

    testApp = threading.Thread(target=starttestapplication,daemon=True)

    huapp = threading.Thread(target=HUSimuationApp,daemon=True)

    testApp.start()

    huapp.start()

    while True:
        time.sleep(3)
        send_flag.set()

        if msvcrt.kbhit():
            key = msvcrt.getch()

            if key == b'q':
                stop_flag.set()
                stop_hu_flag.set()
                break
            if key == b's':
                print("key pressed S")
                send_flag.set()
