#include "PrecompiledHeader.hpp"

#include "kv/predef/keyword/library.hpp"
importlib("opengl32.lib");
importlib("FreeImage.lib")
importlib("FreeImagePlus.lib")
#if defined(_DEBUG)
importlib("glew32sd.lib");
#else
importlib("glew32s.lib");
#endif

// external begin
#include "termcolor/termcolor.cpp"
#include "fmt/format.cc"
// external end