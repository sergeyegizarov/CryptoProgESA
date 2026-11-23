CXX      = g++
CXXFLAGS = -O2 -Wall -std=c++14
LDFLAGS  = -lcryptopp

TARGET   = cipher_prog
SRC      = cipher_prog.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
