#include <iostream>
#include <string>
#include <cstdint>
#include <array>

// NOT FINISHED
// NOT TESTED
// NOT THE STANDARD (yet...)

namespace GORBITSA
{
	enum class Arch { RAM, ROM };

	template<typename T = uint8_t, Arch A = Arch::ROM, size_t RAMSIZE = 256, size_t ROMSIZE = 256>
	class Machine
	{
	public:
		Machine()
		{
			static_assert(std::is_integral<T>::value, "Integral Type Required");
		}

		bool ExecuteProgram(const std::string& program, const std::string& input)
		{
			if constexpr (A == Arch::ROM)
			{
				// Populate ROM with commands
			}
			else
			{
				// Populate RAM with commands
			}

			if (!Step()) Error();
		}

		bool Step(size_t steps = 1)
		{
			bool valid = true;
			while (steps-- > 0 && valid)
			{
				if constexpr (A == Arch::ROM)
					valid = Compute(static_cast<uint8_t>(rom[pc * 2 + 0] & 0xFF), rom[pc * 2 + 1]);
				else
					valid = Compute(static_cast<uint8_t>(ram[pc * 1 + 0] & 0xFF), ram[pc * 1 + 1]);
			}

			return valid;			
		}

	private:
		bool Compute(const uint8_t opcode, const T operand)
		{
			constexpr T inc = A == Arch::ROM ? 1 : 2;
			constexpr size_t maxaddr = A == Arch::ROM ? ROMSIZE : RAMSIZE;
			auto& m = ram[operand];

			switch (opcode)
			{
			case 'G': x = m;						break;
			case 'O': m = x;						break;
			case 'R': x = input();					break;
			case 'B': pc = !x ? operand : pc + inc; break;
			case 'I': x = x + operand;				break;
			case 'T': output(x);					break;
			case 'S': x = operand;					break;
			case 'A': x = x + m;					break;
			case 'g': x = ram[m];					break;
			case 'o': ram[m] = x;					break;
			case 'r': m = input();					break;
			case 'b': pc = !x ? m : pc + inc;		break;
			case 'i': m = m + x; 					break;
			case 't': output(m);					break;
			case 's': x = x ^ m;					break;
			case 'a': x = x + ram[m];				break;
			default: return false;					break;
			}

			if (opcode != 'B' && opcode != 'b') pc = pc + inc;
			if (pc >= maxaddr) return false;
			
			pc %= maxaddr;
			return true;
		}

	protected:
		virtual void output(const T n)
		{
			std::cout << std::to_string(n);
		}

		virtual T input()
		{
			return 0;
		}

		virtual void Error()
		{
			std::cout << "Terminated! Invalid Opcode Addr:" << std::to_string(pc) << std::endl;
		}

	private:
		std::array<T, RAMSIZE * 1> ram{ 0 };
		std::array<T, ROMSIZE * 2> rom{ 0 };
		size_t pc = 0;
		T x = 0;	
	};
}

int main()
{
	GORBITSA::Machine vm;
	vm.ExecuteProgram("R O201 R O202 S0 O203 G202 A202 O202 G201 I255 O201 B6 G202 T", "15, 12");
	return 0;
}
