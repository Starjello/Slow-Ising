#include "IsingModel.h"
#include <sys/time.h>

int main(){
    struct timeval start_time, stop_time, elapsed_time;  // timers

    gettimeofday(&start_time,NULL); // Unix timer

    // IsingExperiment::phase_diagram(100);
    // IsingExperiment::hysteresis(20);
    IsingExperiment::movie(20);
    
    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine

    printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);


    return 0;
}