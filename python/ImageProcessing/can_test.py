# class CAN:
#     def __init__(self):
#         print (CAN)
#     def signal (self):
#
#  can = CAN DBC()
import threading
import time

import can
from can.interfaces.virtual import channels

# Create CAN bus interface (adjust to your hardware)
# bus = can.interface.Bus(
#     channel='can0',       # e.g. 'can0' on Linux
#     bustype='socketcan'   # or 'pcan', 'usb2can', etc.
# )

bus = can.interface.Bus(
    interface='virtual',
    channel='vcan0'
)

sendMsg = False

# Create a CAN message
msg = can.Message(
    arbitration_id=0x476,   # CAN ID
    data=[0x08, 0x00, 0x00, 0x00],  # up to 8 bytes (classic CAN)
    is_extended_id=False
)
#msg1 = can.Message(
#    arbitration_id=0x477,   # CAN ID
#    data=[0x08, 0x00, 0x00, 0x00],  # up to 8 bytes (classic CAN)
#    is_extended_id=False
#)
#msg2 = can.Message(
#    arbitration_id=0x231,   # CAN ID
#    data=[0x08, 0x00, 0x00, 0x00],  # up to 8 bytes (classic CAN)
#    is_extended_id=False
#)
# def sender():
#     print("sender called\n")
#     while True:
#         # Send the message
#         try:
#             bus.send(msg)
#             print("Message sent\n")
#         except can.CanError:
#             print("Message NOT sent\n")
#         time.sleep(0.5)

#import can
import time

#bus = can.Bus(interface='virtual', channel='vcan0')

#Request message (1 signal)
# msg = can.Message(
#    arbitration_id=0x476,
#    data=[0x08, 0, 0, 0, 0, 0, 0, 0],  # signal1 = 4
#    is_extended_id=False
# )

# Send request
#bus.send(msg)
#print("Request sent")

# Wait for response
#while True:
#    response = bus.recv(timeout=5)

#    if response and response.arbitration_id == 0x476:
#        sig1 = response.data[0]
#        sig2 = response.data[1]

#        print(f"Response received: Signal1={sig1}, Signal2={sig2}")
#        break

verifymsg = False
stop = False
start_time = time.time()
send_flag = threading.Event()
stop_flag = threading.Event()

def sendClamp(state=0):
    print("clamp msg send")
    bus.send(msg)

def setSendMsg(value):
    global sendMsg
    sendMsg = value

def setStop(value):
    global stop
    stop = value

def starttestapplication():
    global verifymsg,start_time,stop
    while True:
        if send_flag.is_set():
            sendClamp(1)
            verifymsg = True
            start_time = time.time()
            send_flag.clear()

        response = bus.recv(timeout=0.1)

        if response is not None and verifymsg is True and response.arbitration_id == 0x450:
           sig1 = response.data[0]
           sig2 = response.data[1]
           print(f"Response received: Signal1={sig1}, Signal2={sig2}")
           verifymsg = False

        if verifymsg is True and time.time() -start_time > 0.5 :
            print("Response not received on Time")
            verifymsg = False

        if stop_flag.is_set():
            stop_flag.clear()
            break







