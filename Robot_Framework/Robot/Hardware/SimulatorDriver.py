from Hardware.Can_Interface import CANInterface


class SimulatorDriver(CANInterface):
    def start_measurement(self):
        self.measurement = True

    def stop_measurement(self):
        self.measurement = False

    def set_signal(self, signal, value):
        self.signals[signal] = value

    def get_signal(self, signal):
        self.signals.get(signal,None)

    def is_measurement_running(self):
        return self.measurement

    def __init__(self):
        self.measurement = False
        self.signals = {}
