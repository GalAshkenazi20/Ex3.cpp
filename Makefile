CXX = g++
CXXFLAGS = -std=c++17 -pedantic -g -fPIC

ALL_SRC = $(wildcard *.cpp)


CORE_SRC = $(filter-out Main.cpp gui.cpp test.cpp, $(ALL_SRC))
CORE_OBJ = $(CORE_SRC:.cpp=.o)


MAIN_EXEC = Main
TEST_EXEC = test
GUI_EXEC = gui

QTFLAGS = `pkg-config --cflags --libs Qt5Widgets`

.PHONY: all main test valgrind clean help


all: $(MAIN_EXEC) $(GUI_EXEC) $(TEST_EXEC)
	@echo "All executables built successfully!"
	@echo "Run 'make main' to execute Main program"
	@echo "Run 'make gui' to launch GUI"
	@echo "Run 'make test' to run tests"


main: $(MAIN_EXEC)
	@echo "Running Main program..."
	./$(MAIN_EXEC)

$(MAIN_EXEC): Main.o $(CORE_OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) Main.o $(CORE_OBJ)


gui: $(GUI_EXEC)
	@echo "Launching GUI..."
	./$(GUI_EXEC)

gui.o: gui.cpp
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -c -o gui.o gui.cpp

$(GUI_EXEC): gui.o $(CORE_OBJ)
	$(CXX) $(CXXFLAGS) -o $(GUI_EXEC) gui.o $(CORE_OBJ) $(QTFLAGS)


test: $(TEST_EXEC)
	@echo "Running tests..."
	./$(TEST_EXEC)

$(TEST_EXEC): test.o $(CORE_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) test.o $(CORE_OBJ)


valgrind: $(MAIN_EXEC)
	@echo "Running valgrind on Main program..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(MAIN_EXEC)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	@echo "Cleaning all files..."
	rm -f *.o $(MAIN_EXEC) $(TEST_EXEC) $(GUI_EXEC)
	@echo "Clean complete!"

# עזרה
help:
	@echo "Available targets:"
	@echo "  all      - Build all executables"
	@echo "  main     - Build and run Main program"
	@echo "  gui      - Build and run GUI"
	@echo "  test     - Build and run tests"
	@echo "  valgrind - Run Main with valgrind memory check"
	@echo "  clean    - Remove all built files"
	@echo "  help     - Show this help message"
