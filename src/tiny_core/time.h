/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#ifndef TIME_H
#define TIME_H

#include <boost/operators.hpp>
#include <string>

class Time : public boost::less_than_comparable<Time>
{
    public:
        Time()
            : microSecondsSinceEpoch_(0)
        {
        }

        explicit Time(int64_t microSecondsSinceEpoch);

        void swap(Time &that)
        {
            std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
        }
        std::string toString() const;
        std::string toFormattedString() const;
        bool valid() const { return microSecondsSinceEpoch_ > 0; }
        int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
        time_t secondsSinceEpoch() const
        {
            return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
        }
        static Time now();
        static Time invalid();

        static const int kMicroSecondsPerSecond = 1000 * 1000;

    private:
        int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Time lhs, Time rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Time lhs, Time rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Time high, Time low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Time::kMicroSecondsPerSecond;
}

inline Time addTime(Time time, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Time::kMicroSecondsPerSecond);
    return Time(time.microSecondsSinceEpoch() + delta);
}
#endif
