from Variables.config import DEFAULT_SESSION


class Diagnostics:
    def __init__(self):
        self.session = DEFAULT_SESSION
        self.dtcs = [ "P1000",
            "U0100"]
        self.SW = "X316"

    def change_session(self,session):
        self.session = session

    def get_session(self):
        return self.session

    def get_dtc(self):
        return self.dtcs.copy()

    def clear_dtc(self):
        self.dtcs.clear()
        return "DTC Cleared"

    def add_dtc(self,dtc):
        self.dtcs.append(dtc)

    def get_sw_version(self):
        return self.SW
