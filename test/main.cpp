// MIT License, Copyright (c) 2016 Felix Laufer

#define DEBUG 1
#include "../ButterworthLP.h"
#include <iostream>

int main()
{
    std::cout << "SOS of ButterworthLP(2400, 80, 11):\n\n";

    ButterworthLP bw = ButterworthLP(2400, 80, 11);
	
    std::cout << "\n##############################################\n";

    bw.stepInitialization(0);

    size_t size = 400;

    std::cout << "\nStep response for x = ones(1, " << size << "):\n\ny = [";
    for (size_t i = 0; i < size; ++i)
    {
        std::string whiteSpace = (i < size - 1) ? " " : "";
        std::cout << bw.process(1) << whiteSpace;
    }
    std::cout << "]\n\nMATLAB: plot(y)\n";
	
    /**
    Offline forward-backward filtering with [1, 10]-random input signal
    */

    double* input = new double[size];
    double* output = new double[size];

    for (size_t i = 0; i < size; ++i)
    {
        input[i] = rand() % 10 + 1;
    }

    bw.filter(input, output, size, 5.5, true);

    std::cin.get();

    return 0;
}
