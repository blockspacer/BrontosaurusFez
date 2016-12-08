#pragma once

namespace CU
{
	typedef float TimeUnit; // is this wierd? // no mvh carl :)

	class Time
	{
	public:
		Time(const TimeUnit someTime);
		~Time();

		inline void Reset();
		Time operator+(const Time& aRight) const;
		Time& operator+=(const Time& aRight);

		TimeUnit GetHours() const;
		TimeUnit GetMinutes() const;
		TimeUnit GetSeconds() const;
		TimeUnit GetMilliseconds() const;
		TimeUnit GetMicroseconds() const;
		
	private:
		TimeUnit myTime;
	};

	inline void Time::Reset()
	{
		myTime = 0.f;
	}

	inline Time Time::operator+(const Time& aRight) const
	{
		return Time(*this) += aRight;
	}

	inline Time& Time::operator+=(const Time& aRight)
	{
		myTime += aRight.myTime;
		return *this;
	}

	inline TimeUnit Time::GetHours() const
	{
		return 0;//myTime / (60000000 * 60);
	}

	inline TimeUnit Time::GetMinutes() const
	{
		return myTime / 60000000;
	}

	inline TimeUnit Time::GetSeconds() const
	{
		return myTime / 1000000;
	}

	inline TimeUnit Time::GetMilliseconds() const
	{
		return myTime / 1000;
	}

	inline TimeUnit Time::GetMicroseconds() const
	{
		return myTime;
	}
}  