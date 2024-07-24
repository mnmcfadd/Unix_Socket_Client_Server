client:
	mkdir -p build/bin/release
	g++ src/lib.cpp -c -I include
	ar rs libpack109.a lib.o
	g++ src/bin/client.cpp -lpack109 -L. -I include
	mv a.out build/bin/release/client
	mv lib.o build/bin/lib
	mv libpack109.a build/bin/pack109
	
server:
	mkdir -p build/bin/release
	g++ src/lib.cpp -c -I include
	ar rs libpack109.a lib.o
	g++ src/bin/server.cpp -lpack109 -L. -I include
	mv a.out build/bin/release/server
	mv lib.o build/bin/lib
	mv libpack109.a build/bin/pack109

clean:
	rm -r build