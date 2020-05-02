all: compile run

compile:
	g++ -I. -o app *.cpp *.hpp sqlite3.a -Wall

dll:
	g++ -std=c++11 -I. *.cpp *.hpp -lsqlite3 -Wall -fPIC -shared -o ../libengine.so

run:
	app