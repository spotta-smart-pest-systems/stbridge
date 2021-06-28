print("Import worked!")

import stbridge.stbridge_api as api


class STBridge:
    def __init__(self, i2c_speed=100):
        self.api = api
        self.api.open()
        self.api.initI2C(i2c_speed)

    def read_i2c(self, slave_addr, num_bytes):
        return self.api.readI2C(slave_addr, num_bytes)

    def write_i2c(self, slave_addr, data):
        self.api.writeI2C(slave_addr, data)
