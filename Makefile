bst : main.cpp bst.cpp bst.h
	g++ -o bst main.cpp bst.cpp

test.png : bst
	./bst | dot -T svg -o test.png
