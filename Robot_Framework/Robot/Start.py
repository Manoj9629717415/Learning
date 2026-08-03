import threading
import time
from time import sleep
import openpyxl

import keyboard
import pandas as pd
import logging

logging.basicConfig(level=logging.INFO)

user_lock = threading.Lock()
start_event1 = threading.Event()
start_event2 = threading.Event()
stop_event = threading.Event()
common_variable = 0

class singleton:
    instance_ = None
    counter = 0

    def __new__(cls):
        if cls.instance_ is None:
            cls.instance_ = super().__new__(cls)
        return cls.instance_

    def increment_counter(self):
        self.counter += 1

    def get_counter(self):
        return self.counter



def thread_1():
    while not stop_event.is_set() :
        start_event1.wait()

        if stop_event.is_set():
            break

        with user_lock:
            ins = singleton()
            ins.increment_counter()
            print(f"thread1 variable {ins.get_counter()}")
        sleep(0.5)
        start_event1.clear()
        start_event2.set()


def thread_2():
    while not stop_event.is_set():
        start_event2.wait()
        if stop_event.is_set():
            break

        with user_lock:
            ins = singleton()
            ins.increment_counter()
            print(f"thread2 variable {ins.get_counter()}")
        sleep(0.5)
        start_event2.clear()
        start_event1.set()


def start_process():
    th1 = threading.Thread(target=thread_1)
    th2 = threading.Thread(target=thread_2)
    th1.start()
    th2.start()
    start_event1.set()

    while not keyboard.is_pressed('q'):
        sleep(0.5)

    stop_event.set()
    # Wake any waiting threads
    start_event1.set()
    start_event2.set()

    th1.join()
    th2.join()

#start_process()

def execution_time(func):
    def wrapper(*args,**kwargs):
        start = time.perf_counter()
        result = func(*args,**kwargs)
        end = time.perf_counter()
        print(f"time taken {end - start}")
        return result
    return wrapper

def read_log_file(file_path):
    with open(file_path) as f:
        for line in f:
            if "intellij" in line:
                yield line

@execution_time
def read_csv():
    file_path = r"C:\Users\AppConnect\Downloads\Self-Evaluation - In-Vehicle Test Engineer_Manoj Palanisamy.xlsx"

    try:
        df = pd.read_excel(file_path)

    # for index,rows in df.iterrows():
    #     print(f"index = {index} rows {rows}")

    #print(df["Self-Evaluation Checklist"])

        log = r"C:\Users\AppConnect\java_error_in_pycharm_22436.log"

        for line in read_log_file(log):
            logging.info(line)

    except Exception as e:
        logging.exception(e)
    finally:
        logging.info("everything closed finally")

read_csv()


