#include "arg_parser.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void print_usage(const char* tool_name) {
    printf("\nUsage: %s [options]\n\n", tool_name);
    printf("Optional:\n");
    printf("  -p, --target-pid <pid>              The process ID to profile.\n");
    printf("  -t, --total-profile-time <seconds>  The total time in seconds to run the profiler (Default: 10).\n");
    printf("  -s, --sample-period <seconds>       The sampling period in seconds (Default: 0.0001).\n");
    printf("  -h, --help                          Display this help message.\n\n");
    printf("Example:\n");
    printf("  %s --target-pid 12345 --total-profile-time 10\n", tool_name);
    printf("  %s -p 12345 -t 10\n\n", tool_name);
}

void parse_arguments(int argc, char* argv[], struct profiler_config* config) {
    int opt;
    int long_index = 0;
    static struct option long_options[] = {
        {"target-pid", required_argument, 0, 'p'},
        {"total-profile-time", required_argument, 0, 't'},
        {"sample-period", required_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:t:s:h", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'p':
                config->target_pid = atoi(optarg);
                break;
            case 't':
                config->total_profile_time = atof(optarg);
                break;
            case 's':
                config->sample_period = atof(optarg);
                break;
            case 'h':
                print_usage(argv[0]);
                exit(EXIT_SUCCESS);
            case '?':  // This case is triggered for unknown options or missing arguments
                printf("Unknown arg");  // TODO: add a print_usage() function
                exit(EXIT_FAILURE);
            default:
                // This should not happen
                abort();
        }
    }
}
