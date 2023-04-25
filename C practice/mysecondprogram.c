#include <stdio.h>

int main(){
    /*
    for(int i = 0; i < 4; i++){
        printf("-");
    }
    printf("*\n");

    for(int i = 0; i < 3; i++){
        printf("-");
    }
    printf("* *\n");
    */


/*    for(int i = spaces; i >= 0; i--){
        for(int j = 0; j < i; j++){
            printf("-");
        }
        int numStars = (spaces - i)+1;
        for (int k = 0; k < numStars; k++){
            printf("* ");
        }
        printf("\n");
    }

    for(int i = 0; i <= spaces; i++){

        int numStars = (spaces - i)+1;
        for (int k = numStars; k > 0; k--){
            printf("* ");
        }
        printf("\n");
    }
*/

    printDiamond(4);
    return 0;
}

void printDiamond(int spaces){
    for (int i = spaces; i >= 0; i--){
        PrintSpaces(i);
        int numStars = CalcNumStars(spaces, i);
        for (int k = 0; k < numStars; k++){
            printf("* ");
        }
        printf("\n");
    }

    for (int i = 0; i <= spaces; i++){
        PrintSpaces(i);
        int numStars = CalcNumStars(spaces, i);
        for (int k = numStars; k > 0; k--){
            printf("* ");
        }
        printf("\n");
    }
}

void PrintSpaces(int i){
    for (int j = 0; j < i; j++){
        printf("-");
    }
}

int CalcNumStars(int spaces, int i){
    return (spaces - i) + 1;
}

//----*
//---* *
//--* * *
//-* * * *
//* * * * *
//* * * * *
//-* * * *
//--* * *
//---* *
//----*

//23 minutes
