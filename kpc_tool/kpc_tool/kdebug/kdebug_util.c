#include "kdebug_util.h"

#include <sys/sysctl.h>

// -----------------------------------------------------------------------------
// kdebug utils
// -----------------------------------------------------------------------------

int kdebug_reset(void) {
    int mib[3] = { CTL_KERN, KERN_KDEBUG, KERN_KDREMOVE };
    return sysctl(mib, 3, NULL, NULL, NULL, 0);
}

int kdebug_reinit(void) {
    int mib[3] = { CTL_KERN, KERN_KDEBUG, KERN_KDSETUP };
    return sysctl(mib, 3, NULL, NULL, NULL, 0);
}

int kdebug_setreg(kd_regtype *kdr) {
    int mib[3] = { CTL_KERN, KERN_KDEBUG, KERN_KDSETREG };
    usize size = sizeof(kd_regtype);
    return sysctl(mib, 3, kdr, &size, NULL, 0);
}

int kdebug_trace_setbuf(int nbufs) {
    int mib[4] = { CTL_KERN, KERN_KDEBUG, KERN_KDSETBUF, nbufs };
    return sysctl(mib, 4, NULL, NULL, NULL, 0);
}

int kdebug_trace_enable(bool enable) {
    int mib[4] = { CTL_KERN, KERN_KDEBUG, KERN_KDENABLE, enable };
    return sysctl(mib, 4, NULL, 0, NULL, 0);
}

int kdebug_get_bufinfo(kbufinfo_t *info) {
    if (!info) return -1;
    int mib[3] = { CTL_KERN, KERN_KDEBUG, KERN_KDGETBUF };
    size_t needed = sizeof(kbufinfo_t);
    return sysctl(mib, 3, info, &needed, NULL, 0);
}

int kdebug_trace_read(void *buf, usize len, usize *count) {
    if (count) *count = 0;
    if (!buf || !len) return -1;
    
    // Note: the input and output units are not the same.
    // input: bytes
    // output: number of kd_buf
    int mib[3] = { CTL_KERN, KERN_KDEBUG, KERN_KDREADTR };
    int ret = sysctl(mib, 3, buf, &len, NULL, 0);
    if (ret != 0) return ret;
    *count = len;
    return 0;
}

int kdebug_wait(usize timeout_ms, bool *suc) {
    if (timeout_ms == 0) return -1;
    int mib[3] = { CTL_KERN, KERN_KDEBUG, KERN_KDBUFWAIT };
    usize val = timeout_ms;
    int ret = sysctl(mib, 3, NULL, &val, NULL, 0);
    if (suc) *suc = !!val;
    return ret;
}
