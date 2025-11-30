
CXX = g++
CXXFLAGS = -std=c++11 -Wall

CORE_DIR = core
SFML_DIR = sfml
OUT_DIR = out

CORE_SRCS = $(CORE_DIR)/simulation_state.cpp \
            $(CORE_DIR)/io.cpp \
            $(CORE_DIR)/grid.cpp \
            $(CORE_DIR)/trains.cpp \
            $(CORE_DIR)/switches.cpp \
            $(CORE_DIR)/simulation.cpp

CORE_OBJS = $(CORE_SRCS:.cpp=.o)

all: switchback_rails

switchback_rails: $(CORE_OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $(CORE_OBJS) main.cpp -o switchback_rails

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(CORE_OBJS)
	rm -f switchback_rails
	rm -f $(OUT_DIR)/*.csv $(OUT_DIR)/*.txt

run-easy: switchback_rails
	./switchback_rails data/levels/easy_level.lvl

run-medium: switchback_rails
	./switchback_rails data/levels/medium_level.lvl

run-hard: switchback_rails
	./switchback_rails data/levels/hard_level.lvl

.PHONY: all clean run-easy run-medium run-hard
