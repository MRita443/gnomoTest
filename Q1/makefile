# Set gcc as the C++ compiler
CXX=gcc


# Enable all standard warnings, uninitialized variable warnings, 
# and treat all warnings as errors
CXXFLAGS=-Wall

samples:
	$(CXX) samples.c $(CXXFLAGS) -o samples

clean:
	rm -rf samples