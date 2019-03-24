#include <iostream>

template <class T>
void reverseArray(T* array, size_t length) {
    size_t i = 0;
    while (i < length / 2) {
        std::swap(array[i], array[length - i - 1]);
        i++;
    }
}

int main() {

    unsigned int uintarr[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    float floatarr[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    std::cout << "Unsigned int array:" << std::endl;

    for (unsigned int i : uintarr) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    std::cout << "Float array:" << std::endl;

    for (float i : floatarr) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    // reverseArray(uintarr, 10);
    // reverseArray(floatarr, 10);
    reverseArray<unsigned int>(uintarr, 11);
    reverseArray<float>(floatarr, 11);

    std::cout << "Unsigned int array:" << std::endl;

    for (unsigned int i : uintarr) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    std::cout << "Float array:" << std::endl;

    for (float i : floatarr) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    system("pause");

    return 0;
}