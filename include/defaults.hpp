#ifndef DEFAULTS
#define DEFAULTS

#define ADD_DEFAULTS \
	addval("MaxModels", 0, opt_veto, opt_single); \
	addval("NParams", 0, opt_veto, opt_single); \
	addval("Likelihood", " ", opt_veto, opt_single); \
	addval("Agent", "mod/likelihood.so", opt_noveto, opt_array); \
	addval("Outputfile", 0, opt_veto, opt_single);

#endif