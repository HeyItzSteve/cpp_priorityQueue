INC_DIR := ../include
CFLAGS := -Wall -Werror -Wextra -I$(INC_DIR) -o

all: hash_table priority_queue main

hash_table: demo_hash_table.cpp $(INC_DIR)/priority_queue.hpp
	g++ $(CFLAGS) demo_hash_table.x demo_hash_table.cpp

priority_queue: demo_priority_queue.cpp $(INC_DIR)/priority_queue.hpp
	g++ $(CFLAGS) demo_priority_queue.x demo_priority_queue.cpp

main: mainxd.cpp $(INC_DIR)/priority_queue.hpp
	g++ $(CFLAGS) mainxd.x mainxd.cpp

clean:
	rm *.x

