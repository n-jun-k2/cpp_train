.ONESHELL:
.PHONY: clean build

clean/%:
	@echo "\e[36m  _/_/_/_/_/_/ Clean Start _/_/_/_/_/_/ \e[m"
	-rm -r ./$(@F)/build
	@echo "\e[36m  _/_/_/_/_/_/ Clean End _/_/_/_/_/_/ \e[m"

build/%: 
	@echo "\e[36m  _/_/_/_/_/_/ BUILD START _/_/_/_/_/_/ \e[m"
	-rm -r ./$(@F)/build
	cmake -S ./$(@F) -B ./$(@F)/build
	cmake --build ./$(@F)/build
	@echo "\e[36m _/_/_/_/_/_/ BUILD END _/_/_/_/_/_/ \e[m"

.PHONY: project
project/%:
	cp -r /tmp/src/template/project $(@F)
	sed -i -e "2i project($(@F))" $(@F)/CMakeLists.txt