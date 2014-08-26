MYCPP := g++ -g -std=c++11 -fPIC

all: folders rainfall mods
	@echo Make complete

mods: mod/likelihood.so
	@echo Modules complete

rainfall: bin/master.o bin/chain.o bin/options.o bin/pick.o
	$(MYCPP) -ldl bin/master.o bin/chain.o bin/options.o -orainfall
	@echo MCMC core code complete

bin/master.o: master.cpp
	$(MYCPP) -c master.cpp -obin/master.o
	
bin/chain.o: chain.cpp
	$(MYCPP) -c chain.cpp -obin/chain.o
	
bin/pick.o: pick.cpp
	$(MYCPP) -c pick.cpp -obin/pick.o
	
bin/options.o: options.cpp
	$(MYCPP) -c options.cpp -obin/options.o
	
mod/likelihood.so: likelihood.cpp chain.cpp
	$(MYCPP) -shared -ldl likelihood.cpp chain.cpp -omod/likelihood.so
	
mod/histogram.so: histogram.cpp chain.cpp
	$(MYCPP) -shared -ldl histogram.cpp chain.cpp -omod/histogram.so

folders:
	mkdir -p bin
	mkdir -p mod

clean:	
	rm rainfall
	rm mod/*.so
	rm bin/*.o
