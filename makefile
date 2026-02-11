# Binary:
venv: .obj .obj/main.cpp.o .obj/repository.cpp.o .obj/static.cpp.o .obj/venv.cpp.o .obj/venv_item.cpp.o .obj/x_options.cpp.o .obj/x_process.cpp.o .obj/x_shell.cpp.o .obj/x_table.cpp.o .obj/x_uuid.cpp.o
	g++ -std=c++23 .obj/main.cpp.o .obj/repository.cpp.o .obj/static.cpp.o .obj/venv.cpp.o .obj/venv_item.cpp.o .obj/x_options.cpp.o .obj/x_process.cpp.o .obj/x_shell.cpp.o .obj/x_table.cpp.o .obj/x_uuid.cpp.o -o venv

.obj:
	mkdir .obj

.obj/main.cpp.o: main.cpp x_options.hpp x_shell.hpp repository.hpp
	g++ -std=c++23 -o .obj/main.cpp.o -c main.cpp

.obj/repository.cpp.o: repository.cpp repository.hpp
	g++ -std=c++23 -o .obj/repository.cpp.o -c repository.cpp

.obj/static.cpp.o: static.cpp static.hpp
	g++ -std=c++23 -o .obj/static.cpp.o -c static.cpp

.obj/venv.cpp.o: venv.cpp venv.hpp
	g++ -std=c++23 -o .obj/venv.cpp.o -c venv.cpp

.obj/venv_item.cpp.o: venv_item.cpp venv_item.hpp
	g++ -std=c++23 -o .obj/venv_item.cpp.o -c venv_item.cpp

.obj/x_options.cpp.o: x_options.cpp x_options.hpp x_table.hpp
	g++ -std=c++23 -o .obj/x_options.cpp.o -c x_options.cpp

.obj/x_process.cpp.o: x_process.cpp
	g++ -std=c++23 -o .obj/x_process.cpp.o -c x_process.cpp

.obj/x_shell.cpp.o: x_shell.cpp x_shell.hpp
	g++ -std=c++23 -o .obj/x_shell.cpp.o -c x_shell.cpp

.obj/x_table.cpp.o: x_table.cpp x_table.hpp
	g++ -std=c++23 -o .obj/x_table.cpp.o -c x_table.cpp

.obj/x_uuid.cpp.o: x_uuid.cpp x_uuid.hpp
	g++ -std=c++23 -o .obj/x_uuid.cpp.o -c x_uuid.cpp

run: venv
	./venv

clean:
	rm -rfv .obj

install:
	sudo cp -v venv /usr/local/bin/

uninstall:
	sudo rm -v /usr/local/bin/venv
