#pragma once

#include <cstdint>

#include <phenix/Thunk.h>
#include <phenix/Util.h>

namespace fnx
{
    enum class CallType
    {
        CDecl,
        StdCall,
        FastCall,
        ThisCall,
    };

    class DelegateObject
    {
    protected:
        uintptr_t ObjectValue_;
        uintptr_t FunctionValue_;
        CallType  CallType_;
    public:
        DelegateObject(uintptr_t object_value, uintptr_t function_value, CallType call_type = CallType::ThisCall)
            : ObjectValue_(object_value)
            , FunctionValue_(function_value)
            , CallType_(call_type)
        { }
        DelegateObject(uintptr_t function_value, CallType call_type = CallType::CDecl)
            : DelegateObject(uintptr_t(0), function_value, call_type)
        { }
        //public:
        //	auto const GetCallType() const { return CallType_; }
        //	auto const GetObjectValue() const { return ObjectValue_; }
        //	auto const GetFunctionValue() const { return FunctionValue_; }
    };

    template <class FunctionType>
    class Delegate;

    template <class ReturnType, class ... ArgTypes>
    class Delegate<ReturnType(ArgTypes ...)> : public DelegateObject
    {
    public:
        Delegate(uintptr_t object_value, uintptr_t function_value, CallType call_type)
            : DelegateObject(object_value, function_value, call_type)
        { }
    public:

        constexpr auto operator()(ArgTypes && ... args) const
        {
            return Invoke(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto Invoke(ArgTypes && ... args) const
        {
            return (ObjectValue_ != 0)
                ? InvokeMemberFunction(std::forward<ArgTypes>(args) ...)
                : InvokeFreeFunction(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeFreeFunction(ArgTypes && ... args) const
        {
            return (CallType_ == CallType::CDecl)
                ? InvokeFreeFunction__cdecl(std::forward<ArgTypes>(args) ...)
                : (CallType_ == CallType::StdCall)
                ? InvokeFreeFunction__stdcall(std::forward<ArgTypes>(args) ...)
                : (CallType_ == CallType::FastCall)
                ? InvokeFreeFunction__fastcall(std::forward<ArgTypes>(args) ...)
                : InvokeFreeFunction__cdecl(std::forward<ArgTypes>(args) ...);
            //switch (CallType_)
            //{
            //default                     :
            //case fnx::CallType::CDecl   : return InvokeFreeFunction__cdecl   (std::forward<ArgTypes>(args) ...);
            //case fnx::CallType::StdCall : return InvokeFreeFunction__stdcall (std::forward<ArgTypes>(args) ...);
            //case fnx::CallType::FastCall: return InvokeFreeFunction__fastcall(std::forward<ArgTypes>(args) ...);
            //}
        }

        constexpr auto InvokeMemberFunction(ArgTypes && ... args) const
        {
            return (CallType_ == CallType::CDecl)
                ? InvokeMemberFunction__cdecl(std::forward<ArgTypes>(args) ...)
                : (CallType_ == CallType::StdCall)
                ? InvokeMemberFunction__stdcall(std::forward<ArgTypes>(args) ...)
                : (CallType_ == CallType::FastCall)
                ? InvokeMemberFunction__fastcall(std::forward<ArgTypes>(args) ...)
                : InvokeMemberFunction__thiscall(std::forward<ArgTypes>(args) ...);
            //switch (CallType_)
            //{
            //case fnx::CallType::CDecl   : return InvokeMemberFunction__cdecl   (std::forward<ArgTypes>(args) ...);
            //case fnx::CallType::StdCall : return InvokeMemberFunction__stdcall (std::forward<ArgTypes>(args) ...);
            //case fnx::CallType::FastCall: return InvokeMemberFunction__fastcall(std::forward<ArgTypes>(args) ...);
            //default                     :
            //case fnx::CallType::ThisCall: return InvokeMemberFunction__thiscall(std::forward<ArgTypes>(args) ...);
            //}
        }
    private:

        constexpr auto InvokeMemberFunction__cdecl(ArgTypes && ... args) const
        {
            using ThisType = Delegate *;
            using InvokeType = ReturnType(__cdecl Delegate::*)(ArgTypes ...);
            return (force_cast<ThisType>(ObjectValue_)->*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeMemberFunction__stdcall(ArgTypes && ... args) const
        {
            using ThisType = Delegate *;
            using InvokeType = ReturnType(__stdcall Delegate::*)(ArgTypes ...);
            return (force_cast<ThisType>(ObjectValue_)->*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeMemberFunction__fastcall(ArgTypes && ... args) const
        {
            using ThisType = Delegate *;
            using InvokeType = ReturnType(__fastcall Delegate::*)(ArgTypes ...);
            return (force_cast<ThisType>(ObjectValue_)->*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeMemberFunction__thiscall(ArgTypes && ... args) const
        {
            using ThisType = Delegate *;
            using InvokeType = ReturnType(__thiscall Delegate::*)(ArgTypes ...);
            return (force_cast<ThisType>(ObjectValue_)->*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeFreeFunction__cdecl(ArgTypes && ... args) const
        {
            using InvokeType = ReturnType(__cdecl *)(ArgTypes ...);
            return (*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeFreeFunction__stdcall(ArgTypes && ... args) const
        {
            using InvokeType = ReturnType(__stdcall *)(ArgTypes ...);
            return (*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }

        constexpr auto InvokeFreeFunction__fastcall(ArgTypes && ... args) const
        {
            using InvokeType = ReturnType(__fastcall *)(ArgTypes ...);
            return (*force_cast<InvokeType>(FunctionValue_))(std::forward<ArgTypes>(args) ...);
        }
    };

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const * const object_pointer, ReturnType(__cdecl ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(object_pointer), force_cast<uintptr_t>(function_pointer), CallType::CDecl);
    //}

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const * const object_pointer, ReturnType(__stdcall ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(object_pointer), force_cast<uintptr_t>(function_pointer), CallType::StdCall);
    //}

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const * const object_pointer, ReturnType(__fastcall ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(object_pointer), force_cast<uintptr_t>(function_pointer), CallType::FastCall);
    //}

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const * const object_pointer, ReturnType(__thiscall ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(object_pointer), force_cast<uintptr_t>(function_pointer), CallType::ThisCall);
    //}

    template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    constexpr auto const delegate(ObjectType const & object, ReturnType(__cdecl ClassType::* const function_pointer)(ArgTypes ...))
    {
        return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(&object), force_cast<uintptr_t>(function_pointer), CallType::CDecl);
    }

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const & object, ReturnType(__stdcall ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(&object), force_cast<uintptr_t>(function_pointer), CallType::StdCall);
    //}

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const & object, ReturnType(__fastcall ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(&object), force_cast<uintptr_t>(function_pointer), CallType::FastCall);
    //}

    //template <class ObjectType, class ReturnType, class ClassType, class ... ArgTypes>
    //constexpr auto const delegate(ObjectType const & object, ReturnType(__thiscall ClassType::* const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(force_cast<uintptr_t>(&object), force_cast<uintptr_t>(function_pointer), CallType::ThisCall);
    //}

    template <class ReturnType, class ... ArgTypes>
    constexpr auto const delegate(ReturnType(__cdecl * const function_pointer)(ArgTypes ...))
    {
        return Delegate<ReturnType(ArgTypes ...)>(0, force_cast<uintptr_t>(function_pointer), CallType::CDecl);
    }

    //template <class ReturnType, class ... ArgTypes>
    //constexpr auto const delegate(ReturnType(__stdcall * const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(0, force_cast<uintptr_t>(function_pointer), CallType::StdCall);
    //}

    //template <class ReturnType, class ... ArgTypes>
    //constexpr auto const delegate(ReturnType(__fastcall * const function_pointer)(ArgTypes ...))
    //{
    //	return Delegate<ReturnType(ArgTypes ...)>(0, force_cast<uintptr_t>(function_pointer), CallType::FastCall);
    //}

    template <class FunctionType, class LambdaType>
    Delegate<FunctionType> const delegate(LambdaType const & lambda_function)
    {
        static char storage[sizeof(LambdaType)];
        std::memcpy(storage, &lambda_function, sizeof(LambdaType));
        return Delegate<FunctionType>(0, force_cast<uintptr_t>(storage), CallType::CDecl);
    }
}
