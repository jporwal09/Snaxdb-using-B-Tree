output : main.o show.o Snax_insert.o Snax_delete.o searchKey.o supportfunctions.o
	g++ main.o show.o Snax_insert.o Snax_delete.o searchKey.o supportfunctions.o -o output

main.o : main.cpp
	g++ -c main.cpp	
	   
show.o : show.cpp
	g++ -c show.cpp	

Snax_insert.o : Snax_insert.cpp
	g++ -c Snax_insert.cpp		

Snax_delete.o : Snax_delete.cpp
	g++ -c Snax_delete.cpp

searchKey.o : searchKey.cpp
	g++ -c searchKey.cpp				

supportfunctions.o : supportfunctions.cpp
	g++ -c supportfunctions.cpp	

clean: rm*.o output       