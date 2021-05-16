main:
	mkdir build & cd build && cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ../ && cmake --build . --parallel
test:
	cd tests && mkdir build & cd build && cmake -DTESTS=1 -G"Unix Makefiles" ../../ && cmake --build . --parallel
	./tests/build/run_tests