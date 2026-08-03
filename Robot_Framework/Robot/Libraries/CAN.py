
from DriverFactory import DriverFactory
from Variables.config import CAN_DRIVER


class CAN:
    def __init__(self):
        self.driver = DriverFactory.get_driver(CAN_DRIVER)

    def start_measurement(self):
        self.driver.start_measurement()
        return "Measurement Started"

    def stop_measurement(self):
        self.driver.stop_measurement()
        return "Measurement Stopped"

    def is_measurement_running(self):
        return self.driver.is_measurement_running()

    def set_signal(self, signal, value):
        self.set_signal(signal,value)

    def get_signal(self, signal):
        return self.driver.get_signal(signal)

    # def accelerate(self, increment):
    #     self.signals["VehicleSpeed"] += increment
    #     return self.signals["VehicleSpeed"]
    #
    # def apply_brake(self):
    #     self.signals["Brake"] = True
    #     self.signals["VehicleSpeed"] = 0