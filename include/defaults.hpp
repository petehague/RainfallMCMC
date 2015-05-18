#ifndef DEFAULTS
#define DEFAULTS

#define ADD_DEFAULTS \
	addval("MaxModels", 0, opt_veto, opt_single); \
	addval("NParams", 0, opt_veto, opt_single); \
	addval("Likelihood", " ", opt_veto, opt_single); \
	addval("Agent", "", opt_noveto, opt_emptyarray); \
	addval("agentparams", "", opt_noveto, opt_emptyarray); \
	addval("Outputfile", " ", opt_veto, opt_single); \
	addval("paramname", "", opt_noveto, opt_emptyarray); \
	addval("lowerlimit", 0, opt_noveto, opt_emptyarray); \
	addval("upperlimit", 0, opt_noveto, opt_emptyarray); \
	addval("stepsize", 0, opt_noveto, opt_emptyarray); \
	addval("startmean", 0, opt_noveto, opt_emptyarray); \
	addval("startdev", 0, opt_noveto, opt_emptyarray); \
	addval("limtype", "flat", opt_noveto, opt_emptyarray);

#endif