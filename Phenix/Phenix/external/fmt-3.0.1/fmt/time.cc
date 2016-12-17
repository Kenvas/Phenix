#include "time.h"



namespace fmt {
    namespace internal{
        inline Null<> localtime_r(...) { return Null<>(); }
        inline Null<> localtime_s(...) { return Null<>(); }
        inline Null<> gmtime_r(...) { return Null<>(); }
        inline Null<> gmtime_s(...) { return Null<>(); }
    }

    std::tm localtime(std::time_t time)
    {
        struct LocalTime {
            std::time_t time_;
            std::tm tm_;

            LocalTime(std::time_t t) : time_(t) {}

            bool run() {
                using namespace fmt::internal;
                return handle(localtime_r(&time_, &tm_));
            }

            bool handle(std::tm *tm) { return tm != FMT_NULL; }

            bool handle(internal::Null<>) {
                using namespace fmt::internal;
                return fallback(localtime_s(&tm_, &time_));
            }

            bool fallback(int res) { return res == 0; }

            bool fallback(internal::Null<>) {
                //using namespace fmt::internal;
                //std::tm *tm = std::localtime(&time_);
                //if (tm) tm_ = *tm;
                //return tm != FMT_NULL;
                return fallback(localtime_s(&tm_, &time_));
            }
        };
        LocalTime lt(time);
        if (lt.run())
            return lt.tm_;
        // Too big time values may be unsupported.
        FMT_THROW(fmt::FormatError("time_t value out of range"));
        return std::tm();
    }

    std::tm gmtime(std::time_t time)
    {
        struct GMTime {
            std::time_t time_;
            std::tm tm_;

            GMTime(std::time_t t) : time_(t) {}

            bool run() {
                using namespace fmt::internal;
                return handle(gmtime_r(&time_, &tm_));
            }

            bool handle(std::tm *tm) { return tm != FMT_NULL; }

            bool handle(internal::Null<>) {
                using namespace fmt::internal;
                return fallback(gmtime_s(&tm_, &time_));
            }

            bool fallback(int res) { return res == 0; }

            bool fallback(internal::Null<>) {
                //std::tm *tm = std::gmtime(&time_);
                //if (tm != FMT_NULL) tm_ = *tm;
                //return tm != FMT_NULL;
                return fallback(gmtime_s(&tm_, &time_));
            }
        };
        GMTime gt(time);
        if (gt.run())
            return gt.tm_;
        // Too big time values may be unsupported.
        FMT_THROW(fmt::FormatError("time_t value out of range"));
        return std::tm();
    }

}