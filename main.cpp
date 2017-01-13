/* 
 * File:   main.cpp
 * Author: Desmond
 *
 * Created on December 25, 2016, 5:08 PM
 */

//This somehow helps windows.h import correctly, do not remove
#define _WIN32_WINNT 0x0500

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

//Object that specifies where a word is located on the board
class wordPos{
public:
    int startPos;
    string word;
    int position[16];
    wordPos();
    
    void operator=(const wordPos &rhs){
        startPos = rhs.startPos;
        word = rhs.word;
        for(int i=0;i<16;i++)
            position[i] = rhs.position[i];
    }
};
wordPos::wordPos(void){
    startPos=-1;
    word=" ";
    for(int i=0;i<16;i++)
        position[i]=0;
}

//Given a word and a board, sees if it exists and returns wordPos
wordPos checkWord(string word, char board[16]);
//Helper function to check each letter in the word
int checkLetter(string word, char board[16], int searched[16], int letter, int position);
//Gets the (no bonus) score for a word
int getScore(string word);

int main(int argc, char** argv) {
    
    //Variables
    string line; //Generic string input
    string wordArray[172820]=" "; //Stores the ENABLE list
    char gameBoard[16]=" "; //Stores the board
    int valid = 0; //Generic bool
    wordPos found; //Stores found words
    int score = 0; //Total score, used for board resets
    int targetScore = 0; //Target score (0 for none)
    
    //Store the ENABLE word list into an array
    ifstream wordList ("enable1.txt");
    if (wordList.is_open()){
        int i=0;
        while(getline(wordList,line)){
            wordArray[i] = line;
            i++;
        }
        wordList.close();
    }
    else
        cout << "Unable to open file." << endl;
    
    //Loop for inputting board states
    while(1){
        //Reset variables
        valid = 0;
        
        //Get input and initialize the board state
        cout << "Target Score? (0 for none)" << endl;
        cin >> targetScore;
        cout << "Input Board State:" << endl;
        while(!valid){
            cin >> line;
            if(line.size()==16 || line.size()==1)
                valid=1;
            if(!valid)
                cout << "Incorrect Length." << endl;
        }
        
        //If user inputted a single character line, end program
        if(line.size()==1)
            break;
        
        //Otherwise, initialize the board
        for(int i=0;i<16;i++)
            gameBoard[i] = line[i];

        //Check all ENABLE words, draw them on the board
        for(int i=0;i<172820;i++){
            //Variables for word checking
            line = wordArray[i];
            found = checkWord(line,gameBoard);

            //Variables for automated word entering
            int xPos[16];
            int yPos[16];
            int wordPos, row, col;
            INPUT input;

            if(found.startPos!=-1){
                cout << found.word << endl;

                //Convert the wordPos to a series of mouse coordinates
                for(int j=0;j<16;j++){
                    wordPos = found.position[j];
                    row = j/4;
                    col = j%4;
                    if(wordPos!=0){
                        xPos[found.position[j]-1] = 11050+3600*col;
                        yPos[found.position[j]-1] = 30500+6450*row;
                    }
                }

                //Draw the word on the board
                for(int k=0;k<found.word.size();k++){
                    input.type=INPUT_MOUSE;
                    input.mi.dx=xPos[k];
                    input.mi.dy=yPos[k];
                    if(k==0)
                        input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE|MOUSEEVENTF_LEFTDOWN);
                    else if(k==found.word.size()-1)
                        input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE|MOUSEEVENTF_LEFTUP);
                    else
                        input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE);
                    input.mi.mouseData=0;
                    input.mi.dwExtraInfo=NULL;
                    input.mi.time=0;
                    SendInput(1,&input,sizeof(INPUT));

                    Sleep(25);
                }

                //Add the word's score to the tally
                score = score+getScore(found.word);
            }

            //If we have reached our target score, stop
            if(targetScore!=0)
                if(score>=targetScore)
                    break;

            //If user presses "q", stop
            if(GetAsyncKeyState(113) & 0x8000 )
                break;
        }
    }
    
    return 0;
}

wordPos checkWord(string word, char board[16]){
    //Variables
    int searched[16] = {0}; //Tracks location of the word
    int startPos = 0; //Starting position of the word
    wordPos found;
    int check = 0;
    
    while(startPos<16 && !check){  
        if(board[startPos]==word[0]){
            check = checkLetter(word, board, searched, 0, startPos);
            if(check){
                found.startPos = startPos;
                for(int i=0;i<16;i++)
                    found.position[i] = searched[i];
                found.word = word;
            }
        }
        startPos++;
    }
    return found;
}

int checkLetter(string word, char board[16], int searched[16], int letter, int position){
    //Indicates whether we have found the word
    int found = 0;
    
    //Position in row and column of board
    int row = position/4;
    int col = position%4;
    
    //This position is now searched (17 used as placeholder)
    searched[position]=17;
    
    if(letter==word.size()-1)
        found=1;
    else{
        if(row!=0&&col!=0&&found==0)
            if(board[position-5]==word[letter+1]&&searched[position-5]==0)
                found = checkLetter(word,board,searched,letter+1,position-5);
        if(row!=0&&col!=3&&found==0)
            if(board[position-3]==word[letter+1]&&searched[position-3]==0)
                found = checkLetter(word,board,searched,letter+1,position-3);
        if(row!=3&&col!=0&&found==0)
            if(board[position+3]==word[letter+1]&&searched[position+3]==0)
                found = checkLetter(word,board,searched,letter+1,position+3);
        if(row!=3&&col!=3&&found==0)
            if(board[position+5]==word[letter+1]&&searched[position+5]==0)
                found = checkLetter(word,board,searched,letter+1,position+5);
        if(row!=0&&found==0)
            if(board[position-4]==word[letter+1]&&searched[position-4]==0)
                found = checkLetter(word,board,searched,letter+1,position-4);
        if(row!=3&&found==0)
            if(board[position+4]==word[letter+1]&&searched[position+4]==0)
                found = checkLetter(word,board,searched,letter+1,position+4);
        if(col!=0&&found==0)
            if(board[position-1]==word[letter+1]&&searched[position-1]==0)
                found = checkLetter(word,board,searched,letter+1,position-1);
        if(col!=3&&found==0)
            if(board[position+1]==word[letter+1]&&searched[position+1]==0)
                found = checkLetter(word,board,searched,letter+1,position+1);
    }
    
    //If we found the word, add this position to it
    if(found)
        searched[position]=letter+1;
    
    return found;
}

int getScore(string word){
    int score = 0;
    int letter = 0;
    int letterScores[26] =
    {1,4,4,2,1,4,3,3,1,10,5,2,4,2,1,4,10,1,1,1,2,5,4,8,3,10};
    
    for(int i=0;i<word.size();i++){
        letter = (int)word[i];
        letter = letter-97; //a=0, b=1, c=2, etc.
        score += letterScores[letter];
    }
    return score;
}