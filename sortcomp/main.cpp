#include <iostream>

void StraightInsertion(int* a, int n) {
    int i, j, x;
    for (i = 1; i < n; i++) {
        x = a[i];
        for (j = i; j && x < a[j - 1]; j--) /* находим место вставки, попутно сдвигая элементы */
            a[j] = a[j - 1];
        a[j] = x;
    }
}

void ShakerSort(int* a, int n) {
    int j, k = n - 1, left = 1, right = n - 1, x;
    do {
        for (j = right; j >= left; j--)    /*сначала просматриваем справа налево*/
            if (a[j - 1] > a[j]) {
                x = a[j - 1];
                a[j - 1] = a[j];
                a[j] = x;
                k = j;
            }
        left = k + 1;
        for (j = left; j <= right; j++)    /*а теперь просматриваем слева направо*/
            if (a[j - 1] > a[j]) {
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
    if (j < R && a[j + 1] > a[j])      /*если элемент j не последний в рассматр. последовательности*/
        j++;                  /*и правый брат больше левого, переставляем указатель на него*/
    while (j <= R && a[j] > x)      /*просматриваем потомков до конца последовательности,*/
    {                         /*если они больше текущего (х) элемента, т.е. нарушена*/
        a[i] = a[j];        /*упорядоченность пирамиды, то восстанавливаем ее,*/
        i = j;                /*поднимая потомков на уровень выше*/
        j = 2 * j + 1;
        if (j < R && a[j + 1] > a[j])    /*и каждый раз выбираем для анализа большего из*/
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

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}