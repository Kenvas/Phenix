#pragma once

#define KVTEXTA(quoted) quoted
#define KVTEXTW(quoted) L ## quoted
#define KVTEXTU8(quoted) u8 ## quoted
#define KVTEXTU16(quoted) u ## quoted
#define KVTEXTU32(quoted) U ## quoted

#define KVRAWTEXTA(quoted) R ## "(" quoted ")"
#define KVRAWTEXTW(quoted) LR ## "(" quoted ")"
#define KVRAWTEXTU8(quoted) u8R ## "(" quoted ")"
#define KVRAWTEXTU16(quoted) uR ## "(" quoted ")"
#define KVRAWTEXTU32(quoted) UR ## "(" quoted ")"

using char8_t  = char            ;
using char8s   = char8_t        *;
using cchar8s  = char8_t  const *;
using char16s  = char16_t       *;
using cchar16s = char16_t const *;
using char32s  = char32_t       *;
using cchar32s = char32_t const *;
using wchars   = wchar_t        *;
using cwchars  = wchar_t  const *;

#if defined(UNICODE) || defined(_UNICODE)
using xchar = wchar_t;
#define KVTEXT(quoted) KVTEXTW(quoted)
#define KVRAWTEXT(quoted) KVRAWTEXTW(quoted)
#else
using xchar = char8_t;
#define KVTEXT(quoted) KVTEXTA(quoted)
#define KVRAWTEXT(quoted) KVRAWTEXTA(quoted)
#endif
using xchars  = xchar       *;
using cxchars = xchar const *;