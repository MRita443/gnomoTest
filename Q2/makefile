# Set gcc as the C++ compiler
CXX=gcc


# Enable all standard warnings, uninitialized variable warnings, 
# and treat all warnings as errors
CXXFLAGS=-Wall

txt2epub:
	$(CC) txt2epub.c $(CFLAGS) -o txt2epub

clean:
	rm -rf txt2epub
	rm -rf *.epub
	rm -rf *.zip