#include <stdio.h>

int main(){
    printf("Hello world!\n");
    const int myNum = 15; //read only var
    char myChar = 'A';
    printf("myNum: %d\nmyChar: %c\n", myNum, myChar);

    int myNum2 = 20;
    int sum = myNum + myNum2;
    printf("Sum: %d\n", sum);
    sum += 1;
    printf("plus: %d\n", sum);

    if (myNum > myNum2){
        printf("myNum big\n");
    } else {
        printf("myNum small\n");
    }

    int day = 4;
    switch(day){
        case 4:
        printf("Thursday\n");
        break;
    }

    int i = 0;
    while (i < 5){
        i += 1;
        printf("%d\n", i);
    }

    for(int j = 0; j < 4; j++){
        printf("B\n");
    }

    int myNumbers[] = {25,50,75,100};
    for(int k = 0; k < sizeof(myNumbers) / sizeof(int); k++){
       printf("%d\n", myNumbers[k]);
    }

    int myNumbers2[4];
    myNumbers2[0] = 25;
    myNumbers2[1] = 50;
    myNumbers2[2] = 75;
    myNumbers2[3] = 100;

    int matrix[2][3] = { {1, 4, 2}, {3, 6, 8} };
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 3; j++) {
        printf("%d\n", matrix[i][j]);
      }
    }

    char greetings[] = "Hello World!";
    printf("%s\n", greetings);

    int myAge = 43;
    printf("%d\n", myAge);  // Outputs the value of myAge (43)
    printf("%p\n", &myAge); // Outputs the memory address of myAge

    int* ptr = &myAge;
    // Output the memory address of myAge with the pointer (0x7ffe5367e044)
    printf("%p\n", ptr);
    // Dereference: Output the value of myAge with the pointer (43)
    printf("%d\n", *ptr);

    return 0;
}
