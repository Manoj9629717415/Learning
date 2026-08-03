import threading
import time

import can


# Use same virtual bus name as sender
bus = can.interface.Bus(
    interface='virtual',
    channel='vcan0'   # must match sender!
)

print("Listening for messages...")

msg1 = can.Message(
    arbitration_id=0x450,   # CAN ID
    data=[0x01, 0x00, 0x00, 0x00],  # up to 8 bytes (classic CAN)
    is_extended_id=False
)

msg2 = can.Message(
    arbitration_id=0x410,   # CAN ID
    data=[0x50, 0x00, 0x00, 0x00],  # up to 8 bytes (classic CAN)
    is_extended_id=False
)

# def receiver():
#     print("thread started")
#     while True:
#         msg = bus.recv()  # wait for message
#         if msg is not None:
#             print(f"Received: ID={hex(msg.arbitration_id)} Data={msg.data}")
#         else:
#             print("No message received")
#
#
#
# t = threading.Thread(target=receiver, daemon=True).start()
# sender()

#import can

#bus = can.Bus(
#    interface='virtual',
#    channel='vcan0')

#print("Simulation running...")

#while True:
#    msg = bus.recv()

#    if msg.arbitration_id == 0x476:
#        print("Request received:", msg.data)

        # Extract incoming signal
#        signal_in = msg.data[0]

        # Create response (2 signals)
#        signal1 = signal_in + 1
#        signal2 = signal_in + 2

#        response = can.Message(
#            arbitration_id=0x476,
#            data=[signal1, signal2, 0, 0, 0, 0, 0, 0],
#            is_extended_id=False
#        )

#        bus.send(response)
#        print("Response sent")

#t = threading.Thread(target=recv, daemon=True).start()
#sender()
stop_hu_flag = threading.Event()

def sendMessage(msg):
    bus.send(msg)

def setStop_HU(value):
    global stop
    stop = value


def HUSimuationApp():
    while True:

        response = bus.recv(timeout=0.1)

        if response is not None and response.arbitration_id == 0x476:
           sig1 = response.data[0]
           sig2 = response.data[1]
           print(f"clamp msg received with {sig1}")
           if sig1 == 0x08:
               print("response msg send")
               time.sleep(2)
               sendMessage(msg1)
               sendMessage(msg2)


        if stop_hu_flag.is_set():
            stop_hu_flag.clear()
            break
