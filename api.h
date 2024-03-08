#ifndef STC_API_H
#define STC_API_H

#ifndef STC_DISABLE_COMMON
#    include "common.h"
#endif

#ifndef STC_DISABLE_MATH
#    include "math.h"
#endif

#ifndef STC_DISABLE_COLLECTION
#    ifndef STC_DISABLE_BTREE
#        include "collection/btree.h"
#    endif
#    ifndef STC_DISABLE_HASHMAP
#        include "collection/hashmap.h"
#    endif
#    ifndef STC_DISABLE_HASHSET
#        include "collection/hashset.h"
#    endif
#    ifndef STC_DISABLE_LINKEDLIST
#        include "collection/linkedlist.h"
#    endif
#    ifndef STC_DISABLE_MAP
#        include "collection/map.h"
#    endif
#    ifndef STC_DISABLE_SET
#        include "collection/set.h"
#    endif
#endif /* STC_DISABLE_COLLECTION */

#ifndef STC_DISABLE_FATP
#    ifndef STC_DISABLE_SLICE
#        include "fatp/slice.h"
#    endif
#    ifndef STC_DISABLE_STRING
#        include "fatp/string.h"
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
#    ifndef STC_DISABLE_ARGS
#        include "util/args.h"
#    endif
#    ifndef STC_DISABLE_UTF
#        include "util/utf.h"
#    endif
#endif /* STC_DISABLE_UTIL */

#endif /* STC_API_H */
