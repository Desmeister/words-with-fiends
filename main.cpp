/* 
 * File:   main.cpp
 * Author: Desmond
 *
 * Created on December 25, 2016, 5:08 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Given a word and a board, sees if it exists and returns start position
int checkWord(string word, char board[16]);
//Helper function to check each letter in the word
int checkLetter(string word, char board[16], int searched[16], int letter, int position);
//Gets the (no bonus) score for the words
int getScore(string word);

int main(int argc, char** argv) {
    
    //Variables
    string line;
    string wordArray[172820]=" ";
    char gameBoard[16]=" ";
    int valid = 0;
    
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
    
    //Get input and initialize the board state
    cout << "Input Board State:" << endl;
    while(!valid){
        cin >> line;
        if(line.size()==16)
            valid=1;
        if(!valid)
            cout << "Incorrect Length." << endl;
    }
    for(int i=0;i<16;i++)
        gameBoard[i] = line[i];
    
    //Check all ENABLE words, print results
    for(int i=0;i<172820;i++){
        line = wordArray[i];
        valid = checkWord(line,gameBoard);
        if(valid!=-1&&getScore(line)>=10){
            if(valid<10)
                cout << valid << "  " << line << endl;
            else
                cout << valid << " " << line << endl;
        }
    }
    
    return 0;
}

int checkWord(string word, char board[16]){
    //Variables
    int searched[16] = {0}; //Which squares are already part of the word
    int startPos = 0; //Starting position of the word
    int foundPos = -1; //-1 if not found, otherwise location of start of word
    int check = 0;
    
    while(startPos<16 && check==0){  
        if(board[startPos]==word[0]){
            check = checkLetter(word, board, searched, 0, startPos);
            if(check==1)
                foundPos = startPos;
        }
        startPos++;
    }
    
    if(check==1)
        return foundPos;
    
    return -1;
}

int checkLetter(string word, char board[16], int searched[16], int letter, int position){
    //Indicates whether we have found the word
    int found = 0;
    
    //Position in row and column of board
    int row = position/4;
    int col = position%4;
    
    //This position is now part of the word
    searched[position]=1;
    
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