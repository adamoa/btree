btree : main.cpp btree.cpp btree.h
	g++ -o btree main.cpp btree.cpp

test.png : btree
	./btree | dot -T png > test.png
