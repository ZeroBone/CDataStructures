#include <iostream>
#include <math.h>

#define PI 3.1415926535
#define PRINT_METHOD
#define COMPARISON_EPSILON 1e-6

class Triangle {

private:
    double c;
    double alpha, beta;

public:

    Triangle(double length, double alpha, double beta): c(length), alpha(alpha), beta(beta) {
#ifdef PRINT_METHOD
        std::cout << "Constructor called" << std::endl;
#endif
    }

    Triangle(const Triangle &t):c(t.c), alpha(t.alpha), beta(t.beta) {
#ifdef PRINT_METHOD
        std::cout << "Copy constructor called" << std::endl;
#endif
    }

    ~Triangle() {
#ifdef PRINT_METHOD
        std::cout << "Destructor called" << std::endl;
#endif
    }

    double getP() {
#ifdef PRINT_METHOD
        std::cout << "getP() called" << std::endl;
#endif
        double otherAngleSin = sin(PI - alpha - beta);

        double a = c * sin(beta) / otherAngleSin;
        double b = c * sin(alpha) / otherAngleSin;

        return a + b + c;
    }

    double getOuterCircleRadius() {

#ifdef PRINT_METHOD
        std::cout << "getOuterCircleRadius() called" << std::endl;
#endif

        double otherAngleSin = sin(PI - alpha - beta);

        // double a = c * sin(beta);
        // double b = c * sin(alpha);
        double a = c * sin(beta) / otherAngleSin;
        double b = c * sin(alpha) / otherAngleSin;

        double p = (a + b + c) / 2.;
        return a * b * c / 4. / sqrt(p * (p - a) * (p - b) * (p - c));

    }

    double getMedianC() {

#ifdef PRINT_METHOD
        std::cout << "getMedianC() called" << std::endl;
#endif

        double otherAngleSin = sin(PI - alpha - beta);

        // double a = c * sin(beta);
        // double b = c * sin(alpha);
        double a = c * sin(beta) / otherAngleSin;
        double b = c * sin(alpha) / otherAngleSin;

        std::cout << "A: " << a << std::endl;
        std::cout << "B: " << b << std::endl;
        std::cout << "C: " << c << std::endl;

        return .5 * sqrt(2. * a * a + 2. * b * b - c * c);

    }

    int operator==(Triangle &t) {
#ifdef PRINT_METHOD
        std::cout << "== operator called" << std::endl;
#endif

        return fabs(c - t.c) < COMPARISON_EPSILON && fabs(alpha - t.alpha) < COMPARISON_EPSILON &&
            fabs(beta - t.beta) < COMPARISON_EPSILON;

        return c == t.c && alpha == t.alpha && beta == t.beta;
    }

    double getC() const {
#ifdef PRINT_METHOD
        std::cout << "getC() called" << std::endl;
#endif
        return c;
    }

    void setC(double c) {
#ifdef PRINT_METHOD
        std::cout << "setC() called" << std::endl;
#endif
        Triangle::c = c;
    }

    void printC() {
        std::cout << "C: " << c << std::endl;
    }

    double getAlpha() const {
#ifdef PRINT_METHOD
        std::cout << "getAlpha() called" << std::endl;
#endif
        return alpha;
    }

    void setAlpha(double alpha) {
#ifdef PRINT_METHOD
        std::cout << "setAlpha() called" << std::endl;
#endif
        Triangle::alpha = alpha;
    }

    void printAlpha() {
        std::cout << "Alpha: " << alpha << std::endl;
    }

    double getBeta() const {
#ifdef PRINT_METHOD
        std::cout << "getBeta() called" << std::endl;
#endif
        return beta;
    }

    void setBeta(double beta) {
#ifdef PRINT_METHOD
        std::cout << "setBeta() called" << std::endl;
#endif
        Triangle::beta = beta;
    }

    void printBeta() {
        std::cout << "Beta: " << beta << std::endl;
    }

};

int main() {

    Triangle* triangle = new Triangle(10., PI / 2., PI / 4.);

    while (true) {

        std::cout << "0 - exit: " << std::endl;
        std::cout << "1 - getP(): " << std::endl;
        std::cout << "2 - getOuterCircleRadius(): " << std::endl;
        std::cout << "3 - getMedianC(): " << std::endl;
        std::cout << "4 - getC(): " << std::endl;
        std::cout << "5 - setC(): " << std::endl;
        std::cout << "6 - setAlpha(): " << std::endl;
        std::cout << "7 - setBeta(): " << std::endl;
        std::cout << "8 - printC(): " << std::endl;
        std::cout << "9 - getAlpha(): " << std::endl;
        std::cout << "10 - printAlpha(): " << std::endl;
        std::cout << "11 - getBeta(): " << std::endl;
        std::cout << "12 - printBeta(): " << std::endl;
        std::cout << "13 - compare triangles " << std::endl;

        std::cout << "Item:";
        int choice;
        std::cin >> choice;

        system("cls");

        switch (choice) {

            case 0:
                goto end;

            case 1:
                std::cout << "Perimetr:" << std::endl;
                std::cout << triangle->getP() << std::endl;
                break;

            case 2:
                std::cout << "Outer circle radius:" << std::endl;
                std::cout << triangle->getOuterCircleRadius() << std::endl;
                break;

            case 3:
                std::cout << "Median:" << std::endl;
                std::cout << triangle->getMedianC() << std::endl;
                break;

            case 4:
                std::cout << "C:" << std::endl;
                std::cout << triangle->getC() << std::endl;
                break;

            case 5: {
                std::cout << "Enter C:" << std::endl;
                double v;
                std::cin >> v;

                triangle->setC(v);

                break;
            }

            case 6: {
                std::cout << "Enter alpha:" << std::endl;
                double v;
                std::cin >> v;

                triangle->setAlpha(v);

                break;
            }

            case 7: {
                std::cout << "Enter beta:" << std::endl;
                double v;
                std::cin >> v;

                triangle->setBeta(v);

                break;
            }

            case 8:
                triangle->printC();
                break;

            case 9:
                std::cout << "Alpha:" << std::endl;
                std::cout << triangle->getAlpha() << std::endl;
                break;

            case 10:
                triangle->printAlpha();
                break;

            case 11:
                std::cout << "Beta:" << std::endl;
                std::cout << triangle->getBeta() << std::endl;
                break;

            case 12:
                triangle->printBeta();
                break;

            case 13: {

                double c, alpha, beta;

                std::cout << "Enter C:" << std::endl;
                std::cin >> c;

                std::cout << "Enter alpha:" << std::endl;
                std::cin >> alpha;

                std::cout << "Enter beta:" << std::endl;
                std::cin >> beta;

                Triangle t = Triangle(c, alpha, beta);

                std::cout << "Triangles equal: " << (t == *triangle ? "true" : "false") << std::endl;

            }

            default:
                std::cout << "Unknown menu item, try again." << std::endl;
                break;

        }

    }
    end:

    delete triangle;

    system("pause");

    return 0;
}