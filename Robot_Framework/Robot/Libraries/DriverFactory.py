from Hardware.SimulatorDriver import SimulatorDriver


class DriverFactory:

    @staticmethod
    def get_driver(driver):
        if driver == "SIM":
            return SimulatorDriver()
        return None