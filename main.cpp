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
    int words = 0; //Number of words found so far
    
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
    
    //Check all ENABLE words, add then to foundWords
    for(int i=0;i<172820;i++){
        line = wordArray[i];
        found = checkWord(line,gameBoard);
        if(found.startPos!=-1)
            cout << found.word << endl;
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
    
    //This position is now part of the word
    searched[position]=letter+1;
    
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