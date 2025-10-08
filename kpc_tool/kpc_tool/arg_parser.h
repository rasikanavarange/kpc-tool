#ifndef arg_parser_h
#define arg_parser_h

struct profiler_config {
    int target_pid;
    double total_profile_time;
    double sample_period;
};

void parse_arguments(int argc, char* argv[], struct profiler_config* config);

#endif /* arg_parser_h */
