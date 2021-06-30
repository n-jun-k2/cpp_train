.ONESHELL:
.PHONY: clean build

clean:
	@echo "\e[36m  _/_/_/_/_/_/ Clean _/_/_/_/_/_/ \e[m"
	-rm -r ./$(dir)/build

build: 
	@echo "\e[36m  _/_/_/_/_/_/ BUILD START _/_/_/_/_/_/ \e[m"
	-rm -r ./$(dir)/build
	cmake -S ./$(dir) -B ./$(dir)/build
	cmake --build ./$(dir)/build
	@echo "\e[36m _/_/_/_/_/_/ BUILD END _/_/_/_/_/_/ \e[m"

.PHONY: project
project:
	cp -r /tmp/src/template/project $(name)
	sed -i -e "2i project($(name))" $(name)/CMakeLists.txt