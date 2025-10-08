#include "kpc.h"
#include "arg_parser.h"

int main(int argc, char * argv[]) {
    struct profiler_config config = {
        .target_pid = -1,          // Default -1 for all processes
        .total_profile_time = 10,  // Default to 10s
        .sample_period = 0.0001    // Default to 10ms
    };
    
    parse_arguments(argc, argv, &config);
    
    return profile_process(config.target_pid, config.total_profile_time, config.sample_period);
}
