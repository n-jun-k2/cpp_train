.ONESHELL:
.PHONY: clean build

clean:
	-rm -r ./$(dir)/build

build: 
	cmake -S ./$(dir) -B ./$(dir)/build
	cmake --build ./$(dir)/build
