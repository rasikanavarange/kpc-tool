#ifndef kdebug_util_h
#define kdebug_util_h

#include "kdebug_private.h"
#include "types.h"

#include <stdbool.h>

/// Clean up trace buffers and reset ktrace/kdebug/kperf.
/// @return 0 on success.
int kdebug_reset(void);

/// Disable and reinitialize the trace buffers.
/// @return 0 on success.
int kdebug_reinit(void);

/// Set debug filter.
int kdebug_setreg(kd_regtype *kdr);

/// Set maximum number of trace entries (kd_buf).
/// Only allow allocation up to half the available memory (sane_size).
/// @return 0 on success.
int kdebug_trace_setbuf(int nbufs);

/// Enable or disable kdebug trace.
/// Trace buffer must already be initialized.
/// @return 0 on success.
int kdebug_trace_enable(bool enable);

/// Retrieve trace buffer information from kernel.
/// @return 0 on success.
int kdebug_get_bufinfo(kbufinfo_t *info);

/// Retrieve trace buffers from kernel.
/// @param buf Memory to receive buffer data, array of `kd_buf`.
/// @param len Length of `buf` in bytes.
/// @param count Number of trace entries (kd_buf) obtained.
/// @return 0 on success.
int kdebug_trace_read(void *buf, usize len, usize *count);

/// Block until there are new buffers filled or `timeout_ms` have passed.
/// @param timeout_ms timeout milliseconds, 0 means wait forever.
/// @param suc set true if new buffers filled.
/// @return 0 on success.
int kdebug_wait(usize timeout_ms, bool *suc);

#endif /* kdebug_util_h */
