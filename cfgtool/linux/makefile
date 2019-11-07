DEFINES := -DUSE_LOG4CXX -DUSE_LIBCURL -DUSE_LIBICONV
INCLUDE_PATH := -I/usr/local/include/libls -I/usr/local/include/libls/core -I/usr/local/include/libls/utils -I/usr/local/include/libls_util
LIB_PATH := 
LIB := -lls -lls_util

cfgtool: ../cfgtool.cpp
	g++ -g $(INCLUDE_PATH) $(LIB_PATH) $(LIB) -o cfgtool ../cfgtool.cpp

clean:
	rm -rf cfgtool

.PHONY: clean
