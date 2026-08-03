from abc import ABC,abstractmethod


class CANInterface(ABC):
    @abstractmethod
    def start_measurement(self):
        pass

    @abstractmethod
    def stop_measurement(self):
        pass

    @abstractmethod
    def set_signal(self, signal, value):
        pass

    @abstractmethod
    def get_signal(self, signal):
        pass
