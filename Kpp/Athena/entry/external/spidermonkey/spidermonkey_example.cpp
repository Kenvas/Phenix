#include "PrecompiledHeader.hpp"
//#include "kv/entry/IncludeAll.hpp"
//#include "kv/log/IncludeAll.hpp"
//
//#define __STDC_LIMIT_MACROS
//#include <stdint.h>
//
//#include "jsapi.h"
//#include "js/Initialization.h"
//#include "js/Conversions.h"
//
//using namespace kv;
//
//static JSClass global_class
//{
//    "global",
//    JSCLASS_GLOBAL_FLAGS,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    nullptr,
//    JS_GlobalObjectTraceHook,
//};
//
//static bool
//println(JSContext* cx, unsigned argc, JS::Value* vp)
//{
//    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
//
//    for (unsigned i = 0; i < args.length(); i++) {
//        JS::RootedString str(cx, JS::ToString(cx, args[i]));
//        if (!str) return false;
//        char* bytes = JS_EncodeStringToUTF8(cx, str);
//        if (!bytes) return false;
//        fprintf(stdout, "%s%s", i ? " " : "", bytes);
//        JS_free(cx, bytes);
//    }
//
//    fputc('\n', stdout);
//    fflush(stdout);
//
//    args.rval().setUndefined();
//    return true;
//}
//
//static JSFunctionSpec myjs_global_functions[] = {
//    JS_FS("print", &println, 0, 0),
//    JS_FS_END
//};
//
//KV_QuickAddEntry
//{
//    JS_Init();
//
//    auto runtime = JS_NewRuntime(8U * 1024 * 1024);
//    if (runtime == nullptr) return 1;
//
//    auto context = JS_NewContext(runtime, 8192);
//    if (context == nullptr) return 1;
//
//    { // Scope for our various stack objects (JSAutoRequest, RootedObject), so they all go
//      // out of scope before we JS_DestroyContext.
//
//        auto auto_request = JSAutoRequest(context);
//        auto& global = JS::RootedObject(context, JS_NewGlobalObject(context, &global_class, nullptr, JS::FireOnNewGlobalHook));
//        if (global == nullptr) return 1;
//
//        auto& rval = JS::RootedValue(context);
//
//        { // Scope for JSAutoCompartment
//            auto auto_compartment = JSAutoCompartment(context, global);
//            JS_InitStandardClasses(context, global);
//
//            //auto script = "'hello'+' world, it is '+new Date()";
//            //auto filename = "noname";
//            //auto lineno = 1;
//            //auto& options = JS::CompileOptions (context);
//            //options.setFileAndLine(filename, lineno);
//            //auto ok = JS::Evaluate(context, options, script, strlen(script), &rval);
//            if (!JS_DefineFunctions(context, global, myjs_global_functions))
//                return 1;
//
//            auto filename = "E:/Workspace/Phenix/Kpp/Athena/bin/test.js";
//            auto& script = JS::RootedScript(context);
//            //JS::Compile(context, )
//            auto& options = JS::CompileOptions (context);
//            options.setFile(filename)
//                .setIsRunOnce(true);
//            auto ok = JS::Compile(context, options, filename, &script);
//            if (!ok) return 1;
//            if (!JS_ExecuteScript(context, script, &rval)) return 1;
//        }
//        if (rval.isString())
//        {
//            auto str = rval.toString();
//            log::debug("{0}", JS_EncodeString(context, str))();
//        }
//        else
//        {
//            log::debug("script finished.")();
//        }
//    }
//
//
//    JS_DestroyContext(context);
//    JS_DestroyRuntime(runtime);
//    JS_ShutDown();
//    return 0;
//};