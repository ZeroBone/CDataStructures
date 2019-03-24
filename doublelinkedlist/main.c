#include "main.h"

#include "doublelinkedlist.h"

#define MAX_FNAME_LENGTH 50
#define MAX_STUDENT_LAST_NAME_LENGTH 20
#define MAX_STUDENT_ADDRESS_LENGTH 20
#define STUDENT_COMPONENT_LENGTH 45
#define MARK_MIN 2
#define MARK_MAX 5
#define MARK_TWO 2
#define ROWS_PER_PAGE 5
#define RU 1

typedef struct Student {
    char marks[3];
    char name[MAX_STUDENT_LAST_NAME_LENGTH];
    // char :1;
    char inWG:1;
    union {
        int wgId;
        char address[MAX_STUDENT_ADDRESS_LENGTH];
    } livingIn;
} student_t;

int mainMenu(char* fileName);
char readMark();
void inputStudent(struct Student *student);
void tableTop();
void tableStudent(long number, struct Student *stud);
void tableBottom();
char markValid(char mark);
void fileSave(char* fileName, doublelinkedlist_t* students);
int fileViewMenu(char *fileName);
void writeStudent(FILE *file, struct Student *student);
char readStudent(FILE *file, struct Student *student);
unsigned long fwriteString(FILE *file, char *string);
void freadString(FILE *file, char *str, unsigned int maxLength);
int menu_view(doublelinkedlist_t* students);
int menu_viewInBackOrder(doublelinkedlist_t* students);
int menu_twosForAtLeast1Exam(doublelinkedlist_t* students);
int menu_deleteExamResult(doublelinkedlist_t* students);
int menu_twosForEveryDiscipline(doublelinkedlist_t* students);

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Russian");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (RU) puts("Добро пожаловать!");
    else puts("Welcome!");

    char* fileName = NULL;

    if (argc >= 2) {
        fileName = argv[1];
    }

    while (mainMenu(fileName) == 1);

    system("pause");

    return 0;

}

int mainMenu(char* fileName) {

    if (RU) {

        puts("Введите нужный пункт меню:");
        puts("1 - создать новый файл результатов экзамена.");
        puts("2 - просмотреть файл результатов экзамена.");
        puts("3 - выход.");

    }
    else {

        puts("Please enter a number below to select what you want to do:");
        puts("1 - create new exam results file.");
        puts("2 - view the exam results file.");
        puts("3 - exit the program.");

    }

    int result;
    scanf("%d", &result);

    getchar();

    switch (result) {

        case 1: {

            /* new file */

            char outputFileName[MAX_FNAME_LENGTH];

            if (fileName == NULL) {

                if (RU) puts("Введите название файла:");
                else puts("Enter output file name:");

                fgets(outputFileName, MAX_FNAME_LENGTH, stdin);
                outputFileName[strlen(outputFileName) - 1] = '\0';

            }
            else {

                strncpy(outputFileName, fileName, MAX_FNAME_LENGTH - 1);

            }

            FILE* outputFile;

            if ((outputFile = fopen(outputFileName, "wb")) == NULL) {

                if (RU) perror("Ошибка при открытии файла на запись");
                else perror("An error occurred while trying to open the output file for writing");

                return 1;

            }

            /* writeInt(outputFile, 0); */ /* student count */

            fclose(outputFile);

            system("CLS");

            if (RU) puts("Файл успешно создан");
            else puts("File successfully created.");

            break;

        }

        case 2: {

            /* view & edit file */

            char viewFileName[MAX_FNAME_LENGTH];

            if (fileName == NULL) {

                if (RU) puts("Введите имя файла:");
                else puts("Enter file name:");

                fgets(viewFileName, MAX_FNAME_LENGTH, stdin);
                viewFileName[strlen(viewFileName) - 1] = '\0';

            }
            else {
                strncpy(viewFileName, fileName, MAX_FNAME_LENGTH - 1);
            }

            system("CLS");

            while (fileViewMenu(viewFileName) == 1);

            break;

        }

        case 3:
            return 0;

        default:

            system("CLS");

            if (RU) puts("Введённый вами номер не соответствует ни одному из пунктов меню. Попробуйте снова.");
            else puts("The number you entered doesn't correspond to any of the menu items. Please try again.");

            return 1;

    }

    return 1;

}

void fileSave(char* fileName, doublelinkedlist_t* students) {

    FILE *file;

    if ((file = fopen(fileName, "wb")) == NULL) {

        system("CLS");

        if (RU) perror("Ошибка файловой системы");
        else perror("FS error");

        return;

    }

    doublelinkedlist_node_t* current = students->firstNode;

    while (current != NULL) {

        writeStudent(file, (student_t*)dll_nodePayload(students, current));

        current = current->next;

    }

    fclose(file);

}

int fileViewMenu(char *fileName) {

    doublelinkedlist_t students = DLL_INIT(sizeof(student_t));

    {
        FILE *file;

        if ((file = fopen(fileName, "rb")) == NULL) {

            system("CLS");

            if (RU) perror("Ошибка файловой системы");
            else perror("FS error");

            return 0;

        }

        long offset = 0;

        while (1) {

            fseek(file, offset * STUDENT_COMPONENT_LENGTH, SEEK_SET);

            student_t student;
            if (!readStudent(file, &student)) {

                break;

            }

            // student_t* s = (student_t*)dll_addNodeToBeginning(&students);
            student_t* s = (student_t*)dll_addNodeToEnd(&students);

            if (s == NULL) break; // not a good thing to do

            *s = student;

            offset++;

        }

        fclose(file);
    }

    /*printf("Students: %p %p\n", students.firstNode, students.lastNode);*/

    if (RU) {

        puts("Введите нужный пункт меню:");
        puts("1 - просмотреть файл.");
        puts("2 - просмотреть файл в обратном порядке.");
        puts("3 - добавить результат экзамена.");
        puts("4 - удалить результат экзамена.");
        puts("5 - посчитать количество двоек для каждой дисциплины.");
        puts("6 - вывести студентов, у которых двойки хотя-бы по одному экзамену.");
        puts("7 - вернуться в главное меню.");

    }
    else {

        puts("Please enter a number below to select what you want to do:");
        puts("1 - view file.");
        puts("2 - view file in reverse order.");
        puts("3 - add exam result.");
        puts("4 - delete an exam result.");
        puts("5 - get the amount of 2-s for every discipline.");
        puts("6 - get students, that have 2-s for at least 1 exam.");
        puts("7 - return to the main menu.");

    }

    int result;

    scanf("%d", &result);
    getchar();

    system("CLS");

    switch (result) {

        case 1:

            /* view file */

            return menu_view(&students);

        case 2:

            return menu_viewInBackOrder(&students);

        case 3:

            system("CLS");

            /* add exam result */

            student_t newStudent;
            inputStudent(&newStudent);

            doublelinkedlist_node_t* current = students.firstNode;
            student_t* student;

            if (current == NULL) {
                // we are adding to an empty file
                printf("Adding '%s' to the beginning.\n", newStudent.name);

                student = (student_t*)dll_addNodeToBeginning(&students);

            }
            else {

                bool foundNext = false;

                while (current->next != NULL) {

                    student_t* compareStudent = (student_t*)dll_nodePayload(&students, current);

                    // printf("Comparing '%s' and current '%s'\n", newStudent.name, compareStudent->name);
                    if (strcmp(newStudent.name, compareStudent->name) < 0) {
                        // printf("Adding '%s' before '%s'\n", newStudent.name, compareStudent->name);

                        student = (student_t*)dll_insertNodeBefore(&students, current);

                        foundNext = true;

                        break;

                    }

                    current = current->next;

                }

                // if (current->next == NULL) {
                if (!foundNext) {

                    student_t* compareStudent = (student_t*)dll_nodePayload(&students, current);

                    // printf("(2) Comparing '%s' and current '%s'\n", newStudent.name, compareStudent->name);
                    if (strcmp(newStudent.name, compareStudent->name) < 0) {
                        // printf("(2) Adding '%s' before '%s'\n", newStudent.name, compareStudent->name);

                        student = (student_t*)dll_insertNodeBefore(&students, current);

                    }
                    else {
                        // printf("Adding '%s' to the end.\n", newStudent.name);

                        student = (student_t*)dll_addNodeToEnd(&students);
                    }

                }

            }

            *student = newStudent;

            fileSave(fileName, &students);

            if (RU) puts("Результат экзамена добавлен.");
            else puts("Exam result added.");

            break;

        case 4:
            /* delete an exam result */

            menu_deleteExamResult(&students);

            fileSave(fileName, &students);

            break;

        case 5:

            /* get the amount of 2-s for every discipline */

            return menu_twosForEveryDiscipline(&students);

        case 6:

            /* get students, that have 2-s for at least 1 exam */

            return menu_twosForAtLeast1Exam(&students);

        case 7:

            system("CLS");

            return 0;

        default:

            system("CLS");

            if (RU) puts("Некорректный пункт меню выбран, попробуйте снова.");
            else puts("Invalid menu item selected, try again.");

            break;

    }

    return 1;

}

void inputStudent(struct Student *student) {

    if (RU) puts("Введите фамилию студента:");
    else puts("Enter student last name:");

    fgets(student->name, MAX_STUDENT_LAST_NAME_LENGTH, stdin);
    student->name[strlen(student->name) - 1] = '\0';

    if (RU) puts("Введите оценку по физике:");
    else puts("Enter the mark for physics:");

    while (!markValid(student->marks[0] = readMark())) {

        if (RU) puts("Некорректная оценка введена, попробуйте снова:");
        else puts("Invalid mark entered, try again:");

    }

    if (RU) puts("Введите оценку по математике:");
    else puts("Enter the mark for math:");

    while (!markValid(student->marks[1] = readMark())) {

        if (RU) puts("Некорректная оценка введена, попробуйте снова:");
        else puts("Invalid mark entered, try again:");

    }

    if (RU) puts("Введите оценку по информатике:");
    else puts("Enter the mark for informatics:");

    while (!markValid(student->marks[2] = readMark())) {

        if (RU) puts("Некорректная оценка введена, попробуйте снова:");
        else puts("Invalid mark entered, try again:");

    }

    if (RU) puts("Этот студент живёт в общежитии? (y/n)");
    else puts("Does this student live in a hostel? (y/n)");

    char isLivingInWG;

    getchar();
    scanf("%c", &isLivingInWG);
    // printf("%c\n", isLivingInWG);
    getchar();

    if (isLivingInWG == 'y' || isLivingInWG == 'Y') {
        student->inWG = 1;

        if (RU) puts("Введите номер общежития студента.");
        else puts("Enter student WG id.");

        scanf("%d", &student->livingIn.wgId);
        getchar();

    }
    else {
        student->inWG = 0;

        if (RU) puts("Введите место жительства студента.");
        else puts("Enter student address.");

        fgets(student->livingIn.address, MAX_STUDENT_ADDRESS_LENGTH, stdin);
        student->livingIn.address[strlen(student->livingIn.address) - 1] = '\0';

        // printf("Got address: '%s'\n", student->livingIn.address);

    }

}

void tableTop() {

    if (RU) {

        puts("--------------------------------------------------------------");
        puts("|  #|      Фамилия       | Ф | М | И | Место жительства      |");
        puts("--------------------------------------------------------------");

    }
    else {

        puts("--------------------------------------------------------------");
        puts("|  #|        Name        | P | M | I | Living |");
        puts("--------------------------------------------------------------");

    }

}

void tableStudent(long number, student_t *stud) {

    printf("|%3ld|%-20s| %1d | %1d | %1d |",
           number + 1,
           stud->name,
           stud->marks[0],
           stud->marks[1],
           stud->marks[2]
    );

    if (stud->inWG) {

        printf(" Общежитие #%-10d |\n", stud->livingIn.wgId);

    }
    else {

        printf(" %-21s |\n", stud->livingIn.address);

    }

}

void tableBottom() {

    puts("--------------------------------------------------------------");

}

char markValid(char mark) {

    return mark >= MARK_MIN && mark <= MARK_MAX;

}

char readMark() {

    int temp;

    scanf("%d", &temp);

    return temp;

}

void writeStudent(FILE *file, student_t *student) {

    fputc(student->marks[0], file);
    fputc(student->marks[1], file);
    fputc(student->marks[2], file);

    long written = 3 + fwriteString(file, student->name);

    fputc(student->inWG, file);

    written++;

    if (student->inWG) {
        fwrite(&student->livingIn.wgId, sizeof(int), 1, file);
        written += sizeof(int);
    }
    else {
        written += fwriteString(file, student->livingIn.address);
    }

    /* printf("Written: %d\n", written); */

    for (;written < STUDENT_COMPONENT_LENGTH; written++) {

        fputc(0, file);

    }

}

char readStudent(FILE *file, struct Student *student) {

    int current, i;

    for (i = 0; i < 3; i++) {

        current = fgetc(file);

        if (current == EOF) {

            return 0;

        }

        student->marks[i] = (char)current;

    }

    /*printf("CUROSSTR: %d\n", ftell(file));*/

    /*printf("Mark: %d\n", student->marks[0]);
    printf("Mark 2 : %d\n", student->marks[1]);
    printf("Mark 3: %d\n", student->marks[2]);*/

    freadString(file, student->name, MAX_STUDENT_LAST_NAME_LENGTH);

    current = fgetc(file);

    if (current == EOF) return 0;

    student->inWG = (char)current;

    if (student->inWG) {
        fread(&student->livingIn.wgId, sizeof(int), 1, file);
    }
    else {
        freadString(file, student->livingIn.address, MAX_STUDENT_ADDRESS_LENGTH);
    }

    return 1;

}

unsigned long fwriteString(FILE *file, char *string) {

    unsigned long len = 1;

    while (*string != '\0') {

        fputc(*string, file);

        string++;

        len++;

    }

    fputc(0, file);

    return len;

}

void freadString(FILE *file, char *str, unsigned int maxLength) {

    unsigned int i;
    int current;

    for (i = 0; i < maxLength; i++) {

        current = fgetc(file);

        if (current == '\0' || current == EOF) {

            str[i] = '\0';

            break;

        }

        str[i] = (char)current;

    }

}

int menu_view(doublelinkedlist_t* students) {

    doublelinkedlist_node_t* startingFrom = students->firstNode;

    if (startingFrom == NULL) {

        if (RU) puts("Файл пустой.");
        else puts("File empty, nothing to view.");

        return 1;

    }

    doublelinkedlist_node_t* current;
    int i, offset = 0;

    while (1) {

        system("CLS");
        current = startingFrom;

        tableTop();

        bool atTheEnd = false;

        for (i = 0; i < ROWS_PER_PAGE; i++) {

            /*printf("Student last name: ");
            puts(stud->name);
            printf("Physics mark: %d\n", stud->marks[0]);
            printf("Math mark: %d\n", stud->marks[1]);
            printf("Informatics mark: %d\n", stud->marks[2]);*/

            tableStudent(i + offset, (student_t*)dll_nodePayload(students, current));

            if (current->next == NULL) {
                atTheEnd = true;
                break;
            }

            current = current->next;
            // offset++;
        }

        tableBottom();

        if (RU) {

            puts("В каком направлении прокрутить список?");
            puts("1 - вверх.");
            puts("2 - вниз.");
            puts("3 - выход.");

        }
        else {

            puts("Which direction do you want to scroll to?");
            puts("1 - scroll up.");
            puts("2 - scroll down.");
            puts("3 - exit.");

        }

        int choise;
        scanf("%d", &choise);

        getchar();

        switch (choise) {

            case 1:
                /* scroll up */

                i = 0;
                while (i < ROWS_PER_PAGE && startingFrom->previous != NULL) {

                    startingFrom = startingFrom->previous;
                    offset--;

                    i++;

                }

                /*for (i = 0; i < ROWS_PER_PAGE; i++) {

                    if (current->previous == NULL) break;

                    current = current->previous;
                    offset--;

                }*/

                /*offset -= ROWS_PER_PAGE;*/
                /*if (offset < 0) offset = 0;*/

                // offset += ROWS_PER_PAGE;

                break;

            case 2:
                /* scroll down */

                // if (current->next == NULL) {
                if (atTheEnd) {
                    // printf("END\n");
                    break;
                }

                startingFrom = current;
                offset += ROWS_PER_PAGE;

                /*for (i = 0; i < ROWS_PER_PAGE; i++) {

                    if (current->next == NULL) break;

                    current = current->next;
                    offset++;

                }*/

                break;

            default:

                system("CLS");

                goto exitMark;

        }

    }
    exitMark:

    return 1;

}

int menu_viewInBackOrder(doublelinkedlist_t* students) {

    doublelinkedlist_node_t* startingFrom = students->lastNode;

    if (startingFrom == NULL) {

        if (RU) puts("Файл пустой.");
        else puts("File empty, nothing to view.");

        return 1;

    }

    doublelinkedlist_node_t* current = startingFrom;
    long elementCount = 0;

    while (current != NULL) {

        current = current->previous;

        if (++elementCount == ROWS_PER_PAGE) {
            break;
        }

    }

    // printf("Element count: %ld\n", elementCount);

    int i, offset = 0;

    while (1) {

        system("CLS");
        current = startingFrom;

        tableTop();

        bool atTheStart = false;

        for (i = 0; i < elementCount; i++) {

            /*printf("Student last name: ");
            puts(stud->name);
            printf("Physics mark: %d\n", stud->marks[0]);
            printf("Math mark: %d\n", stud->marks[1]);
            printf("Informatics mark: %d\n", stud->marks[2]);*/

            long id = elementCount - 1 - i + offset;

            // tableStudent(ROWS_PER_PAGE - i + offset, (student_t*)dll_nodePayload(students, current));
            tableStudent(id, (student_t*)dll_nodePayload(students, current));

            if (current->previous == NULL) {
                atTheStart = true;
                break;
            }

            current = current->previous;
            // offset++;
        }

        tableBottom();

        if (RU) {

            puts("В каком направлении прокрутить список?");
            puts("1 - вверх.");
            puts("2 - вниз.");
            puts("3 - выход.");

        }
        else {

            puts("Which direction do you want to scroll to?");
            puts("1 - scroll up.");
            puts("2 - scroll down.");
            puts("3 - exit.");

        }

        int choise;
        scanf("%d", &choise);

        getchar();

        switch (choise) {

            case 1:
                /* scroll up */

                if (atTheStart) {
                    // printf("END\n");
                    break;
                }

                startingFrom = current;
                offset += ROWS_PER_PAGE;

                break;

            case 2:
                /* scroll down */

                i = 0;
                while (i < ROWS_PER_PAGE && startingFrom->next != NULL) {

                    startingFrom = startingFrom->next;
                    offset--;

                    i++;

                }

                break;

            default:

                system("CLS");

                goto exitMark;

        }

    }
    exitMark:

    return 1;

}

int menu_twosForAtLeast1Exam(doublelinkedlist_t* students) {

    doublelinkedlist_node_t* startingFrom = students->firstNode;

    if (startingFrom == NULL) {

        if (RU) puts("Файл пустой.");
        else puts("File empty, nothing to view.");

        return 1;

    }

    doublelinkedlist_node_t* current;
    int i, offset = 0;

    while (1) {

        system("CLS");
        current = startingFrom;

        tableTop();

        bool atTheEnd = false;

        i = 0;
        while (i < ROWS_PER_PAGE) {

            /*printf("Student last name: ");
            puts(stud->name);
            printf("Physics mark: %d\n", stud->marks[0]);
            printf("Math mark: %d\n", stud->marks[1]);
            printf("Informatics mark: %d\n", stud->marks[2]);*/

            student_t* currentStudent = (student_t*)dll_nodePayload(students, current);

            if (currentStudent->marks[0] == MARK_TWO || currentStudent->marks[1] == MARK_TWO || currentStudent->marks[2] == MARK_TWO) {
                tableStudent(i + offset, currentStudent);
                i++;
            }

            if (current->next == NULL) {
                atTheEnd = true;
                break;
            }

            current = current->next;

        }

        tableBottom();

        if (RU) {

            puts("В каком направлении прокрутить список?");
            puts("1 - вверх.");
            puts("2 - вниз.");
            puts("3 - выход.");

        }
        else {

            puts("Which direction do you want to scroll to?");
            puts("1 - scroll up.");
            puts("2 - scroll down.");
            puts("3 - exit.");

        }

        int choise;
        scanf("%d", &choise);

        getchar();

        switch (choise) {

            case 1:
                /* scroll up */

                i = 0;
                while (i < ROWS_PER_PAGE && startingFrom->previous != NULL) {

                    student_t* currentStudent = (student_t*)dll_nodePayload(students, startingFrom);

                    if (currentStudent->marks[0] == MARK_TWO || currentStudent->marks[1] == MARK_TWO || currentStudent->marks[2] == MARK_TWO) {
                        i++;
                        offset--;
                    }

                    startingFrom = startingFrom->previous;

                }

                break;

            case 2:
                /* scroll down */

                // if (current->next == NULL) {
                if (atTheEnd) {
                    // printf("END\n");
                    break;
                }

                startingFrom = current;
                offset += ROWS_PER_PAGE;

                /*for (i = 0; i < ROWS_PER_PAGE; i++) {

                    if (current->next == NULL) break;

                    current = current->next;
                    offset++;

                }*/

                break;

            default:

                system("CLS");

                goto exitMark;

        }

    }
    exitMark:

    return 1;

}

int menu_deleteExamResult(doublelinkedlist_t* students) {

    system("CLS");

    doublelinkedlist_node_t* current = students->firstNode;

    if (current == NULL) {

        if (RU) puts("Файл пустой.");
        else puts("File empty, nothing to view.");

        return 1;

    }

    if (RU) puts("Введите фамилию студента");
    else puts("Enter student last name:");

    char studentName[MAX_STUDENT_LAST_NAME_LENGTH];
    fgets(studentName, MAX_STUDENT_LAST_NAME_LENGTH, stdin);
    studentName[strlen(studentName) - 1] = '\0';

    int offset = 0;
    bool found = false;

    while (current != NULL) {

        student_t* currentStudent = (student_t*)dll_nodePayload(students, current);

        if (!strcmp(currentStudent->name, studentName)) {

            found = true;

            tableTop();

            tableStudent(offset, currentStudent);

            if (RU) puts("Удалить? (y/n)");
            else puts("Delete? (y/n)");

            char yes;

            do {

                yes = getchar();

            } while (yes != 'y' && yes != 'Y' && yes != 'n' && yes != 'N');

            getchar();

            system("CLS");

            if (yes == 'y' || yes == 'Y') {
                // delete

                doublelinkedlist_node_t* next = current->next;

                dll_deleteNode(students, current);

                current = next;

                puts("Запись удалена.");

                continue;

            }
            else {
                puts("Удаление отменено.");
            }

        }

        current = current->next;

    }

    if (!found) {

        system("cls");

        puts("Не найдено ни одного студента с такой фамилией.");

    }

    return 1;

}

int menu_twosForEveryDiscipline(doublelinkedlist_t* students) {

    system("cls");

    doublelinkedlist_node_t* current = students->firstNode;

    if (current == NULL) {

        if (RU) puts("Файл пустой.");
        else puts("File empty, nothing to view.");

        return 1;

    }

    long int twos[] = {0, 0, 0};
    int k;

    while (current->next != NULL) {

        student_t* currentStudent = (student_t*)dll_nodePayload(students, current);

        for (k = 0; k < 3; k++) {

            if (currentStudent->marks[k] == MARK_TWO) {

                twos[k]++;

            }

        }

        current = current->next;

    }

    if (RU) {

        printf("Двоек по физике: %ld\n", twos[0]);
        printf("Двоек по математике: %ld\n", twos[1]);
        printf("Двоек по информатике: %ld\n", twos[2]);

    }
    else {

        printf("Two's for physics: %ld\n", twos[0]);
        printf("Two's for math: %ld\n", twos[1]);
        printf("Two's for informatics: %ld\n", twos[2]);

    }

    return 1;

}