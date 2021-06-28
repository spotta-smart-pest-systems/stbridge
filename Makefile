all:
	python3 setup.py build_ext

clean:
	rm -rf build
	rm stbridge.*.so
