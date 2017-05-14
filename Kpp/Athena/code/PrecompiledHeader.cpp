#include "PrecompiledHeader.hpp"

#include "kv/predef/OS.hpp"
#include "kv/predef/Keyword.hpp"

#if defined(KV_OS_WINDOWS)

importlib("gdi32.lib");
importlib("kernel32.lib");
importlib("opengl32.lib");
importlib("user32.lib");

importlib("FreeImage.lib")
importlib("FreeImagePlus.lib")

#if defined(_DEBUG)
importlib("glew32sd.lib");
#else
importlib("glew32s.lib");
#endif

#endif


// external begin
#include "termcolor/termcolor.cpp"
#include "fmt/format.cc"
// external end