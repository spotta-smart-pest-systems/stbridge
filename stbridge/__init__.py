import stbridge_api as api


class STBridge:
    def __init__(self, i2c_speed=100):
        self.open()
        api.initI2C(i2c_speed)

    def open(self):
        api.open()

    def close(self):
        api.close()

    def target_voltage(self):
        return api.getTargetVoltage()

    def read_i2c(self, slave_addr, num_bytes):
        return api.readI2C(slave_addr, num_bytes)

    def write_i2c(self, slave_addr, data):
        api.writeI2C(slave_addr, data)

