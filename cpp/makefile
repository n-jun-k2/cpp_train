.ONESHELL:
.PHONY: clean build

clean:
	-rm -r ./$(dir)/build

build: 
	cmake -S ./$(dir) -B ./$(dir)/build
	cmake --build ./$(dir)/build

.PHONY: project
project:
	cp -r /tmp/src/template/project $(name)
	sed -i -e "2i project($(name))" $(name)/CMakeLists.txt