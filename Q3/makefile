# Set gcc as the C++ compiler
CXX=gcc


# Enable all standard warnings, uninitialized variable warnings, 
# and treat all warnings as errors
CXXFLAGS=-Wall

Pipes= $(pipe?to?)

tokenring:
	$(CXX) tokenring.c $(CXXFLAGS) -o tokenring

clean:
	rm -rf tokenring
	rm -rf $(Pipes)