#ifndef kperf_loader_h
#define kperf_loader_h

#include "kperf_framework.h"

#include <dlfcn.h>          // for dlopen() and dlsym()
#include <stdio.h>


// -----------------------------------------------------------------------------
// load kperf/kperfdata dynamic library
// -----------------------------------------------------------------------------

#define lib_path_kperf "/System/Library/PrivateFrameworks/kperf.framework/kperf"
#define lib_path_kperfdata "/System/Library/PrivateFrameworks/kperfdata.framework/kperfdata"
#define lib_nelems(x)  (sizeof(x) / sizeof((x)[0]))
#define lib_symbol_def(name) { #name, (void **)&name }

typedef struct {
    const char *name;    void **impl;
} lib_symbol;

static const lib_symbol lib_symbols_kperf[] = {
    lib_symbol_def(kpc_pmu_version),
    lib_symbol_def(kpc_cpu_string),
    lib_symbol_def(kpc_set_counting),
    lib_symbol_def(kpc_get_counting),
    lib_symbol_def(kpc_set_thread_counting),
    lib_symbol_def(kpc_get_thread_counting),
    lib_symbol_def(kpc_get_config_count),
    lib_symbol_def(kpc_get_counter_count),
    lib_symbol_def(kpc_set_config),
    lib_symbol_def(kpc_get_config),
    lib_symbol_def(kpc_get_cpu_counters),
    lib_symbol_def(kpc_get_thread_counters),
    lib_symbol_def(kpc_force_all_ctrs_set),
    lib_symbol_def(kpc_force_all_ctrs_get),
    lib_symbol_def(kperf_action_count_set),
    lib_symbol_def(kperf_action_count_get),
    lib_symbol_def(kperf_action_samplers_set),
    lib_symbol_def(kperf_action_samplers_get),
    lib_symbol_def(kperf_action_filter_set_by_task),
    lib_symbol_def(kperf_action_filter_set_by_pid),
    lib_symbol_def(kperf_timer_count_set),
    lib_symbol_def(kperf_timer_count_get),
    lib_symbol_def(kperf_timer_period_set),
    lib_symbol_def(kperf_timer_period_get),
    lib_symbol_def(kperf_timer_action_set),
    lib_symbol_def(kperf_timer_action_get),
    lib_symbol_def(kperf_sample_set),
    lib_symbol_def(kperf_sample_get),
    lib_symbol_def(kperf_reset),
    lib_symbol_def(kperf_timer_pet_set),
    lib_symbol_def(kperf_timer_pet_get),
    lib_symbol_def(kperf_ns_to_ticks),
    lib_symbol_def(kperf_ticks_to_ns),
    lib_symbol_def(kperf_tick_frequency),
};

static const lib_symbol lib_symbols_kperfdata[] = {
    lib_symbol_def(kpep_config_create),
    lib_symbol_def(kpep_config_free),
    lib_symbol_def(kpep_config_add_event),
    lib_symbol_def(kpep_config_remove_event),
    lib_symbol_def(kpep_config_force_counters),
    lib_symbol_def(kpep_config_events_count),
    lib_symbol_def(kpep_config_events),
    lib_symbol_def(kpep_config_kpc),
    lib_symbol_def(kpep_config_kpc_count),
    lib_symbol_def(kpep_config_kpc_classes),
    lib_symbol_def(kpep_config_kpc_map),
    lib_symbol_def(kpep_db_create),
    lib_symbol_def(kpep_db_free),
    lib_symbol_def(kpep_db_name),
    lib_symbol_def(kpep_db_aliases_count),
    lib_symbol_def(kpep_db_aliases),
    lib_symbol_def(kpep_db_counters_count),
    lib_symbol_def(kpep_db_events_count),
    lib_symbol_def(kpep_db_events),
    lib_symbol_def(kpep_db_event),
    lib_symbol_def(kpep_event_name),
    lib_symbol_def(kpep_event_alias),
    lib_symbol_def(kpep_event_description),
};

static bool lib_inited = false;
static bool lib_has_err = false;
static char lib_err_msg[256];

static void *lib_handle_kperf = NULL;
static void *lib_handle_kperfdata = NULL;

static void lib_deinit(void) {
    lib_inited = false;
    lib_has_err = false;
    if (lib_handle_kperf) dlclose(lib_handle_kperf);
    if (lib_handle_kperfdata) dlclose(lib_handle_kperfdata);
    lib_handle_kperf = NULL;
    lib_handle_kperfdata = NULL;
    for (usize i = 0; i < lib_nelems(lib_symbols_kperf); i++) {
        const lib_symbol *symbol = &lib_symbols_kperf[i];
        *symbol->impl = NULL;
    }
    for (usize i = 0; i < lib_nelems(lib_symbols_kperfdata); i++) {
        const lib_symbol *symbol = &lib_symbols_kperfdata[i];
        *symbol->impl = NULL;
    }
}

static bool lib_init(void) {
#define return_err() do { \
    lib_deinit(); \
    lib_inited = true; \
    lib_has_err = true; \
    return false; \
} while(false)
    
    if (lib_inited) return !lib_has_err;
    
    // load dynamic library
    lib_handle_kperf = dlopen(lib_path_kperf, RTLD_LAZY);
    if (!lib_handle_kperf) {
        snprintf(lib_err_msg, sizeof(lib_err_msg),
                 "Failed to load kperf.framework, message: %s.", dlerror());
        return_err();
    }
    lib_handle_kperfdata = dlopen(lib_path_kperfdata, RTLD_LAZY);
    if (!lib_handle_kperfdata) {
        snprintf(lib_err_msg, sizeof(lib_err_msg),
                 "Failed to load kperfdata.framework, message: %s.", dlerror());
        return_err();
    }
    
    // load symbol address from dynamic library
    for (usize i = 0; i < lib_nelems(lib_symbols_kperf); i++) {
        const lib_symbol *symbol = &lib_symbols_kperf[i];
        *symbol->impl = dlsym(lib_handle_kperf, symbol->name);
        if (!*symbol->impl) {
            snprintf(lib_err_msg, sizeof(lib_err_msg),
                     "Failed to load kperf function: %s.", symbol->name);
            return_err();
        }
    }
    for (usize i = 0; i < lib_nelems(lib_symbols_kperfdata); i++) {
        const lib_symbol *symbol = &lib_symbols_kperfdata[i];
        *symbol->impl = dlsym(lib_handle_kperfdata, symbol->name);
        if (!*symbol->impl) {
            snprintf(lib_err_msg, sizeof(lib_err_msg),
                     "Failed to load kperfdata function: %s.", symbol->name);
            return_err();
        }
    }
    
    lib_inited = true;
    lib_has_err = false;
    return true;
    
#undef return_err
}

#endif /* kperf_loader_h */
