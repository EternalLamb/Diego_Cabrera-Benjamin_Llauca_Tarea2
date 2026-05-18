CXXFLAGS = -g -Wall -Wextra

src = $(wildcard src/*.cpp)
OBJS = $(src:.cpp=.o)


LIBS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_gfx

TARGET =	Pacman.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

