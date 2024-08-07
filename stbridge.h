#ifndef STBRIDGE_H
#define STBRIDGE_H

#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/python.hpp>

#include <iostream>
#include <string>
// #include <format> // no compiler support yet :(
#include <fmt/format.h> // once support added switch back to std::format

#include "bridge.h"

/* 
 *  Overarching design philosophy is simplicity, this is made for the average user
 */

namespace stbridge_api {
	void open();
	void close();

	float getTargetVoltage();

	enum class bitorderSPI { LSB = SPI_FIRSTBIT_LSB, MSB = SPI_FIRSTBIT_MSB };
	enum class modeSPI { MODE0, MODE1, MODE2, MODE3 };

	uint32_t initSPI(int kHz, bitorderSPI bitorder, modeSPI mode);
	boost::python::object readSPI(uint16_t len);
	void writeSPI(std::string data);
	void setnssSPI(bool level);

	void initI2C(int kHz);
	boost::python::object readI2C(uint16_t addr, uint16_t len);
	void writeI2C(uint16_t addr, std::string data);

	enum class modeGPIO { OUTPUT, INPUT, INPUT_PULLUP, INPUT_PULLDOWN };

	void initGPIO();
	void pinmodeGPIO(uint8_t pin, modeGPIO mode);
	void writeGPIO(uint8_t pin, bool level);
	bool readGPIO(uint8_t pin);

	struct msgCAN {
		uint32_t id;
		boost::python::object data; // bytes object
		bool remote;
		bool extended;

		msgCAN(uint32_t id=0, std::string data="", bool remote=0, bool extended=0) :
			id(id),
			data(boost::python::object(boost::python::handle<>(PyBytes_FromStringAndSize(data.c_str(), data.size())))),
			remote(remote), extended(extended) {}

		static boost::shared_ptr<msgCAN> msgCAN_init(uint32_t id, std::string data, bool remote, bool extended) {
			return boost::shared_ptr<msgCAN>(new msgCAN(id, data, remote, extended));
		}
	};

	uint32_t initCAN(int bps);
	void writeCAN(msgCAN msg);
	msgCAN readCAN();
	uint16_t readableCAN();

	void checkError(Brg_StatusT stat);
	void checkNull(void* ptr);
}

void translate_c_str(const char* s) {
	PyErr_SetString(PyExc_RuntimeError, s);
}

void translate_str(std::string& s) {
	PyErr_SetString(PyExc_RuntimeError, s.c_str());
}

BOOST_PYTHON_MODULE(stbridge_api) {
	boost::python::register_exception_translator<const char*>(translate_c_str);
	boost::python::register_exception_translator<std::string&>(translate_str);

	boost::python::def("open", stbridge_api::open);
	boost::python::def("close", stbridge_api::close);

	boost::python::def("getTargetVoltage", stbridge_api::getTargetVoltage);

	boost::python::enum_<stbridge_api::bitorderSPI>("bitorderSPI")
		.value("LSB", stbridge_api::bitorderSPI::LSB)
		.value("MSB", stbridge_api::bitorderSPI::MSB);

	boost::python::enum_<stbridge_api::modeSPI>("modeSPI")
		.value("MODE0", stbridge_api::modeSPI::MODE0)
		.value("MODE1", stbridge_api::modeSPI::MODE1)
		.value("MODE2", stbridge_api::modeSPI::MODE2)
		.value("MODE3", stbridge_api::modeSPI::MODE3);

	boost::python::def("initSPI", stbridge_api::initSPI);
	boost::python::def("readSPI", stbridge_api::readSPI);
	boost::python::def("writeSPI", stbridge_api::writeSPI);
	boost::python::def("setnssSPI", stbridge_api::setnssSPI);

	boost::python::def("initI2C", stbridge_api::initI2C);
	boost::python::def("readI2C", stbridge_api::readI2C);
	boost::python::def("writeI2C", stbridge_api::writeI2C);

	boost::python::enum_<stbridge_api::modeGPIO>("modeGPIO")
		.value("OUTPUT", stbridge_api::modeGPIO::OUTPUT)
		.value("INPUT", stbridge_api::modeGPIO::INPUT)
		.value("INPUT_PULLUP", stbridge_api::modeGPIO::INPUT_PULLUP)
		.value("INPUT_PULLDOWN", stbridge_api::modeGPIO::INPUT_PULLDOWN);

	boost::python::scope().attr("numGPIO") = BRG_GPIO_MAX_NB;

	boost::python::def("initGPIO", stbridge_api::initGPIO);
	boost::python::def("pinmodeGPIO", stbridge_api::pinmodeGPIO);
	boost::python::def("writeGPIO", stbridge_api::writeGPIO);
	boost::python::def("readGPIO", stbridge_api::readGPIO);

	boost::python::class_<stbridge_api::msgCAN>("msgCAN", boost::python::no_init)
		.def("__init__", boost::python::make_constructor(stbridge_api::msgCAN::msgCAN_init, boost::python::default_call_policies(), (
			boost::python::arg("id"), 
			boost::python::arg("data"), 
			boost::python::arg("remote")=false, 
			boost::python::arg("extended")=false)))
		.def(boost::python::self_ns::repr(boost::python::self_ns::self))
		.def_readonly("extended", &stbridge_api::msgCAN::extended)
		.def_readonly("id", &stbridge_api::msgCAN::id)
		.def_readonly("remote", &stbridge_api::msgCAN::remote)
		.def_readonly("data", &stbridge_api::msgCAN::data);

	boost::python::def("initCAN", stbridge_api::initCAN);
	boost::python::def("writeCAN", stbridge_api::writeCAN);
	boost::python::def("readCAN", stbridge_api::readCAN);
	boost::python::def("readableCAN", stbridge_api::readableCAN);
}

#endif
