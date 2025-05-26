CXX = g++
CXXFLAGS = -std=c++17 -pedantic -g -fPIC

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

MAIN_EXEC = Main
TEST_EXEC = test
GUI_EXEC = gui

QTFLAGS = `pkg-config --cflags --libs Qt5Widgets`

# כל הקבצים חוץ מ-main
NON_MAIN_SRC = $(filter-out Main.cpp, $(SRC))
NON_MAIN_OBJ = $(NON_MAIN_SRC:.cpp=.o)

# כל הקבצים חוץ מ-main וגם חוץ מ-gui
NON_MAIN_NON_GUI_SRC = $(filter-out Main.cpp gui.cpp, $(SRC))
NON_MAIN_NON_GUI_OBJ = $(NON_MAIN_NON_GUI_SRC:.cpp=.o)

.PHONY: all Main test gui valgrind clean

all: $(MAIN_EXEC) $(GUI_EXEC)

Main: $(NON_MAIN_NON_GUI_OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) $(NON_MAIN_NON_GUI_OBJ)

gui.o: gui.cpp
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -c -o gui.o gui.cpp

gui: gui.o $(NON_MAIN_NON_GUI_OBJ)
	$(CXX) $(CXXFLAGS) -o $(GUI_EXEC) gui.o $(NON_MAIN_NON_GUI_OBJ) $(QTFLAGS)

test: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(OBJ)

valgrind: $(MAIN_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(MAIN_EXEC)

clean:
	rm -f *.o $(MAIN_EXEC) $(TEST_EXEC) $(GUI_EXEC)
