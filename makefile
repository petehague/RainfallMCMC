ifeq ($(build), serial)
	OMPFILE := serial.cpp
	MYCPP := g++ -g -std=c++11 -fPIC
else
	OMPFILE := parallel.cpp
	MYCPP := g++ -g -std=c++11 -fPIC -fopenmp
endif
AGENTFILES := chain.cpp pick.cpp options.cpp
AGENTOPTS := -shared -ldl $(AGENTFILES)

all: folders rainfall mods
	@echo Make complete

mods: mod/likelihood.so mod/histogram.so mod/flat.so mod/blob.so mod/grid.so mod/adaptive.so
	@echo Modules complete

rainfall: bin/master.o bin/chain.o bin/options.o bin/pick.o bin/ompswitch.o
	$(MYCPP) -ldl bin/master.o bin/chain.o bin/options.o bin/pick.o -orainfall bin/ompswitch.o
	@echo MCMC core code complete

#--------------------------------------------------------------
#Core code
#--------------------------------------------------------------

bin/master.o: master.cpp
	$(MYCPP) -c master.cpp -obin/master.o
	
bin/chain.o: chain.cpp
	$(MYCPP) -c chain.cpp -obin/chain.o
	
bin/pick.o: pick.cpp
	$(MYCPP) -c pick.cpp -obin/pick.o
	
bin/options.o: options.cpp
	$(MYCPP) -c options.cpp -obin/options.o
	
bin/ompswitch.o: $(OMPFILE)
	$(MYCPP) -c $(OMPFILE) -obin/ompswitch.o

#--------------------------------------------------------------
#Housekeeping
#--------------------------------------------------------------

folders:
	mkdir -p bin
	mkdir -p mod

clean:	
	rm -f rainfall
	rm -f mod/*.so
	rm -f bin/*.o
	
cleanmods:
	rm -f mod/*.so

#--------------------------------------------------------------
#Agents	
#--------------------------------------------------------------

mod/likelihood.so: likelihood.cpp $(AGENTFILES)
	$(MYCPP) $(AGENTOPTS) likelihood.cpp -omod/likelihood.so

mod/flat.so: flat.cpp $(AGENTFILES)
	$(MYCPP) $(AGENTOPTS) flat.cpp -omod/flat.so

mod/blob.so: blob.cpp $(AGENTFILES)
	$(MYCPP) $(AGENTOPTS) blob.cpp -omod/blob.so
	
mod/histogram.so: histogram.cpp $(AGENTFILES)
	$(MYCPP) $(AGENTOPTS) histogram.cpp -omod/histogram.so

mod/grid.so: grid.cpp $(AGENTFILES)
	$(MYCPP) $(AGENTOPTS) grid.cpp -omod/grid.so
	
mod/adaptive.so: adaptive.cpp $(AGENTFILES)
	$(MYCPP) $(AGENTOPTS) adaptive.cpp -omod/adaptive.so
