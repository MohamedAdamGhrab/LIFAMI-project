TARGET = simulation
SRC = src/simulation.cpp
CXX = g++
GRAPIC_DIR = $(HOME)/grapic

CXXFLAGS = -std=c++17 -Wall \
           -I$(GRAPIC_DIR)/src \
           -I$(GRAPIC_DIR)/libs/include

LDFLAGS = -L$(GRAPIC_DIR)/libs/lib \
          -lGrapic \
          -lSDL2 \
          -lSDL2_image \
          -lSDL2_ttf \
          -lSDL2_mixer \
          -lm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
