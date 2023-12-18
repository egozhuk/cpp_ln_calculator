#include "LN.h"
#include "return_codes.h"

#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#define ll long long

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Invalid number of parameters" << std::endl;
		return ERROR_PARAMETER_INVALID;
	}

	std::stack< LN > stack;
	std::string operation;
	std::ifstream input(argv[1]);
	if (!input.is_open())
	{
		std::cerr << "Can't open input file!" << std::endl;
		return ERROR_CANNOT_OPEN_FILE;
	}
	try
	{
		while (std::getline(input, operation))
		{
			if (operation == "+")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a + b);
			}
			else if (operation == "-")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a - b);
			}
			else if (operation == "*")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a * b);
			}
			else if (operation == "/")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a / b);
			}
			else if (operation == "%")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a % b);
			}
			else if (operation == "+=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a += b);
			}
			else if (operation == "-=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a -= b);
			}
			else if (operation == "*=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a *= b);
			}
			else if (operation == "/=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a /= b);
			}
			else if (operation == "%=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a %= b);
			}
			else if (operation == "==")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a == b);
			}
			else if (operation == "!=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a != b);
			}
			else if (operation == ">=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a >= b);
			}
			else if (operation == "<=")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a <= b);
			}
			else if (operation == ">")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a > b);
			}
			else if (operation == "<")
			{
				auto a = stack.top();
				stack.pop();
				auto b = stack.top();
				stack.pop();
				stack.emplace(a < b);
			}
			else if (operation == "~")
			{
				auto a = stack.top();
				stack.pop();
				stack.emplace(~a);
			}
			else if (operation == "_")
			{
				auto a = stack.top();
				stack.pop();
				stack.emplace(-a);
			}
			else
			{
				stack.emplace(LN(operation));
			}
		}
	} catch (std::runtime_error error)
	{
		std::cerr << error.what() << std::endl;
		return ERROR_DATA_INVALID;
	} catch (std::bad_alloc error)
	{
		std::cerr << "Memory allocation failed" << std::endl;
		return ERROR_OUT_OF_MEMORY;
	}
	input.close();
	std::ofstream output(argv[2]);
	if (!output.is_open())
	{
		std::cerr << "Can't open input file!" << std::endl;
		return ERROR_CANNOT_OPEN_FILE;
	}
	uint32_t size = stack.size();
	for (int i = 0; i < size; ++i)
	{
		char *answer = LN::getResult(stack.top());
		stack.pop();
		output << answer << std::endl;
		delete[] answer;
	}
	output.close();
	return SUCCESS;
}
