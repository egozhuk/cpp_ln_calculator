#include "LN.h"

#include <iostream>

LN::LN(long long val)
{
	size = 0;
	int length = 0;
	long long temp = val;

	if (temp == 0)
	{
		length = 1;
		size = 1;
	}
	else
	{
		if (temp < 0)
		{
			temp = -temp;
		}

		while (temp > 0)
		{
			temp /= 10;
			length++;
		}
	}
	size = length / 4;
	if (length % 4 > 0)
	{
		size++;
	}
	digits = new uint16_t[size];
	if (val < 0)
	{
		is_negative = true;
	}
	int c = 0;
	do
	{
		uint32_t group = val % 65536;
		digits[c++] = group;
		val /= 65536;
	} while (val > 0);
	isNaN = false;
}

LN::LN(const char *str) : LN(static_cast< std::string_view >(str)) {}

LN::LN(const std::string_view str)
{
	if (str == "")
	{
		throw std::runtime_error("Empty line");
	}
	int start = 0;
	if (str[0] == '-')
	{
		start++;
		is_negative = true;
	}
	else
	{
		is_negative = false;
	}
	int len_of_str = str.size();
	size = len_of_str / 4;
	if (len_of_str % 4 != 0)
	{
		size++;
	}
	int c = 0;
	isNaN = false;
	digits = new uint16_t[size];
	int cou = 0;
	for (int i = len_of_str; i >= start; i--)
	{
		int pow = -1;
		int curr_number = 0;
		for (int j = 3; j >= 0; j--)
		{
			if (i <= start)
			{
				break;
			}
			i--;
			int curr_num;

			char ch = str[i];
			if (ch >= 'A' && ch <= 'F')
			{
				ch += 32;
			}

			if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f')))
			{
				throw std::runtime_error("Invalid character in number");
			}

			switch (ch)
			{
			case 'a':
				curr_num = 10;
				break;
			case 'b':
				curr_num = 11;
				break;
			case 'c':
				curr_num = 12;
				break;
			case 'd':
				curr_num = 13;
				break;
			case 'e':
				curr_num = 14;
				break;
			case 'f':
				curr_num = 15;
				break;
			default:
				curr_num = ch - '0';
				break;
			}

			int mnozhitel = 16;
			for (int k = 0; k < pow; k++)
			{
				mnozhitel *= 16;
			}
			if (pow == -1)
			{
				mnozhitel = 1;
			}
			pow++;

			curr_number += curr_num * mnozhitel;
		}
		cou++;
		digits[c] = curr_number;
		c++;
		if (i <= start)
		{
			return;
		}
		i++;
	}
}

LN::LN(const LN &other)
{
	size = other.size;
	isNaN = other.isNaN;
	digits = new uint16_t[size];
	for (int i = 0; i < other.size; i++)
	{
		digits[i] = other.digits[i];
	}
	is_negative = other.is_negative;
}

LN::LN(LN &&other)
{
	size = other.size;
	isNaN = other.isNaN;
	is_negative = other.is_negative;
	digits = new uint16_t[size];
	for (int i = 0; i < other.size; i++)
	{
		digits[i] = other.digits[i];
	}
	delete[] other.digits;
	other.size = 0;
	other.digits = nullptr;
	other.is_negative = false;
}

LN::~LN()
{
	delete[] digits;
}

LN &LN::operator=(const LN &other)
{
	delete[] digits;
	if (this == &other)
	{
		return *this;
	}
	size = other.size;
	isNaN = other.isNaN;
	digits = new uint16_t[size];
	is_negative = other.is_negative;
	for (int i = 0; i < size; i++)
	{
		digits[i] = other.digits[i];
	}
	return *this;
}

LN &LN::operator=(LN &&other)
{
	delete[] digits;
	if (this == &other)
	{
		return *this;
	}
	size = other.size;
	isNaN = other.isNaN;
	is_negative = other.is_negative;
	digits = new uint16_t[size];
	for (int i = 0; i < size; i++)
	{
		digits[i] = other.digits[i];
	}
	other.size = 0;
	other.is_negative = false;
	delete[] other.digits;
	other.digits = nullptr;
	other.isNaN = false;
	return *this;
}

LN LN::abs() const
{
	LN result;
	result = *this;
	result.is_negative = false;
	return result;
}

LN LN::operator+(const LN &b) const
{
	LN result;
	if (isNaN || b.isNaN)
	{
		result.isNaN = true;
		return result;
	}
	int bigger = 0;
	int max_size;
	int min_size;
	if (size > b.size)
	{
		max_size = size;
		min_size = b.size;
		bigger = 1;
	}
	else if (size < b.size)
	{
		max_size = b.size;
		min_size = size;
		bigger = 2;
	}
	else
	{
		max_size = size;
		min_size = size;
	}

	delete[] result.digits;
	result.digits = new uint16_t[max_size + 1];
	result.size = max_size;

	if (is_negative != b.is_negative)
	{
		if (!is_negative && b.is_negative)
		{
			return *this - b.abs();
		}
		else
		{
			return -(abs() - b);
		}
	}
	else
	{
		result.is_negative = is_negative;
	}

	int carry = 0;
	for (int i = 0; i < min_size; i++)
	{
		long long a = digits[i] + b.digits[i] + carry;
		result.digits[i] = a % 65536;
		carry = a / 65536;
	}
	if (min_size < max_size)
	{
		for (int j = min_size; j < max_size; ++j)
		{
			if (bigger == 1)
			{
				result.digits[j] = digits[j];
			}
			else
			{
				result.digits[j] = b.digits[j];
			}
		}
	}

	if (carry == 1)
	{
		result.digits[max_size] = 1;
		result.size++;
	}

	return result;
}

LN LN::operator-(const LN &b) const
{
	LN result;
	if (isNaN || b.isNaN)
	{
		result.isNaN = true;
		return result;
	}
	int bigger = 0;
	int max_size;
	int min_size;
	if (size > b.size)
	{
		max_size = size;
		min_size = b.size;
		bigger = 1;
	}
	else if (size < b.size)
	{
		max_size = b.size;
		min_size = size;
		bigger = 2;
	}
	else
	{
		max_size = size;
		min_size = size;
	}
	delete[] result.digits;
	result.digits = new uint16_t[max_size + 1];
	result.size = max_size;

	if (is_negative != b.is_negative)
	{
		if (!is_negative && b.is_negative)
		{
			return *this + b.abs();
		}
		else
		{
			return -(abs() + b);
		}
	}

	if (abs() < b.abs())
	{
		return -(b - *this);
	}

	long long carry = 0;
	for (int i = 0; i < min_size; i++)
	{
		long long a = digits[i] - b.digits[i];
		if (a < 0)
		{
			carry++;
			result.digits[i] = a + 65536;
		}
		else
		{
			result.digits[i] = a - carry;
			if (carry > a)
			{
				result.digits[i] += 65536;
			}
			else
			{
				carry = 0;
			}
		}
	}

	if (min_size < max_size)
	{
		for (int j = min_size; j < max_size; ++j)
		{
			if (bigger == 1)
			{
				result.digits[j] = digits[j];
			}
			else
			{
				result.digits[j] = b.digits[j];
			}
		}
	}

	if (carry > 0)
	{
		result.digits[min_size] -= carry;
	}

	result.is_negative = is_negative;

	for (int i = max_size; i >= 0; i--)
	{
		if (result.digits[i] != '0')
		{
			break;
		}
		result.size--;
	}

	return result;
}

LN LN::operator*(const LN &b) const
{
	LN result;
	if (isNaN || b.isNaN)
	{
		result.isNaN = true;
		return result;
	}
	delete[] result.digits;
	result.size = size + b.size;
	result.digits = new uint16_t[result.size];
	for (int i = 0; i < result.size; i++)
	{
		result.digits[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		uint64_t carry = 0;
		for (int j = 0; j < b.size; j++)
		{
			uint64_t curr_product = static_cast< uint64_t >(digits[i]) * b.digits[j];
			curr_product += result.digits[i + j];
			curr_product += carry;
			result.digits[i + j] = curr_product % 65536;
			carry = curr_product / 65536;
		}
		result.digits[i + b.size] = carry;
	}

	int new_size = result.size;
	while (new_size > 1 && result.digits[new_size - 1] == 0)
	{
		new_size--;
	}
	result.size = new_size;

	result.is_negative = is_negative != b.is_negative;

	return result;
}

LN LN::middle(const LN &b) const
{
	LN result = b;
	bool one = false;
	for (size_t i = result.size - 1; i < result.size; i--)
	{
		long long curr = (!one ? 0 : 65536) + result.digits[i];
		result.digits[i] = curr / 2;
		one = curr % 2;
	}

	return result;
}

LN LN::operator/(const LN &b)
{
	LN left = LN((long long)0);
	if (isNaN || b.isNaN)
	{
		LN result;
		result.isNaN = true;
		return result;
	}
	if (b.size == 1 && b.digits[0] == 0)
	{
		left.isNaN = true;
		return left;
	}
	if (b == 1_ln)
	{
		return *this;
	}
	if (*this < b)
	{
		return 0_ln;
	}
	if (*this == b)
	{
		return 1_ln;
	}
	LN right = *this;
	LN one = LN("1");
	while (left < right - one)
	{
		LN mid = middle(left + right);
		if (mid * b <= *this)
		{
			left = mid;
		}
		else
		{
			right = mid;
		}
	}
	return left;
}

LN LN::operator%(const LN &b)
{
	if (isNaN || b.isNaN)
	{
		LN result;
		result.isNaN = true;
		return result;
	}
	if (b.size == 1 && b.digits[0] == 0)
	{
		LN result;
		result.isNaN = true;
		return result;
	}

	LN quotient = *this / b;
	LN remainder = *this - (static_cast< LN >(b) * static_cast< const LN >(quotient));

	if (remainder.is_negative)
	{
		remainder.is_negative = false;
	}

	return remainder;
}

LN LN::operator~()
{
	if (is_negative)
	{
		LN result;
		result.isNaN = true;
		return result;
	}

	if (size == 1 && digits[0] == 0)
	{
		return *this;
	}
	LN low;
	LN high = *this;

	while (low < high - 1_ln)
	{
		LN mid = middle(high + low);

		if (mid * mid <= *this)
		{
			low = mid;
		}
		else
		{
			high = mid;
		}
	}

	return low;
}

LN LN::operator-() const
{
	LN result = *this;
	result.is_negative = !result.is_negative;
	return result;
}

LN &LN::operator+=(const LN &b)
{
	return *this = *this + b;
}

LN &LN::operator-=(const LN &b)
{
	return *this = *this - b;
}

LN &LN::operator*=(const LN &b)
{
	return *this = *this * b;
}

LN &LN::operator/=(const LN &b)
{
	return *this = *this / b;
}

LN &LN::operator%=(const LN &b)
{
	return *this = *this % b;
}

bool LN::operator==(const LN &b) const
{
	if (is_negative != b.is_negative || size != b.size)
	{
		return false;
	}
	for (int i = 0; i < size; i++)
	{
		if (digits[i] != b.digits[i])
		{
			return false;
		}
	}
	return true;
}

bool LN::operator!=(const LN &b) const
{
	return !(*this == b);
}

bool LN::operator<(const LN &b) const
{
	if (isNaN || b.isNaN)
	{
		return false;
	}

	if (is_negative && !b.is_negative)
	{
		return true;
	}
	if (!is_negative && b.is_negative)
	{
		return false;
	}

	if (size < b.size)
	{
		return true;
	}
	if (size > b.size)
	{
		return false;
	}

	for (int i = size - 1; i >= 0; --i)
	{
		if (!is_negative)
		{
			if (digits[i] < b.digits[i])
			{
				return true;
			}
			if (digits[i] > b.digits[i])
			{
				return false;
			}
		}
		else if (is_negative)
		{
			if (digits[i] < b.digits[i])
			{
				return false;
			}
			if (digits[i] > b.digits[i])
			{
				return true;
			}
		}
	}
	return false;
}

bool LN::operator<=(const LN &b) const
{
	return (*this) < b || (*this) == b;
}

bool LN::operator>(const LN &b) const
{
	return !(*this <= b);
}

bool LN::operator>=(const LN &b) const
{
	return !(*this < b);
}

char *LN::getResult(const LN &b)
{
	char *ans = nullptr;
	ans = new char[b.size * 4 + 2];

	if (b.isNaN)
	{
		ans[0] = 'N';
		ans[1] = 'a';
		ans[2] = 'N';
		ans[3] = '\0';
		return ans;
	}

	int index = 0;

	if (b.digits[0] == 0 && b.size == 1)
	{
		ans[0] = '0';
		ans[1] = '\0';
		return ans;
	}

	if (b.is_negative)
	{
		ans[index++] = '-';
	}

	for (int i = b.size - 1; i >= 0; i--)
	{
		int tempIndex = 0;
		char *temp = new char[5];

		int x = b.digits[i];

		while (x != 0)
		{
			int remainder = x % 16;

			if (remainder < 10)
			{
				temp[tempIndex++] = remainder + '0';
			}
			else
			{
				temp[tempIndex++] = (remainder - 10) + 'A';
			}

			x /= 16;
		}

		if (tempIndex < 4 && i != b.size - 1)
		{
			for (int j = tempIndex; j < 4; ++j)
			{
				tempIndex++;
				temp[j] = '0';
			}
		}

		for (int j = 0; j < tempIndex / 2; ++j)
		{
			char t = temp[j];
			temp[j] = temp[tempIndex - j - 1];
			temp[tempIndex - j - 1] = t;
		}

		for (int j = 0; j < tempIndex; ++j)
		{
			ans[index++] = temp[j];
		}
		delete[] temp;
	}

	ans[index] = '\0';
	return ans;
}

LN::operator long long()
{
	long long result = 0;
	long long base = 1;
	const long long max_value = 9223372036854775807;

	for (int i = 0; i < size; i++)
	{
		long long part = static_cast< long long >(digits[i]);
		long long temp_result = result + part * base;

		if (part * base > max_value - result)
		{
			throw std::runtime_error("Number is too large for long long");
		}

		result = temp_result;
		base *= 10000;
	}

	if (is_negative)
	{
		result *= -1;
	}

	return result;
}

LN::operator bool()
{
	if (size == 0 || isNaN)
	{
		return false;
	}
	if (size == 1 && digits[0] == 0)
	{
		return false;
	}
	return true;
}

LN operator"" _ln(const char *str)
{
	return LN(str);
}