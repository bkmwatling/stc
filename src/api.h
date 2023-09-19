#ifndef STC_API_H
#define STC_API_H

/* Set all short names macros if STC_ENABLE_SHORT_NAMES is set */
#ifdef STC_ENABLE_SHORT_NAMES
#    ifndef STC_COMMON_ENABLE_SHORT_NAMES
#        define STC_COMMON_ENABLE_SHORT_NAMES
#    endif
#    ifndef STC_MEM_ENABLE_SHORT_NAMES
#        define STC_MEM_ENABLE_SHORT_NAMES
#    endif
#    ifndef STC_SLICE_ENABLE_SHORT_NAMES
#        define STC_SLICE_ENABLE_SHORT_NAMES
#    endif
#    ifndef STC_VEC_ENABLE_SHORT_NAMES
#        define STC_VEC_ENABLE_SHORT_NAMES
#    endif
#endif /* STC_ENABLE_SHORT_NAMES */

#include "common.h"
#include "math.h"
#include "memory.h"

#include "fatp/slice.h"
#include "fatp/vec.h"

#endif /* STC_API_H */
