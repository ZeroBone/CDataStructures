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

int mainMenu();
char readMark();
void inputStudent(struct Student *student);
void tableTop();
void tableStudent(long number, struct Student *stud);
void tableBottom();
char markValid(char mark);
int fileViewMenu(char *fileName);
void writeStudent(FILE *file, struct Student *student);
char readStudent(FILE *file, struct Student *student);
unsigned long fwriteString(FILE *file, char *string);
void freadString(FILE *file, char *str, unsigned int maxLength);
int menu_view(char *fileName);
int menu_twosForAtLeast1Exam(char *fileName);
int menu_deleteExamResult(char *fileName);
int menu_twosForEveryDiscipline(char *fileName);
int menu_editExamResult(char *fileName);

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Russian");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (RU) puts("Добро пожаловать!");
    else puts("Welcome!");

    while (mainMenu() == 1);

    system("pause");

    return 0;

}

int mainMenu() {

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

        case 1:

            /* new file */

            if (RU) puts("Введите название файла:");
            else puts("Enter output file name:");

            char outputFileName[MAX_FNAME_LENGTH];

            fgets(outputFileName, MAX_FNAME_LENGTH, stdin);

            outputFileName[strlen(outputFileName) - 1] = '\0';

            FILE *outputFile;

            if ((outputFile = fopen(outputFileName, "wb")) == NULL) {

                if (RU) perror("Ошибка при открытии файла на чтение");
                else perror("An error occurred while trying to open the output file for writing");

                return 1;

            }

            /* writeInt(outputFile, 0); */ /* student count */

            fclose(outputFile);

            system("CLS");

            if (RU) puts("Файл успешно создан");
            else puts("File successfully created.");

            break;

        case 2:

            /* view & edit file */

            if (RU) puts("Введите имя файла:");
            else puts("Enter file name:");

            char fileName[MAX_FNAME_LENGTH];

            fgets(fileName, MAX_FNAME_LENGTH, stdin);
            fileName[strlen(fileName) - 1] = '\0';

            system("CLS");

            while (fileViewMenu(fileName) == 1);

            break;

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

int fileViewMenu(char *fileName) {

    FILE *file;

    if ((file = fopen(fileName, "rb")) == NULL) {

        system("CLS");

        if (RU) perror("Ошибка файловой системы");
        else perror("FS error");

        return 0;

    }

    long offset = 0;
    doublelinkedlist_t students = DLL_INIT(sizeof(student_t));

    while (1) {

        fseek(file, offset * STUDENT_COMPONENT_LENGTH, SEEK_SET);

        student_t student;
        if (!readStudent(file, &student)) {

            if (!offset) {
                if (RU) puts("Файл пустой.");
                else puts("File empty, nothing to view.");
            }

            break;

        }

        student_t* s = dll_addNodeToBeginning(&students);

        if (s == NULL) break; // not a good thing to do

        *s = student;

        offset++;

    }

    fclose(file);

    printf("Students: %p %p\n", students.firstNode, students.lastNode);

    if (RU) {

        puts("Введите нужный пункт меню:");
        puts("1 - просмотреть файл.");
        puts("2 - добавить результат экзамена.");
        puts("3 - удалить результат экзамена.");
        puts("4 - отредактировать результат экзамена.");
        puts("5 - посчитать количество двоек для каждой дисциплины.");
        puts("6 - вывести студентов, у которых двойки хотя-бы по одному экзамену.");
        puts("7 - вернуться в главное меню.");

    }
    else {

        puts("Please enter a number below to select what you want to do:");
        puts("1 - view file.");
        puts("2 - add exam result.");
        puts("3 - delete an exam result.");
        puts("4 - edit an exam result.");
        puts("5 - get the amount of 2-s for every discipline.");
        puts("6 - get students, that have 2-s for at least 1 exam.");
        puts("7 - return to the main menu.");

    }

    int result;

    scanf("%d", &result);
    getchar();

    switch (result) {

        case 1:

            /* view file */

            return menu_view(fileName);

        case 2:

            system("CLS");

            if ((file = fopen(fileName, "ab")) == NULL) {

                if (RU) perror("Ошибка при открытии файла для записи");
                else perror("An error occurred while trying to open the file for writing");

                return 1;

            }

            /* add exam result */

            struct Student student;

            inputStudent(&student);

            /*fseek(file, 0, SEEK_SET);*/

            /*fseek(file, -1, SEEK_END);*/

            /*printf("Writing to %d\n", ftell(file));*/

            writeStudent(file, &student);

            fclose(file);

            if (RU) puts("Результат экзамена добавлен.");
            else puts("Exam result added.");

            break;

        case 3:
            /* delete an exam result */

            return menu_deleteExamResult(fileName);

        case 4:

            /* edit an exam result */

            return menu_editExamResult(fileName);

        case 5:

            /* get the amount of 2-s for every discipline */

            return menu_twosForEveryDiscipline(fileName);

        case 6:

            /* get students, that have 2-s for at least 1 exam */

            return menu_twosForAtLeast1Exam(fileName);

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

int menu_view(char *fileName) {

    FILE *file;

    if ((file = fopen(fileName, "rb")) == NULL) {

        system("CLS");

        if (RU) perror("Ошибка при открытии файла не чтение");
        else perror("An error occurred while trying to open the file for reading");

        return 1;

    }

    fseek(file, 0, SEEK_SET);

    long int offset = 0, i;
    int choise;
    char hasNextPage;
    student_t student;

    while (1) {

        system("CLS");

        fseek(file, offset * STUDENT_COMPONENT_LENGTH, SEEK_SET);

        if (!readStudent(file, &student)) {

            if (RU) puts("Файл пустой.");
            else puts("File empty, nothing to view.");

            goto exitMark;

        }

        tableTop();

        hasNextPage = 1;

        for (i = 0; i < ROWS_PER_PAGE; i++) {

            fseek(file, (offset + i) * STUDENT_COMPONENT_LENGTH, SEEK_SET);

            if (!readStudent(file, &student)) {

                fseek(file, (offset + i) * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                hasNextPage = 0;

                break;

            }

            /*printf("Student last name: ");
            puts(stud->name);
            printf("Physics mark: %d\n", stud->marks[0]);
            printf("Math mark: %d\n", stud->marks[1]);
            printf("Informatics mark: %d\n", stud->marks[2]);*/

            tableStudent(i + offset, &student);

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

        scanf("%d", &choise);

        getchar();

        switch (choise) {

            case 1:
                /* scroll up */

                offset -= ROWS_PER_PAGE;
                if (offset < 0) offset = 0;

                break;

            case 2:
                /* scroll down */

                if (!hasNextPage) break;

                fseek(file, (offset + ROWS_PER_PAGE) * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                if (readStudent(file, &student)) {

                    offset += ROWS_PER_PAGE;

                }

                break;

            default:

                system("CLS");

                goto exitMark;

        }

    }
    exitMark:

    fclose(file);

    return 1;

}

int menu_twosForAtLeast1Exam(char *fileName) {

    FILE *file;

    if ((file = fopen(fileName, "rb")) == NULL) {

        system("CLS");

        if (RU) perror("Ошибка при открытии файла на чтение");
        else perror("An error occurred while trying to open the file for reading");

        return 1;

    }

    fseek(file, 0, SEEK_SET);

    long int offset = 0, i, currentOffset;
    int scroll;
    struct Student student;
    char hasNextPage;

    while (1) {

        system("CLS");

        fseek(file, offset * STUDENT_COMPONENT_LENGTH, SEEK_SET);

        if (!readStudent(file, &student)) {

            if (RU) puts("Файл пустой.");
            else puts("File empty, nothing to view.");

            break;

        }

        tableTop();

        i = 0;

        currentOffset = offset;

        hasNextPage = 1;

        while (i < ROWS_PER_PAGE) {

            fseek(file, (currentOffset + i) * STUDENT_COMPONENT_LENGTH, SEEK_SET);

            if (!readStudent(file, &student)) {

                fseek(file, (currentOffset + i) * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                hasNextPage = 0;

                break;

            }

            if (student.marks[0] == MARK_TWO || student.marks[1] == MARK_TWO || student.marks[2] == MARK_TWO) {

                tableStudent(i + currentOffset, &student);

                i++;

            }

            currentOffset++;

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

        scanf("%d", &scroll);

        getchar();

        switch (scroll) {

            case 1:
                /* scroll up */

                /*offset -= ROWS_PER_PAGE;
                if (offset < 0) offset = 0;*/
                i = 0;

                while (i < ROWS_PER_PAGE && offset) {

                    fseek(file, (offset) * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                    if (!readStudent(file, &student)) {

                        /* this cannot happen, we already read this data */

                        fseek(file, offset * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                        hasNextPage = 0;

                        break;

                    }

                    if (student.marks[0] == MARK_TWO || student.marks[1] == MARK_TWO || student.marks[2] == MARK_TWO) {

                        tableStudent(offset, &student);

                        i++;

                    }

                    offset--;

                }

                system("CLS");

                break;

            case 2:
                /* scroll down */

                if (hasNextPage && readStudent(file, &student)) {

                    offset = currentOffset;

                }

                system("CLS");

                break;

            default:

                system("CLS");

                goto exitM;

        }

    }
    exitM:

    fclose(file);

    return 1;

}

int menu_deleteExamResult(char *fileName) {

    FILE *file;
    FILE *tempFile;

    if ((file = fopen(fileName, "r+b")) == NULL) {

        if (RU) perror("Ошибка при открытии файла для чтения");
        else perror("An error occurred while trying to open the file for reading");

        return 1;

    }

    if ((tempFile = fopen("temp.bin", "wb")) == NULL) {

        if (RU) perror("Ошибка при открытии файла для чтения");
        else perror("An error occurred while trying to open the file for reading");

        return 1;

    }

    /* fseek(file, 0, SEEK_CUR); */

    if (RU) puts("Введите фамилию студента");
    else puts("Enter student last name:");

    char studentName[MAX_STUDENT_LAST_NAME_LENGTH];
    fgets(studentName, MAX_STUDENT_LAST_NAME_LENGTH, stdin);
    studentName[strlen(studentName) - 1] = '\0';

    system("CLS");

    struct Student st;
    long int n = 0, found = 0;
    char yes;
    char keep;

    while (1) {

        fseek(file, n * STUDENT_COMPONENT_LENGTH, SEEK_SET);

        if (!readStudent(file, &st)) {

            break;

        }

        if (!strcmp(st.name, studentName)) {

            found++;

            system("CLS");

            tableTop();

            tableStudent(n, &st);

            if (RU) puts("Удалить? (y/n)");
            else puts("Delete? (y/n)");

            do {

                yes = getchar();

            } while (yes != 'y' && yes != 'Y' && yes != 'n' && yes != 'N');

            getchar();

            if (yes == 'y' || yes == 'Y') {

                fseek(file, n * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                keep = 0;

                fseek(file, 0, SEEK_CUR); /* to write mode */

            }
            else keep = 1;

        }
        else keep = 1;

        if (keep) {

            writeStudent(tempFile, &st);

        }

        n++;

    }

    if (n == 0) {

        if (RU) puts("Файл пустой.");
        else puts("File empty.");

    }
    else if (found == 0) {

        if (RU) puts("Не найдено студентов с такой фамилией.");
        else puts("No matching students found.");

    }

    fclose(file);
    fclose(tempFile);

    unlink(fileName);
    rename("temp.bin", fileName);

    return 1;

}

int menu_twosForEveryDiscipline(char *fileName) {

    FILE *file;

    if ((file = fopen(fileName, "rb")) == NULL) {

        system("CLS");

        if (RU) perror("Ошибка открытия файла на чтение.");
        else perror("An error occurred while trying to open the file for reading");

        return 1;

    }

    fseek(file, 0, SEEK_SET);

    system("CLS");

    long int twos[] = {0, 0, 0};
    long int off = 0;
    int k;
    struct Student stu;

    while (1) {

        fseek(file, off * STUDENT_COMPONENT_LENGTH, SEEK_SET);

        if (!readStudent(file, &stu)) {

            if (!off) {

                if (RU) puts("Файл пустой.");
                else puts("File empty, nothing to view.");

            }

            break;

        }

        for (k = 0; k < 3; k++) {

            if (stu.marks[k] == MARK_TWO) {

                twos[k]++;

            }

        }

        off++;

    }

    if (RU) {

        printf("Двоек по физике: %d\n", twos[0]);
        printf("Двоек по математике: %d\n", twos[1]);
        printf("Двоек по информатике: %d\n", twos[2]);

    }
    else {

        printf("Two's for physics: %d\n", twos[0]);
        printf("Two's for math: %d\n", twos[1]);
        printf("Two's for informatics: %d\n", twos[2]);

    }

    fclose(file);

    return 1;

}

int menu_editExamResult(char *fileName) {

    FILE *file;

    if ((file = fopen(fileName, "r+b")) == NULL) {

        if (RU) perror("Ошибка открытия файла на редактирование");
        else perror("An error occurred while trying to open the file for editing");

        return 1;

    }

    /* fseek(file, 0, SEEK_CUR); */

    if (RU) puts("Введите фамилию студента:");
    else puts("Enter student last name:");

    char studentName[MAX_STUDENT_LAST_NAME_LENGTH];
    fgets(studentName, MAX_STUDENT_LAST_NAME_LENGTH, stdin);
    studentName[strlen(studentName) - 1] = '\0';

    system("CLS");

    struct Student st;
    long int n = 0, found = 0;
    char yes;

    while (1) {

        fseek(file, n * STUDENT_COMPONENT_LENGTH, SEEK_SET);

        if (!readStudent(file, &st)) {

            break;

        }

        /* printf("Comp: Read: %s Searching: %s\n", st.name, studentName); */

        if (!strcmp(st.name, studentName)) {

            found++;

            system("CLS");

            tableTop();

            tableStudent(n, &st);

            if (RU) puts("Редактировать? (y/n)");
            else puts("Edit? (y/n)");

            do {

                yes = getchar();

            } while (yes != 'y' && yes != 'Y' && yes != 'n' && yes != 'N');

            getchar();

            if (yes == 'y' || yes == 'Y') {

                inputStudent(&st);

                fseek(file, n * STUDENT_COMPONENT_LENGTH, SEEK_SET);

                writeStudent(file, &st);

                fseek(file, 0, SEEK_CUR); /* to write mode */

            }

        }

        n++;

    }

    if (n == 0) {

        if (RU) puts("Файл пустой.");
        else puts("File empty.");

    }
    else if (found == 0) {

        if (RU) puts("Не найдено студентов с этой фамилией.");
        else puts("No matching students found.");

    }

    fclose(file);

    return 1;

}