class ECU:
    def __init__(self):
        self.version = "X316"
        self.signals = {"speed": 100,
                        "rpm": 5000,
                        "battery_voltage": 12.4,
                        "clamp_status":"KL_15 OFF",
                        "engine_temp":95
                        }

    def get_version(self):
        return self.version

    def toggle_clamp(self):
        if self.signals["clamp_status"] == "KL_15 OFF":
            self.signals["clamp_status"] = "KL_15 ON"
        else:
            self.signals["clamp_status"] = "KL_15 OFF"

    def reset(self):
        self.version = ""
        print("ECU Reset Successful")

    def get_temperature(self):
        return 37

    def read_signals(self,signal_name):
        return self.signals.get(signal_name,"Signal Not Found")

    def read_vin(self):
        return "WVWZZZ1KZAW000001"