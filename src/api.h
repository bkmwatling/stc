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
#    ifndef STC_SV_ENABLE_SHORT_NAMES
#        define STC_SV_ENABLE_SHORT_NAMES
#    endif
#    ifndef STC_VEC_ENABLE_SHORT_NAMES
#        define STC_VEC_ENABLE_SHORT_NAMES
#    endif
#    ifndef STC_UTF_ENABLE_SHORT_NAMES
#        define STC_UTF_ENABLE_SHORT_NAMES
#    endif
#endif /* STC_ENABLE_SHORT_NAMES */

#ifndef STC_DISABLE_COMMON
#    include "common.h"

#    ifndef STC_DISABLE_MATH
#        include "math.h"
#    endif
#endif /* STC_DISABLE_COMMON */

#ifndef STC_DISABLE_FATP
#    ifndef STC_DISABLE_SLICE
#        include "fatp/slice.h"
#    endif
#    ifndef STC_DISABLE_SV
#        include "fatp/string_view.h"
#    endif
#    ifndef STC_DISABLE_VEC
#        include "fatp/vec.h"
#    endif
#endif /* STC_DISABLE_FATP */

#ifndef STC_DISABLE_MEM
#    include "memory/base.h"
#    ifndef STC_DISABLE_MEM_LIBC
#        include "memory/libc.h"
#    endif
#endif /* STC_DISABLE_MEM */

#ifndef STC_DISABLE_UTIL
#    ifndef STC_DISABLE_UTF
#        include "util/utf.h"
#    endif
#endif /* STC_DISABLE_UTIL */

#endif /* STC_API_H */
