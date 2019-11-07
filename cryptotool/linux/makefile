DEFINES := -DUSE_LOG4CXX -DUSE_LIBCURL -DUSE_LIBICONV
INCLUDE_PATH := -I/usr/local/include/libls -I/usr/local/include/libls/core -I/usr/local/include/libls/utils -I/usr/local/include/libls_util
LIB_PATH := 
LIB := -lls -lls_util

cryptotool: ../cryptotool.cpp
	g++ -g $(INCLUDE_PATH) $(LIB_PATH) $(LIB) -o cryptotool ../cryptotool.cpp

clean:
	rm -rf cryptotool

.PHONY: clean
