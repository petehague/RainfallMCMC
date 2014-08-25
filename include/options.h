#ifndef OPTIONS
#define OPTIONS

#include <string>
#include <vector>
#include <cinttypes>

using namespace std;

enum opt_type {
	opt_double,
	opt_string
};

enum opt_vetostate {
	opt_veto, 
	opt_noveto
};

enum opt_rank {
	opt_single,
	opt_array
};

class options {
	vector<opt_type> type;
	vector<string> key;
	vector<double> double_val;
	vector<string> string_val;
	vector<int> veto;
	vector<opt_rank> rank;
	vector<int> arrayindex;
public:
	void addval(string k, double v, opt_vetostate m, opt_rank r);
	void addval(string k, string v, opt_vetostate m, opt_rank r);
	
	void setval(string k, double v);
	void setval(string k, string v);
	opt_type gettype(string k);
	opt_rank getrank(string k);
	void *getval(string k);
	double getdoubleval(string k);
	string getstringval(string k);
	
	bool setval(string k, int i, double v);
	bool setval(string k, int i, string v);
	void *getval(string k, int i);
	double getdoubleval(string k, int i);
	string getstringval(string k, int i);
	
	bool vetoCheck();
	
	int keycount(string k);
	
	void parse(string item);
	void parseFile(string filename);
	void parseCL(int argc, char **argv);
	
	void report();
	
	options();
	options(int argc, char **argv);
};
#endif