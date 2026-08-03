import json
import queue
import threading
import time
from abc import ABC, abstractmethod
import logging
from time import sleep
from concurrent.futures import ThreadPoolExecutor


logging.basicConfig(level=logging.INFO)



class logger(ABC):
    @abstractmethod
    def capture_log(self):
        pass

    @abstractmethod
    def stop_capture(self):
        pass

stop_event = threading.Event()
shared_queue = queue.Queue()

class CANLogger(logger):
    def __init__(self,interface):
        self.can_interface = interface
        self.count = 0

    def capture_log(self):
        while not stop_event.is_set():
            time.sleep(0.05)
            shared_queue.put("CAN")
            self.count += 1
            logging.info(f"CAN received from {self.can_interface}")

        self.stop_capture()

    def stop_capture(self):
        logging.info(f"CAN logging stopped for {self.can_interface}")


class EthernetLogger(logger):
    def __init__(self, interface):
        self.eth_interface = interface
        self.count = 0

    def capture_log(self):
        while not stop_event.is_set():
            sleep(0.05)
            shared_queue.put("Ethernet")
            self.count += 1
            logging.info(f"Ethernet received from {self.eth_interface}")
        self.stop_capture()

    def stop_capture(self):
        logging.info(f"Ethernet logging stopped for {self.eth_interface}")

class ADBLogger(logger):
    def __init__(self, interface):
        self.adb_interface = interface
        self.count = 0

    def capture_log(self):
        while not stop_event.is_set():
            sleep(0.05)
            shared_queue.put("ADB")
            self.count += 1
            logging.info(f"ADB received from {self.adb_interface}")
        self.stop_capture()

    def stop_capture(self):
        logging.info(f"ADB logging stopped for {self.adb_interface}")


class DatabaseHandler:
    def __init__(self):
        self.table_created = True

    def insert_db(self):
        while True:
            sleep(0.03)
            data = shared_queue.get()
            logging.info(f"Data sent DB {data}")
            if data is None:
                break


class ConfigHandler:
    instance_ = None
    is_can_logger_enabled = False
    is_eth_logger_enabled = False
    is_adb_logger_enabled = False
    can_interface = "can0"
    eth_interface = "eth0"
    adb_interface = "adb"

    def __new__(cls):
        if cls.instance_ is None:
            cls.instance_ = super().__new__(cls)
        return cls.instance_

    def read_config(self):
        try:
            with open("default.json",'r') as f:
                config = json.load(f)
                self.process_config(config)
        except FileNotFoundError:
            logging.error("default.json File not found")
        except json.JSONDecodeError as e:
            logging.error("json deocde error",e)
        except Exception as e:
            logging.error(e)

    def process_config(self,config):
        if config["default"]["can_logger"]:
            self.is_can_logger_enabled = True
            self.can_interface = config["default"]["can_interface"]

        if config["default"]["eth_logger"]:
            self.is_eth_logger_enabled = True
            self.eth_interface = config["default"]["eth_interface"]

        if config["default"]["adb_logger"]:
            self.is_adb_logger_enabled = True
            self.adb_interface = config["default"]["adb_interface"]


class LoggerFactory:
    @staticmethod
    def create_logger(trace_logger,interface):
        if trace_logger == "CAN":
            return CANLogger(interface)
        elif trace_logger == "ETH":
            return EthernetLogger(interface)
        elif trace_logger == "ADB":
            return ADBLogger(interface)



def test_manager():
    ConfigHandler().read_config()

    with ThreadPoolExecutor(max_workers=4) as executor:
        if ConfigHandler.is_eth_logger_enabled:
            executor.submit(LoggerFactory.create_logger(trace_logger="ETH",interface=ConfigHandler.eth_interface).capture_log)

        if ConfigHandler.is_can_logger_enabled:
            executor.submit(LoggerFactory.create_logger("CAN", ConfigHandler.can_interface).capture_log)

        if ConfigHandler.is_adb_logger_enabled:
            executor.submit(LoggerFactory.create_logger("ADB", ConfigHandler.adb_interface).capture_log)

        executor.submit(DatabaseHandler().insert_db)

        sleep(10)

        stop_event.set()

        shared_queue.put(None)

        executor.shutdown(wait=True)

    # data_loggers = [CANLogger("can0"),EthernetLogger("eth0"),ADBLogger("adb")]
    # threads = []
    # for data_logger in data_loggers:
    #     th = threading.Thread(target=data_logger.capture_log)
    #     th.start()
    #     threads.append(th)
    #
    # db_thread = threading.Thread(target=DatabaseHandler().insert_db)
    # db_thread.start()



    # for th in threads:
    #     th.join()
    #
    # db_thread.join()

test_manager()
