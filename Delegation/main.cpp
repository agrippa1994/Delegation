#include <iostream>
#include <string>
#include <type_traits>

#define optional { }
#define required

template<class T>
class Interface
{
	T *delegate_ = nullptr;

public:
	Interface(T *del) : delegate_(del) { }

	bool valid() const
	{
		return (bool)delegate_;
	}

	operator bool()
	{
		return valid();
	}

	T *const delegate() const
	{
		return delegate_;
	}

	void setDelegate(T *p)
	{
		delegate_ = p;
	}
};

struct GPSDelegate
{
	void onPos(float fx, float fy, float fz) required;
	void onLocation(const std::string& s) optional;
	void onLocationChange(const std::string& strOld, const std::string& strNew) optional;
};

template<class T>
class GPS : public Interface < T >
{
public:
	GPS(T *t) : Interface<T>(t)
	{
		call(234.34f, 656.4f, 2947.396f);
	}

	void call(float fx, float fy, float fz)
	{
		delegate()->onPos(fx, fy, fz);
		delegate()->onLocation("LA");
		delegate()->onLocationChange("NY", "LA");
	}
};



class Program : public GPSDelegate
{
	GPS<Program> gps_;
	friend decltype(gps_);

public:
	Program() : gps_(this)
	{
	}

private:
	void onPos(float fx, float fy, float fz)
	{
		std::cout << fx << ' ' << fy << ' ' << fz << std::endl;
	}
	void onLocationChange(const std::string& strOld, const std::string& strNew)
	{
		std::cout << "Location change from " << strOld << " to " << strNew << std::endl;
	}
};


int main()
{
	Program p;
}