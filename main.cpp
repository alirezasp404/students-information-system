#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
struct passedLessons {
    char lessonName[30];
    char lessonNumber[10];
    int amountOfUnit;
    passedLessons *next;
    passedLessons *pre;
};
struct student {
    char name[25];
    char studentNumber[10];
    int numberOfLessons;
    int amountOfTuition;
    passedLessons *lessons;
    student *next;
};
struct suggestedLessons {
    char lessonName[30];
    char lessonNumber[10];
    int amountOfUnit;
    int admissionCapacity;
    suggestedLessons *next;
    suggestedLessons *pre;
};

suggestedLessons *lessonsInfo;

student *studentsInfo;
int numberOfStudents, numberOfLessons;

void getLessonsInfo();

void getStudentsInfo();

void menu();

student *findSelectedStudent(char[]);

bool printPassedLessons(student *);

void printSuggestedLessons(suggestedLessons [], int);

void addToPassedLessons(student *);

void deletePassedLesson(student *);

void deleteLinkList();

void writeInFile();

int main() {
    getLessonsInfo();
    getStudentsInfo();
    menu();
    return 0;
}

void getLessonsInfo() {
    ifstream getLessons;
    getLessons.open("lessons.txt");
    if (!getLessons.is_open()) {
        cout << "Error in opening file!";
        exit(0);
    }
    getLessons >> numberOfLessons;
    lessonsInfo = nullptr;
    for (int i = 0; i < numberOfLessons; ++i) {
        suggestedLessons *newLessonInfo = new suggestedLessons;
        getLessons.ignore(1);
        getLessons.get(newLessonInfo->lessonName, 30, '1');
        getLessons >> newLessonInfo->lessonNumber;
        getLessons >> newLessonInfo->amountOfUnit;
        getLessons >> newLessonInfo->admissionCapacity;
        newLessonInfo->next = lessonsInfo;
        lessonsInfo = newLessonInfo;
    }
    getLessons.close();
    suggestedLessons *temp1 = lessonsInfo;
    temp1->pre = nullptr;
    suggestedLessons *temp2 = temp1->next;
    //add previous pointer
    while (temp2 != nullptr) {
        temp2->pre = temp1;
        temp1 = temp2;
        temp2 = temp2->next;
    }
}

void getStudentsInfo() {
    ifstream getStudents;
    getStudents.open("students.txt");
    if (!getStudents.is_open()) {
        cout << "Error in opening file!";
        exit(0);
    }
    getStudents >> numberOfStudents;
    studentsInfo = nullptr;
    for (int i = 0; i < numberOfStudents; ++i) {
        auto *newStudentInfo = new student;
        getStudents >> newStudentInfo->studentNumber;
        getStudents.ignore(1);
        getStudents.getline(newStudentInfo->name, 25);
        getStudents >> newStudentInfo->amountOfTuition;
        getStudents >> newStudentInfo->numberOfLessons;
        passedLessons *lessonsHead = nullptr;
        //get passedLessons from file
        if (newStudentInfo->numberOfLessons != 0) {
            for (int j = 0; j < newStudentInfo->numberOfLessons; ++j) {
                auto *lessons = new passedLessons;
                getStudents.ignore(1);
                getStudents.get(lessons->lessonName, 25, '1');
                getStudents >> lessons->lessonNumber;
                getStudents >> lessons->amountOfUnit;
                lessons->next = lessonsHead;
                lessonsHead = lessons;
            }
            //add previous pointer
            passedLessons *temp1 = lessonsHead;
            temp1->pre = nullptr;
            passedLessons *temp2 = temp1->next;
            while (temp2 != nullptr) {
                temp2->pre = temp1;
                temp1 = temp2;
                temp2 = temp2->next;
            }
        }
        newStudentInfo->lessons = lessonsHead;
        newStudentInfo->next = studentsInfo;
        studentsInfo = newStudentInfo;
    }
    getStudents.close();
}

void menu() {
    cout << "----------Students Information----------\n";
    while (true) {
        short int selectedNumber1, selectedNumber2;
        char studentNumber[10];
        cout << "Enter a student number : ";
        cin >> studentNumber;
        student *selectedStudent = findSelectedStudent(studentNumber);
        while (selectedStudent == nullptr) {
            cout << "Enter a student number : ";
            cin >> studentNumber;
            selectedStudent = findSelectedStudent(studentNumber);
        }

        while (true) {
            bool wrongNumber2 = true, wrongNumber1 = false;
            cout
                    << "\n1.Show student information\n2.ShowPassedLessons\n3.Add a lesson to passed lessons\n4.Delete a lesson from passed lessons\n5.Exit\nEnter a number to continue : ";
            cin >> selectedNumber1;
            switch (selectedNumber1) {
                case 1:
                    cout << "\nName :\t\t\t\t" << selectedStudent->name;
                    cout << "\nStudent number :\t\t" << selectedStudent->studentNumber;
                    cout << "\nAmount of tuition :\t\t" << selectedStudent->amountOfTuition;
                    cout << "\nNumber of passed lessons :\t" << selectedStudent->numberOfLessons << endl;
                    break;
                case 2:
                    printPassedLessons(selectedStudent);
                    break;
                case 3:
                    addToPassedLessons(selectedStudent);
                    break;
                case 4:
                    deletePassedLesson(selectedStudent);
                    break;
                case 5:
                    writeInFile();
                    deleteLinkList();
                    exit(0);
                default:
                    cout << "\nYou entered wrong number! Please try again\n";
                    wrongNumber1 = true;
            }
            while (!wrongNumber1 && wrongNumber2) {
                wrongNumber2 = false;
                cout << "\n1.Return to menu\n2.Choose a new student\nEnter a number to continue : ";
                cin >> selectedNumber2;
                if (selectedNumber2 != 1 && selectedNumber2 != 2) {
                    cout << "\nYou entered wrong number! Please try again\n";
                    wrongNumber2 = true;
                }
            }
            if (selectedNumber2 == 2)
                break;
        }
    }
}

student *findSelectedStudent(char studentNumber[10]) {
    student *temp = studentsInfo;
    while (strcmp(temp->studentNumber, studentNumber) != 0) {
        temp = temp->next;
        if (temp == nullptr) {
            cout << "\nNot found this student ! Please try again.\n";
            return nullptr;

        }
    }
    cout << temp->studentNumber << " is student number of " << temp->name
         << "\n1.Continue \n2.Choose another student\nEnter a number to continue : ";
    short int selectedNumber;
    cin >> selectedNumber;
    if (selectedNumber == 2)
        return nullptr;
    return temp;
}

bool printPassedLessons(student *selectedStudent) {
    passedLessons *temp = selectedStudent->lessons;
    if (selectedStudent->numberOfLessons != 0) {
        cout << "\nPassed Lessons:\nName\t\t\t\tNumber\t\tUnit\n";
        for (int i = 0; i < selectedStudent->numberOfLessons; ++i) {
            cout << i + 1 << "." << temp->lessonName;
            for (unsigned int b = 30 - strlen(temp->lessonName); b > 0; b--)
                cout << " ";
            cout << temp->lessonNumber << "\t" << temp->amountOfUnit << endl;
            temp = temp->next;
        }
        return true;
    }
    cout << "\nThere are no passed lessons!\n";
    return false;
}

bool isSuggestedLesson(char lessonNumber[10], passedLessons *lessons) {
    for (; lessons != nullptr; lessons = lessons->next) {
        if (strcmp(lessons->lessonNumber, lessonNumber) == 0)
            return false;
    }
    return true;
}

void printSuggestedLessons(suggestedLessons lessons[], int numberOfSuggestedLessons) {
    cout << "\nSuggested Lessons:\nName\t\t\t\tNumber\t\tUnit\tCapacity\n";
    for (int i = 0; i < numberOfSuggestedLessons; ++i) {
        cout << i + 1 << "." << lessons[i].lessonName;
        for (unsigned int b = 30 - strlen(lessons[i].lessonName); b > 0; b--)
            cout << " ";
        cout << lessons[i].lessonNumber << "\t" << lessons[i].amountOfUnit
             << "\t" << lessons[i].admissionCapacity << endl;
    }
}

void addToPassedLessons(student *selectedStudent) {
    suggestedLessons *temp = lessonsInfo;
    int numberOfSuggestedLesson = numberOfLessons - selectedStudent->numberOfLessons;
// find suggested lessons
    suggestedLessons lessonsToPrint[numberOfSuggestedLesson];
    for (int j = 0; j < numberOfSuggestedLesson; j++, temp = temp->next) {
        if (isSuggestedLesson(temp->lessonNumber, selectedStudent->lessons))
            lessonsToPrint[j] = *temp;
        else
            j--;
    }
    if (numberOfSuggestedLesson == 0) {
        cout << "\nThere are no suggested lessons!";
        return;
    }
    printSuggestedLessons(lessonsToPrint, numberOfSuggestedLesson);
    int selectedNumber;
    while (true) {
        while (true) {
            cout << "\nChoose a lesson : ";
            cin >> selectedNumber;
            bool wrongNumber = selectedNumber > numberOfSuggestedLesson || selectedNumber <= 0;
            if (!wrongNumber)
                break;
            cout << "\nYou entered wrong number!Please try again.\n";
        }
        if (lessonsToPrint[selectedNumber - 1].admissionCapacity != 0)
            break;
        if (numberOfSuggestedLesson == 1)
            return;
        cout
                << "\nThe capacity of this lesson has been completed\n1.Choose another lesson\n2.Return to menu\nEnter a number to continue : ";
        cin >> selectedNumber;
        if (selectedNumber == 2)
            return;
    }
    suggestedLessons selectedLesson = lessonsToPrint[selectedNumber - 1];
    selectedStudent->numberOfLessons++;
    selectedStudent->amountOfTuition += selectedLesson.amountOfUnit * 1000;
    passedLessons *temp1 = selectedStudent->lessons;
    passedLessons *temp2 = new passedLessons;
    if (temp1 != nullptr) {
        while (temp1->next != nullptr)
            temp1 = temp1->next;
        temp1->next = temp2;
        temp2->next = nullptr;
        temp2->pre = temp1;
    } else {
        selectedStudent->lessons = temp2;
        temp2->next = nullptr;
        temp2->pre = nullptr;

    }
    strcpy(temp2->lessonName, selectedLesson.lessonName);
    strcpy(temp2->lessonNumber, selectedLesson.lessonNumber);
    temp2->amountOfUnit = selectedLesson.amountOfUnit;
    selectedLesson.admissionCapacity--;
    cout << endl << selectedLesson.lessonName << " Added to passed lessons.\n";
}

void deletePassedLesson(student *selectedStudent) {
    if (!printPassedLessons(selectedStudent))return;
    int selectedNumber;
    while (true) {
        cout << "Choose a lesson to delete :";
        cin >> selectedNumber;
        bool wrongNumber = selectedNumber > selectedStudent->numberOfLessons || selectedNumber <= 0;
        if (!wrongNumber)
            break;
        cout << "\nYou entered wrong number!Please try again.\n";
    }
    passedLessons *selectedLesson = selectedStudent->lessons;
    for (int j = 1; j < selectedNumber; ++j)
        selectedLesson = selectedLesson->next;
    //increment admission capacity
    suggestedLessons *temp = lessonsInfo;
    for (; strcmp(temp->lessonNumber, selectedLesson->lessonNumber) != 0; temp = temp->next);
    temp->admissionCapacity++;
    cout << endl << selectedLesson->lessonName << " deleted from passed lessons\n";
    selectedStudent->numberOfLessons--;
    selectedStudent->amountOfTuition -= selectedLesson->amountOfUnit * 1000;
    passedLessons *nextLesson = selectedLesson->next;
    passedLessons *preLesson = selectedLesson->pre;
    if (nextLesson == nullptr && preLesson != nullptr)
        preLesson->next = nullptr;
    else if (preLesson == nullptr && nextLesson != nullptr) {
        nextLesson->pre = nullptr;
        selectedStudent->lessons = nextLesson;
    } else if (preLesson != nullptr) {
        preLesson->next = nextLesson;
        nextLesson->pre = preLesson;
    }
    delete selectedLesson;
}

void writeInFile() {
    ofstream writeLessons;
    writeLessons.open("lessons.txt");
    writeLessons << numberOfLessons << endl;
    suggestedLessons *temp1 = lessonsInfo;
    for (int i = 0; i < numberOfLessons; ++i) {
        writeLessons << temp1->lessonName << " " << temp1->lessonNumber << " " << temp1->amountOfUnit
                     << " " << temp1->admissionCapacity << endl;
        temp1 = temp1->next;
    }
    writeLessons.close();
    ofstream writeStudents;
    writeStudents.open("students.txt");
    writeStudents << numberOfStudents << endl;
    student *temp2 = studentsInfo;
    for (int j = 0; j < numberOfStudents; ++j) {
        writeStudents << temp2->studentNumber << endl << temp2->name << endl
                      << temp2->amountOfTuition << endl << temp2->numberOfLessons << endl;
        passedLessons *temp = temp2->lessons;
        if (temp2->numberOfLessons != 0) {
            for (int i = 0; i < temp2->numberOfLessons; ++i) {
                writeStudents << temp->lessonName << " " << temp->lessonNumber << " " << temp->amountOfUnit << endl;
                temp = temp->next;
            }
        }
        temp2 = temp2->next;
    }
    writeStudents.close();
}

void deleteLinkList() {
    //delete lessons information
    suggestedLessons *temp1;
    while (lessonsInfo) {
        temp1 = lessonsInfo;
        lessonsInfo = lessonsInfo->next;
        delete temp1;
    }
    //delete students information
    student *temp2;
    while (studentsInfo) {
        temp2 = studentsInfo;
        passedLessons *temp;
        passedLessons *head = studentsInfo->lessons;
        if (studentsInfo->numberOfLessons != 0) {
            //delete passed lessons
            while (head) {
                temp = head;
                head = head->next;
                delete temp;
            }
        }
        studentsInfo = studentsInfo->next;
        delete temp2;
    }
}