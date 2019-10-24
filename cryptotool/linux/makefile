INCLUDE_PATH := -I/usr/local/include/libls
LIB_PATH := 
LIB := -lls

cryptotool: ../cryptotool.cpp
	g++ -g $(INCLUDE_PATH) $(LIB_PATH) $(LIB) -o cryptotool ../cryptotool.cpp

clean:
	rm -rf cryptotool

.PHONY: clean
