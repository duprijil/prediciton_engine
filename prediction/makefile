all: compile run

compile:
	g++ -I. -o app *.cpp *.hpp sqlite3.a -Wall

dll:
	g++ -I. -o app *.cpp *.hpp sqlite3.a -Wall -fPIC -shared -o ../libengine.so

run:
	app