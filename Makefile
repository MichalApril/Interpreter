__start__: obj __lines_for_space__ interp __plugin__
	export LD_LIBRARY_PATH="./libs"; ./interp config.xml

obj:
	mkdir obj


__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo


__plugin__:
	cd plugin; make

CPPFLAGS=-Wall -pedantic -std=c++17 -Iinc
LDFLAGS=-Wall -ldl -lxerces-c

interp: obj/main.o obj/LibInterface.o obj/Set4LibInterface.o \
    obj/Scene.o obj/Cuboid.o obj/xmlinterp.o obj/ProgramInterpreter.o obj/Sender.o
	g++ -o interp obj/main.o obj/LibInterface.o obj/Set4LibInterface.o \
    obj/Scene.o obj/Cuboid.o obj/xmlinterp.o obj/ProgramInterpreter.o obj/Sender.o $(LDFLAGS)


obj/main.o: src/main.cpp inc/AbstractInterp4Command.hh inc/AbstractScene.hh \
            inc/Set4LibInterface.hh inc/LibInterface.hh inc/xmlinterp.hh
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

obj/LibInterface.o: src/LibInterface.cpp inc/LibInterface.hh inc/AbstractInterp4Command.hh
	g++ -c ${CPPFLAGS} -o obj/LibInterface.o src/LibInterface.cpp

obj/Set4LibInterface.o: src/Set4LibInterface.cpp inc/Set4LibInterface.hh inc/LibInterface.hh
	g++ -c ${CPPFLAGS} -o obj/Set4LibInterface.o src/Set4LibInterface.cpp

obj/Scene.o: src/Scene.cpp inc/Scene.hh inc/AbstractScene.hh inc/AbstractMobileObj.hh
	g++ -c ${CPPFLAGS} -o obj/Scene.o src/Scene.cpp

obj/Cuboid.o: src/Cuboid.cpp inc/Cuboid.hh inc/AbstractMobileObj.hh
	g++ -c ${CPPFLAGS} -o obj/Cuboid.o src/Cuboid.cpp

obj/xmlinterp.o: src/xmlinterp.cpp inc/xmlinterp.hh inc/Scene.hh inc/Set4LibInterface.hh
	g++ -c ${CPPFLAGS} -o obj/xmlinterp.o src/xmlinterp.cpp

obj/ProgramInterpreter.o: src/ProgramInterpreter.cpp inc/ProgramInterpreter.hh \
                          inc/xmlinterp.hh inc/Configuration.hh
	g++ -c ${CPPFLAGS} -o obj/ProgramInterpreter.o src/ProgramInterpreter.cpp

obj/Sender.o: src/Sender.cpp inc/Sender.hh inc/Port.hh
	g++ -c ${CPPFLAGS} -o obj/Sender.o src/Sender.cpp

doc:
	cd dox; make

clean:
	rm -f obj/* interp core*


clean_plugin:
	cd plugin; make clean

cleanall: clean
	cd plugin; make cleanall
	cd dox; make cleanall
	rm -f libs/*
	find . -name \*~ -print -exec rm {} \;

help:
	@echo
	@echo "  Lista podcelow dla polecenia make"
	@echo 
	@echo "        - (wywolanie bez specyfikacji celu) wymusza"
	@echo "          kompilacje i uruchomienie programu."
	@echo "  clean    - usuwa produkty kompilacji oraz program"
	@echo "  clean_plugin - usuwa plugin"
	@echo "  cleanall - wykonuje wszystkie operacje dla podcelu clean oraz clean_plugin"
	@echo "             oprocz tego usuwa wszystkie kopie (pliki, ktorych nazwa "
	@echo "             konczy sie znakiem ~)."
	@echo "  help  - wyswietla niniejszy komunikat"
	@echo
	@echo " Przykladowe wywolania dla poszczegolnych wariantow. "
	@echo "  make           # kompilacja i uruchomienie programu."
	@echo "  make clean     # usuwa produkty kompilacji."
	@echo
 