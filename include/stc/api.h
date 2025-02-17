#ifndef STC_API_H
#define STC_API_H

#ifndef STC_DISABLE_COMMON
#    include <stc/common.h>
#endif

#ifndef STC_DISABLE_MATH
#    include <stc/math.h>
#endif

#ifndef STC_DISABLE_COLLECTION
#    ifndef STC_DISABLE_BTREE
#        include <stc/collection/btree.h>
#    endif
#    ifndef STC_DISABLE_LINKEDLIST
#        include <stc/collection/linkedlist.h>
#    endif
#    ifndef STC_DISABLE_MAP
#        include <stc/collection/map.h>
#    endif
#    ifndef STC_DISABLE_SET
#        include <stc/collection/set.h>
#    endif
#endif /* STC_DISABLE_COLLECTION */

#ifndef STC_DISABLE_FATP
#    ifndef STC_DISABLE_SLICE
#        include <stc/fatp/slice.h>
#    endif
#    ifndef STC_DISABLE_STRING
#        include <stc/fatp/string.h>
#    endif
#    ifndef STC_DISABLE_SV
#        include <stc/fatp/str_view.h>
#    endif
#    ifndef STC_DISABLE_VEC
#        include <stc/fatp/vec.h>
#    endif
#endif /* STC_DISABLE_FATP */

#ifndef STC_DISABLE_MEM
#    include <stc/memory/base.h>
#    ifndef STC_DISABLE_MEM_LIBC
#        include <stc/memory/libc.h>
#    endif
#endif /* STC_DISABLE_MEM */

#ifndef STC_DISABLE_UTIL
#    ifndef STC_DISABLE_ARGPARSER
#        include <stc/util/argparser.h>
#    endif
#    ifndef STC_DISABLE_UTF
#        include <stc/util/utf.h>
#    endif
#endif /* STC_DISABLE_UTIL */

#endif /* STC_API_H */
