CXX       = g++
CXXFLAGS  = -Wall -I.
LDFLAGS   = -fopenmp

PROGRAM   = virusim
SRCS      = virusim.cpp Random.cpp Population.cpp
OBJS      = $(SRCS:.cpp=.o)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(LDFLAGS) -c $(CXXFLAGS) $<

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(SRCS) $(CXXFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
