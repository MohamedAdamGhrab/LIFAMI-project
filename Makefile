# ============================================================
#  Makefile — Simulation Proies/Prédateurs
#  Université Lyon 1 — UE LIFAMI — Ghrab Mohamed Adam
# ============================================================

# Nom de l'exécutable
TARGET = simulation

# Fichier source
SRC = src/simulation.cpp

# Compilateur
CXX = g++

# ── Chemin vers Grapic (à adapter selon ton installation) ──
# Linux / macOS typique :
GRAPIC_DIR = $(HOME)/grapic

# Flags de compilation
CXXFLAGS = -std=c++11 -Wall \
           -I$(GRAPIC_DIR)/src \
           -I$(GRAPIC_DIR)/libs/include

# Flags de liaison
LDFLAGS = -L$(GRAPIC_DIR)/libs/lib \
          -lGrapic \
          -lSDL2 \
          -lSDL2_image \
          -lSDL2_ttf \
          -lSDL2_mixer \
          -lm

# ── Règles ──────────────────────────────────────────────────

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)
	@echo "Compilation réussie → ./$(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
