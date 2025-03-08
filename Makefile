# Détecter le système d'exploitation
UNAME_S := $(shell uname -s)

# Définir le compilateur en fonction du système d'exploitation
ifeq ($(UNAME_S), Darwin)
  CXX = clang++
else
  CXX = g++
endif

# Compilateur C++
CXX = clang++

# Drapeaux de compilation
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Nom de l'exécutable
TARGET = room_robot.exe

# Répertoires sources
SRC_DIRS = . graphisme interface_client objet objet/forme objet/room physique svg util

# Fichiers sources
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# Fichiers objets
OBJS = $(SRCS:.cpp=.o)

# Fichiers d'en-tête
INCLUDES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.h))

# Règle par défaut
all: $(TARGET)

# Règle pour générer l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Règle pour générer les fichiers objets
%.o: %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
