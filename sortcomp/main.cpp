#include <iostream>
#include <fstream>
#include <memory>
#include <cstdlib>

long long compares = 0;

void StraightInsertion(int* a, int n) {

    int i, j, x;

    for (i = 1; i < n; i++) {

        x = a[i];

        for (j = i; j && (compares++, x < a[j - 1]); j--) /* находим место вставки, попутно сдвигая элементы */
            a[j] = a[j - 1];

        a[j] = x;

    }

}

void StraightInsertionDesc(int* a, int n) {
    int i, j, x;
    for (i = 1; i < n; i++) {
        x = a[i];
        for (j = i; j && x > a[j - 1]; j--) /* находим место вставки, попутно сдвигая элементы */
            a[j] = a[j - 1];
        a[j] = x;
    }
}

void ShakerSort(int* a, int n) {
    int j, k = n - 1, left = 1, right = n - 1, x;
    do {
        for (j = right; j >= left; j--)    /*сначала просматриваем справа налево*/
            if ((compares++, a[j - 1]) > a[j]) {
                x = a[j - 1];
                a[j - 1] = a[j];
                a[j] = x;
                k = j;
            }
        left = k + 1;
        for (j = left; j <= right; j++)    /*а теперь просматриваем слева направо*/
            if ((compares++, a[j - 1]) > a[j]) {
                x = a[j - 1];
                a[j - 1] = a[j];
                a[j] = x;
                k = j;
            }
        right = k - 1;
    } while (left < right);    /*и так до тех пор, пока есть что просматривать*/
}

void sift(int* a, int L, int R) /*Просеивание сквозь пирамиду (вспомогательная функция)*/
{
    int i = L, j = 2 * L + 1, x = a[L];
    if (j < R && (compares++, a[j + 1]) > a[j])      /*если элемент j не последний в рассматр. последовательности*/
        j++;                  /*и правый брат больше левого, переставляем указатель на него*/
    while (j <= R && (compares++, a[j] > x))      /*просматриваем потомков до конца последовательности,*/
    {                         /*если они больше текущего (х) элемента, т.е. нарушена*/
        a[i] = a[j];        /*упорядоченность пирамиды, то восстанавливаем ее,*/
        i = j;                /*поднимая потомков на уровень выше*/
        j = 2 * j + 1;
        if (j < R && (compares++, a[j + 1]) > a[j])    /*и каждый раз выбираем для анализа большего из*/
            j++;            /*братьев*/
    }
    a[i] = x;
}


void HeapSort(int* a, int n) {
    int L = n / 2, R = n - 1, x;
    while (L > 0)
        sift(a, --L, R);
    while (R > 0) {
        x = a[0];
        a[0] = a[R];
        a[R] = x;
        sift(a, L, --R);
    }
}

void outputArray(int* arr, int length) {

    /*for (int i = 0; i < length; i++) {
        std::cout << arr[i] << ' ';
    }*/

    for (int i = 0; i < 3; i++) {
        std::cout << arr[i] << ' ';
    }

    std::cout << "... ";

    for (int i = length / 2 - 3; i < length / 2 + 3; i++) {
        std::cout << arr[i] << ' ';
    }

    std::cout << "... ";

    for (int i = length - 4; i < length; i++) {
        std::cout << arr[i] << ' ';
    }
}

void memCopy(int* dest, const int* src, int length) {
    for (int i = 0; i < length; i++) {
        dest[i] = src[i];
    }
}

void printScore(long long* scores) {

    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Non sorted:" << std::endl;
    std::cout << "Straight insertion sort: " << scores[0] << std::endl;
    std::cout << "Shaker sort: " << scores[1] << std::endl;
    std::cout << "Heap sort: " << scores[2] << std::endl;
    std::cout << "-----------------------------------" << std::endl;

}

typedef void (*sortFunc)(int* arr, int length);

int main() {

    int n4 = 150000;
    int* arrn4 = new int[n4];

    std::fstream file("f4.txt");

    int current;
    int i = 0;

    while (file >> current) {

        if (i > n4) {
            break;
        }

        arrn4[i++] = current;

    }

    sortFunc funcs[] = {StraightInsertion, ShakerSort, HeapSort};
    int funcCount = 3;

    int n1 = 10000, n2 = 50000, n3 = 100000;

    int* arrn1 = new int[n1];
    int* arrn2 = new int[n2];
    int* arrn3 = new int[n3];

    memCopy(arrn1, arrn4, n1);
    memCopy(arrn2, arrn4, n2);
    memCopy(arrn3, arrn4, n3);

    std::cout << "Initial arrays:" << std::endl;
    std::cout << "N4: "; outputArray(arrn4, n4); std::cout << std::endl;
    std::cout << "N3: "; outputArray(arrn3, n3); std::cout << std::endl;
    std::cout << "N2: "; outputArray(arrn2, n2); std::cout << std::endl;
    std::cout << "N1: "; outputArray(arrn1, n1); std::cout << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    // not sorted score

    long long nonSortedScores[funcCount];

    for (int f = 0; f < funcCount; f++) {

        std::cout << "1: Sorting with " << (f + 1) << " functions..." << std::endl;

        compares = 0;
        funcs[f](arrn1, n1);
        nonSortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn2, n2);
        nonSortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn3, n3);
        nonSortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn4, n4);
        nonSortedScores[f] = compares;

        std::cout << "1: Sorted with " << (f + 1) << " functions." << std::endl;

    }

    // output result

    std::cout << "NON SORTED => SORTED - score:" << std::endl;
    printScore(nonSortedScores);

    std::cout << "Sorted arrays:" << std::endl;
    std::cout << "N4: "; outputArray(arrn4, n4); std::cout << std::endl;
    std::cout << "N3: "; outputArray(arrn3, n3); std::cout << std::endl;
    std::cout << "N2: "; outputArray(arrn2, n2); std::cout << std::endl;
    std::cout << "N1: "; outputArray(arrn1, n1); std::cout << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    // sort already sorted array

    long long alreadySortedScores[funcCount];

    for (int f = 0; f < funcCount; f++) {

        std::cout << "2: Sorting with " << (f + 1) << " functions..." << std::endl;

        compares = 0;
        funcs[f](arrn1, n1);
        alreadySortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn2, n2);
        alreadySortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn3, n3);
        alreadySortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn4, n4);
        alreadySortedScores[f] = compares;

        std::cout << "2: Sorted with " << (f + 1) << " functions." << std::endl;

    }

    // output result

    std::cout << "SORTED => SORTED - score:" << std::endl;
    printScore(alreadySortedScores);

    std::cout << "Sorted arrays:" << std::endl;
    std::cout << "N4: "; outputArray(arrn4, n4); std::cout << std::endl;
    std::cout << "N3: "; outputArray(arrn3, n3); std::cout << std::endl;
    std::cout << "N2: "; outputArray(arrn2, n2); std::cout << std::endl;
    std::cout << "N1: "; outputArray(arrn1, n1); std::cout << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    // reverse arrays

    StraightInsertionDesc(arrn4, n4);
    StraightInsertionDesc(arrn3, n3);
    StraightInsertionDesc(arrn2, n2);
    StraightInsertionDesc(arrn1, n1);

    std::cout << "Reversed arrays:" << std::endl;
    std::cout << "N4: "; outputArray(arrn4, n4); std::cout << std::endl;
    std::cout << "N3: "; outputArray(arrn3, n3); std::cout << std::endl;
    std::cout << "N2: "; outputArray(arrn2, n2); std::cout << std::endl;
    std::cout << "N1: "; outputArray(arrn1, n1); std::cout << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    // sort reversed arrays

    long long reversedSortedScores[funcCount];

    for (int f = 0; f < funcCount; f++) {

        std::cout << "3: Sorting with " << (f + 1) << " functions..." << std::endl;

        compares = 0;
        funcs[f](arrn1, n1);
        reversedSortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn2, n2);
        reversedSortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn3, n3);
        reversedSortedScores[f] = compares;

        compares = 0;
        funcs[f](arrn4, n4);
        reversedSortedScores[f] = compares;

        std::cout << "3: Sorted with " << (f + 1) << " functions." << std::endl;

    }
    // output result


    std::cout << "SORTED REVERSED => SORTED - score:" << std::endl;
    printScore(reversedSortedScores);

    std::cout << "Sorted arrays:" << std::endl;
    std::cout << "N4: "; outputArray(arrn4, n4); std::cout << std::endl;
    std::cout << "N3: "; outputArray(arrn3, n3); std::cout << std::endl;
    std::cout << "N2: "; outputArray(arrn2, n2); std::cout << std::endl;
    std::cout << "N1: "; outputArray(arrn1, n1); std::cout << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    system("pause");
    system("pause");
    system("pause");
    system("pause");
    system("pause");
    system("pause");
    system("pause");

    return 0;

}