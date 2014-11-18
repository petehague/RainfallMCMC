#include <omp.h>
#include <cinttypes>

uint16_t thread_num() {
	return omp_get_thread_num();
}

uint16_t num_threads() {
	return omp_get_num_threads();
}

void set_threads(uint16_t n) {
	omp_set_num_threads(n);
}
