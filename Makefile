CXX = g++
CXXFLAGS = -std=c++11 -Wall

ODBC_LIB = -lodbc

SRCDIR = src
OBJDIR = build
BINDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

EXECUTABLE = $(BINDIR)/rpbd

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ODBC_LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(EXECUTABLE)
