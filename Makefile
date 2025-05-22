CXX = g++
CXXFLAGS = -std=c++17 -pedantic -g

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

# קובצי הרצה
MAIN_EXEC = Main
TEST_EXEC = test

.PHONY: all Main test valgrind clean

all: $(MAIN_EXEC)

# קומפילציה לקובץ הדגמה (Main)
Main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) $(OBJ)

# בדיקות יחידה (test.cpp צריך להיות קובץ טסטים)
test: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(OBJ)

# בדיקת נזילות זיכרון עם valgrind (דורש התקנת valgrind)
valgrind: $(MAIN_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(MAIN_EXEC)

# ניקוי כל הקבצים המתורגמים
clean:
	rm -f *.o $(MAIN_EXEC) $(TEST_EXEC)
