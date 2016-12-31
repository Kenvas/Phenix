#pragma once

#include <cassert>
#include <chrono>

using namespace std::chrono_literals;

namespace fnx
{
	class Stopwatch final
	{
	public:
		using Clock     = std::chrono::high_resolution_clock;
		using TimePoint = Clock::time_point;
		using Duration  = Clock::duration;
	private:
		struct RAII;
		TimePoint StartTime_;
		TimePoint StopTime_;
	public:
		void Start() noexcept
		{
			StartTime_ = Clock::now();
		}
		void Stop() noexcept
		{
			StopTime_ = Clock::now();
		}
		template <class TargetType = decltype(0.0ms)>
		auto GetDuration(TargetType const & value = TargetType(1)) const noexcept
		{
			using ReturnType  = float;
			auto const num    = TargetType::period::num;
			auto const den    = TargetType::period::den;
			auto const scale  = 1.0e-9 * den / num;
			auto const retval = std::chrono::duration_cast<decltype(1ns)>(StopTime_ - StartTime_).count() * scale;
			return ReturnType(retval);
		}
		template <class ReturnType, class ... ArgTypes>
		auto Run(ReturnType(* function)(ArgTypes ...), ArgTypes && ... args)
		{
			assert(function != nullptr);
			auto watch = RAII(this);
			return function(std::forward<ArgTypes>(args) ...);
		}
		template <class ObjectType, class ClassType, class ReturnType, class ... ArgTypes>
		auto Run(ObjectType * const object, ReturnType(ClassType::* function)(ArgTypes ...), ArgTypes && ... args)
		{
			assert(object != nullptr);
			assert(function != nullptr);
			auto watch = RAII(this);
			return (object->*function)(std::forward<ArgTypes>(args) ...);
		}
		template <class ObjectType, class ClassType, class ReturnType, class ... ArgTypes>
		auto Run(ObjectType & object, ReturnType(ClassType::* function)(ArgTypes ...), ArgTypes && ... args)
		{
			return Run(&object, function, std::forward<ArgTypes>(args) ...);
		}
		template <class ObjectType, class ClassType, class ReturnType, class ... ArgTypes>
		auto Run(ObjectType const * const object, ReturnType(ClassType::* function)(ArgTypes ...) const, ArgTypes && ... args)
		{
			assert(object != nullptr);
			assert(function != nullptr);
			auto watch = RAII(this);
			return (object->*function)(std::forward<ArgTypes>(args) ...);
		}
		template <class ObjectType, class ClassType, class ReturnType, class ... ArgTypes>
		auto Run(ObjectType const & object, ReturnType(ClassType::* function)(ArgTypes ...) const, ArgTypes && ... args)
		{
			return Run(&object, function, std::forward<ArgTypes>(args) ...);
		}
	private:
		struct RAII final
		{
			Stopwatch * const Watch;
			RAII(decltype(Watch) watch) noexcept
				: Watch(watch)
			{
				Watch->Start();
			}
			~RAII() noexcept
			{
				Watch->Stop();
			}
		};
	};


}