/********************************************************************************************************
Title:     Group Project - "Data Mining"

Authors: George Ebeling
         Brendon Dalm
         Tyler Redding
         Tyler Freeman
         Corey Moura

Class:   CIS241
Date:     7/31/2020
Prof:     Dr. Vijay Bhuse

Description:     Write a C program to read in the market data from the text file and draw some
                statistical conclusions from the data. There are no obvious right or wrong answers.
                Your grade depends on:

                    Following items are to be included in your report:
                    •    Source code
                    •    Output supporting conclusions drawn
                    •    Video demo
                    •    Report citing contributions of team members


Notes:
        PROGRAM NOTES:
            DECLARE THE STRUCTURE BEFORE THE FUNCTIONS
            Line count using terminal: wc -l marketData.txt -> determines MACRO NUMENTRIES

            Function Flow:
                Main > importFile >    advanceCursor >  REPEATS: extractLine > formatLine > storeLine

            Data Flow:
                .txt File > imported one line at a time as strings > extract nums from strings...
                and convert to doubles > Store each line of data into the structure array

                    -> Structure array (dataArr) conatins all of the data

        PUT / CALL NOTES:
            The data is a weekly representation of total puts and calls.

            Putt/Call Ratio = (putVolume /  callVolume)
                A falling put-call ratio below .7 and approaching .5, is considered a bullish indicator
                A rising put-call ratio greater than .7 or exceeding 1, is considered a bearish indicator

********************************************************************************************************/

/* C libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* MARCOS */
#define DECIMAL 46              // ASCii value for '.'
#define NUMENTRIES 2332     // Lines of data

/* Order of variables from largest to smallest data type  to reduce memory consumption */
struct dataEntry{

    double putVol;
    double callVol;
    double optVol;
    float ratio;
    int year;
    unsigned int month : 4;
    unsigned int day : 5;

};

/* Program functions */
void importFile(struct dataEntry dataArr[NUMENTRIES]);
void advanceCurserToData(char *buff, FILE *fp);
void extractLine(char *buff, double *numberArr);
int formatLine(char *tempArr);
void storeLine(double *lineOfData, struct dataEntry dataArr[NUMENTRIES]);

/* Global variables */
int i = 0, j = 0;
int logNum = 0;



int main() {

    struct dataEntry dataArr[NUMENTRIES];

    importFile(dataArr);

    unsigned int dataArrSize = NUMENTRIES - 3;  // Location in the structure array of the last line of data
    //    printf("%.2f\t", dataArr[NUMENTRIES - 3].optVol);


}


/*************************************************************************************************************************
* IMPORT THE FILE:  Calls to  the functions to extract each line of data from the file and store it in the structures
* variables.  Each line of data is its own structure.  The structures of data are stored in an array of structures named
* "dataArr".  fscanf reads up to next whitespace (up to the end of line in this case).
*************************************************************************************************************************/
void importFile(struct dataEntry dataArr[NUMENTRIES]){

    double lineOfData[7];

    FILE *fp;
    fp = fopen("marketData.txt", "r");
    char buff[40];

    rewind(fp);     // Ensure the curser is at the begining of the file

    advanceCurserToData(buff, fp);

    while(!feof(fp)){

        extractLine(buff, lineOfData);
        storeLine(lineOfData, dataArr);
        fscanf(fp, "%s", buff);
        logNum++;

    }

    fclose(fp);

    return;
}


/*************************************************************************************************************************
* ADVANCE THE CURSOR:  The begining of the text file has column titles which are not part of the data.  This will move
* the cursor (file pointer fp) to the begining of the first line of data.
*************************************************************************************************************************/
void advanceCurserToData(char *buff, FILE *fp){

    for(i = 0; i <= 10; i++)
        fscanf(fp, "%s", buff);
    return;
}


/*************************************************************************************************************************
* EXRTRACT NUMBERS FROM STRING:  Convert the numbers from the string (line of data) to double values.  Store the
* numerical values in the array "lineOfData[]".  Will extract all numbers from the string.
*************************************************************************************************************************/
void extractLine(char *buff, double *lineOfData){

    char tempArr[100];
    int size = formatLine(strcpy(tempArr, buff));

    char *ptrArray[20] = {tempArr};

    for(i = 0; i <= 6 ; i++){

        lineOfData[i] = strtod(ptrArray[i], &ptrArray[i+1]);
        size = strlen(ptrArray[i+1]);
    }
    return;
}


/*************************************************************************************************************************
* FORMAT STRING FOR THE strtod() FUNCTION: The strtod() function is weak and requires specific formating for it to
* successfully process a string therefore all punctuation is replace by whitespace except for '.' for decimal nums
*************************************************************************************************************************/
int formatLine(char *tempArr){
    int size = strlen(tempArr);

    for(i = 0; i <= size; i++)
        if(ispunct(tempArr[i]))
            (tempArr[i] == DECIMAL)  ?  (tempArr[i]) :  (tempArr[i] = ' ');

    return strlen(tempArr);
}


/*************************************************************************************************************************
* STORE THE LINE OF DATA: Store the parsed data from the line into the strucutre array using it's corresponding index.
*************************************************************************************************************************/
void storeLine(double *lineOfData, struct dataEntry dataArr[NUMENTRIES]){

    dataArr[logNum].month   = lineOfData[0];
    dataArr[logNum].day     = lineOfData[1];
    dataArr[logNum].year    = lineOfData[2];
    dataArr[logNum].ratio   = lineOfData[3];
    dataArr[logNum].putVol  = lineOfData[4];
    dataArr[logNum].callVol = lineOfData[5];
    dataArr[logNum].optVol  = lineOfData[6];

    return;
}





/* DEBUG PRINT STATEMENTS
    printf("\n%d\t", dataArr[NUMENTRIES - 3].month);
    printf("%d\t",   dataArr[NUMENTRIES - 3].day);
    printf("%d\t",   dataArr[NUMENTRIES - 3].year);
    printf("%.2f\t", dataArr[NUMENTRIES - 3].ratio);
    printf("%.2f\t", dataArr[NUMENTRIES - 3].putVol);
    printf("%.2f\t", dataArr[NUMENTRIES - 3].callVol);
    printf("%.2f\t", dataArr[NUMENTRIES - 3].optVol);
*/

