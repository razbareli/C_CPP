#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 60
#define DIV 10
#define LARGE 9999999999
#define SMALL 1000000000
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120


typedef struct Student{
    long int id;
    int grade;
    int age;
}Student;


void print_arr(Student* start, Student* end){
    while (start < end ) {
        printf("%ld,%d,%d\n", start->id,start->grade, start->age);
        start++;
    }
}

void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

Student *partition(Student *start, Student *end) {
    Student *pivot = end;
    Student *p1 = (start - 1);
    Student *p2;
    for (p2 = start; p2 < pivot; p2++) {
        if (p2->age <= pivot->age) {
            p1++;
            swap(p1, p2);
        }
    }
    swap(p1 + 1, pivot);
    return p1 + 1;
}

void quick_sort(Student *start, Student *end) {
    if (start < end) {
        Student *piv = partition(start, end);
        quick_sort(start, (piv - 1));
        quick_sort((piv + 1), end);
    }
}


void bubble_sort(Student *start, Student *end){
    Student *i, *j;
    for (i = start; i < end; ++i){
        for(j = start; j < end -1; ++j){
            if(j -> grade > (j+1) -> grade) {
                swap(j , (j+1));
            }
        }
    }
    print_arr(start, end);
}


void best_student(Student *start, Student *end) {
    Student *temp = start;
    float best_score = 0;
    int pos = 0;
    int counter = 0;
    while (temp - end != 0) {
        float x = (float) (temp->grade) / (float) (temp->age);
        if (best_score < x){
            best_score = x;
            pos = counter;
        }
        temp++;
        counter++;
    }
    printf("best student info is: %ld,%d,%d\n",
           (start + pos)->id, (start + pos)->grade, (start + pos)->age);
}


int main(int argc, char *argv[]){
    int best = strcmp(argv[1], "best");
    int bubble = strcmp(argv[1], "bubble");
    int quick = strcmp(argv[1], "quick");
    if (argc > 2 || ((best != 0) && (bubble != 0) && (quick != 0))){
        printf("USAGE: optional arguments: best, bubble, quick");
        return 1;
    }
    char input[MAX] = {0};
    char *ptr;
    int num_of_students;
    int check1;
    while (1){
        check1 = 1;
        printf("Enter number of students. Then enter\n");
        if (fgets(input, MAX, stdin) == NULL){
            return 1;
        }
        for (int i=0; i <= (int)strlen(input); i++){
            if (('a' <= input[i] && input[i] <= 'z')
                || ('A' <= input[i] && input[i] <= 'Z')){
                printf("ERROR: Characters are not allowed\n");
                check1 = 0;
                break;
            }
        }
        if(check1 == 0){
            continue;
        }
        num_of_students = strtol(input, &ptr, DIV);
        if(num_of_students>0){
            break;
        }
        else{
            printf("ERROR: number of student"
                   " should be a positive integer\n");
        }
    }
    Student *sptr = malloc(sizeof(Student) * (num_of_students));
    if (sptr == NULL) {
        free(sptr);
        return 1;
    }
    Student *temp = sptr;
    Student *end = sptr + num_of_students;
    while (temp - end != 0) {
        char info[MAX];
        int check2;
        while (1) {
            check2 = 1;
            printf("Enter student info. Then enter\n");
            if (fgets(info, MAX, stdin) == NULL){
                printf("ERROR: fgets failed\n");
                free(sptr);
                return 1;
            }
            int i;
            for (i=0; i <= (int)strlen(info); i++){
                if (('a' <= info[i] && info[i] <= 'z')
                || ('A' <= info[i] && info[i] <= 'Z')){
                    printf("ERROR: Characters are not allowed\n");
                    check2 = 0;
                    break;
                }
            }
            if(check2 == 0){
                continue;
            }
            int num = sscanf(info, "%ld,%d,%d", &(temp->id),
                             &(temp->grade), &(temp->age));
            if(num == 0 || num == EOF){
              printf("ERROR: sscanf failed\n");
              free(sptr);
              return 1;
            }
            if ((temp->id) > (long)(LARGE) || (temp->id)
            / SMALL == 0 || info[0] == '0'){
                printf("ERROR: Id should be"
                       " 10 ints and first int != 0\n");
            } else if ((temp->grade) < 0 || (temp->grade) > MAX_GRADE) {
                printf("ERROR: Grade should be an integer"
                       " between 0 and 100\n");
                continue;
            } else if ((temp->age) < MIN_AGE || (temp->age) > MAX_AGE) {
                printf("ERROR: Age should be an integer"
                       " between 18 and 120\n");
                continue;
            } else {
                break;
            }
        }
        temp++;
    }
    if (strcmp(argv[1], "best") == 0){
        best_student(sptr, end);
    }
    else if (strcmp(argv[1], "bubble") == 0){
        bubble_sort(sptr, end);
    }
    else if (strcmp(argv[1], "quick") == 0){
        quick_sort(sptr, end-1);
        print_arr(sptr, end);
    }
    free((sptr));
    (sptr) = NULL;

    return 0;
}
