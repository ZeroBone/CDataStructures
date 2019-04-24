#include <iostream>
#include <fstream>
#include <random>

int main() {

    std::string outputFileName;

    std::cout << "Enter output file name:" << std::endl;
    std::cin >> outputFileName;

    unsigned long numberCount;
    std::cout << "Enter number count:" << std::endl;
    std::cin >> numberCount;

    std::ofstream file(outputFileName);

    std::cout << "Emitting " << numberCount << " numbers..." << std::endl;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(10000000, 99999999);

    for (unsigned long i = 0; i < numberCount; i++) {

        file << distribution(generator);
        file << ' ';

    }

    file.close();

    system("pause");

    return 0;

}