// size.cc

#include <array>        // array
#include <chrono>       // clock
#include <iomanip>      // setw
#include <iostream>     // cout
#include <vector>
using namespace std::chrono;

const unsigned GAP = 12;
const unsigned REP = 10;
const unsigned STEPS = 16 * 512 * 2*1024; // 16M steps

std::array<int, STEPS> bytes; // STEPS x 4 bytes = 16M x 4B = 64MB

int main()
{
	std::cout << "#" 
	          << std::setw(GAP - 1) << "size (B)"
	          << std::setw(GAP    ) << "time (ns)"
	          << std::endl;

	bytes.fill(0);

	for (unsigned size = 1024 / sizeof(int); size <= STEPS; size <<= 1)
	{
		std::vector<unsigned> sec(REP);
	
		unsigned mod = size - 1;

		for (auto &s: sec)
		{
			auto start = high_resolution_clock::now();

			//completar aqui...
            for(unsigned int i = 0; i < bytes.size();i=i+mod){
                bytes[i]++;
            }
			auto stop = high_resolution_clock::now();
			//Para que se aprecie la representacion
			// ha sido necesario poner nanoseconds
			s = duration_cast<nanoseconds>(stop - start).count();
		}

		std::cout << std::setw(GAP) << size * sizeof(int)
		          << std::setw(GAP) << std::fixed << std::setprecision(1)
		          << std::setw(GAP) << *std::min(sec.begin(), sec.end())
		          << std::endl;
	}

	return bytes.front() + bytes.back();
}
