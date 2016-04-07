ann: driver.o layer.o node.o network.o
	g++ -o ann driver.o layer.o node.o network.o

driver.o: driver.cc
	g++ -c -g driver.cc

layer.o: layer.h layer.cc
	g++ -c -g layer.cc

node.o: node.h node.cc
	g++ -c -g node.cc

network.o: network.h network.cc
	g++ -c -g network.cc

clean:
	rm -f *~ ann *.o
