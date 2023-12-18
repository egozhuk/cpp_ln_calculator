#pragma once
#include <string_view>

#define ll long long

class LN
{
	uint16_t* digits;
	int size;
	bool isNaN;
	bool is_negative = false;

  public:
	LN(ll val = 0);
	LN(const char* str);
	LN(const std::string_view str);
	LN(const LN& other);
	LN(LN&& other);
	~LN();

	LN& operator=(const LN& other);
	LN& operator=(LN&& other);
	LN& operator+=(const LN& b);
	LN& operator-=(const LN& b);
	LN& operator*=(const LN& b);
	LN& operator/=(const LN& b);
	LN& operator%=(const LN& b);

	bool operator==(const LN& b) const;
	bool operator!=(const LN& b) const;
	bool operator<(const LN& b) const;
	bool operator>(const LN& b) const;
	bool operator<=(const LN& b) const;
	bool operator>=(const LN& b) const;

	LN operator+(const LN& b) const;
	LN operator-(const LN& b) const;
	LN operator*(const LN& b) const;
	LN operator/(const LN& b);
	LN operator%(const LN& b);
	LN operator~();
	LN operator-() const;

	operator long long();
	operator bool();

	static char* getResult(const LN& b);

  private:
	LN abs() const;
	LN middle(const LN& b) const;
};

LN operator"" _ln(const char* str);