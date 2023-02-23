#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <string.h>

void sort(std::vector<std::string> &nameArr, std::vector<int> & count, int size){
    for(int k= 0; k < size; k++){
       for(int j = k+1; j < size; j++){
           if(nameArr[k] > nameArr[j]){
                std::string temp = nameArr[k];
                nameArr[k] = nameArr[j];                          //sorting function to reorder alphabetically the final output
                nameArr[j] = temp;                                //the following algorithm for this function is a bubble sort since  
                int temps = count[k];                             //we dont have many items to work with it.
                count[k] = count[j];
                count[j] = temps;
            }
       } 
    }
}

bool filledwithletters(std::string word){ 

	for (int i = 0; i < word.size(); i++)
	{
		int lower = tolower(word[i]); //to lowercase letter
        if (lower < 'a' || lower > 'z') //If character is not a-z         //This function is used to check if only strings occupy the space. 
		{                                                                 //If word is filled with numbers then it will return false, else true.
			return false;
		}
	}

	return true;
}

int main(int argc, char *argv[]){


    FILE *inputfile,*outputfile; //input file and output file from terminal;
    inputfile = fopen(argv[1], "r"); 
    outputfile = fopen(argv[2],"w");
    char buf[60960];  //I used 60960 since the the number of characters seem to be big enough for this problem. 
    memset(buf, '\0', 60960);  //seting the buffer to '\0'
    fread(&buf, sizeof(char), 60960, inputfile);  //read file from terminal to buffer.          
    //forloop to get all info which is usable from the buffer.
    std::string userInput;
    for(int i=0;i<60960;i++){
        if((buf[i] >= '!' && buf[i] <= '/')||(buf[i] >= ':' && buf[i] <= '@')||(buf[i] >= '[' && buf[i] <= '`')|| (buf[i] >= '{' && buf[i] <= '~')){
            buf[i] = ' ';
        }
        if(buf[i] == ' ' || buf[i] == '\0'){
            if((buf[i-1] >= 'a' && buf[i-1] <= 'z') || (buf[i-1] >= 'A' && buf[i-1] <= 'Z') || (buf[i-1] >= '0' && buf[i-1] <= '9')){
                userInput = userInput + ' ';
            }
        }
        else if(buf[i] == '\n'){
            userInput = userInput + ' ';
        }
        else{
            userInput = userInput + buf[i];
        }
    }

    if(userInput.empty()){
        std::cout << "the file is empty" << std::endl;
        return 0;
    }

    //lines 49-64 is a for loop going through and removing punctuation marks and empty lines and replacing them with spaces. 
    
    //std::cout << userInput << std::endl;

    
    std::vector<std::string> userwords = {};
    std::string word = "";
    for(int i = 0;i<(userInput.length());i++){
        if((userInput[i] >= 'a' && userInput[i] <= 'z') || (userInput[i] >= 'A' && userInput[i] <= 'Z') || (userInput[i] >= '0' && userInput[i] <= '9')){
            word = word + userInput[i];
        }
        if(userInput[i] == ' '){
            if(!word.empty()){
                std::remove(word.begin(), word.end(), ' ');
                userwords.push_back(word);
                word = "";
            }
        }
    }
    
   //Lines 71-84 is a for loop used to create the words. I used Vectors to store the words. I put used spaces as a breaking point for each word. 

    std::vector<std::string> wordarr = {};
    
    for(int i=0;i<(userwords.size());i++){
       if(filledwithletters(userwords[i])){
         wordarr.push_back(userwords[i]);
       }
    }

    //lines 90-94 is a for loop which checks if the words created in the previous vector is a word. 
    //if it is a word then it stores it in another vector called wordarr.

    for (int i = 0; i < (wordarr.size()); i++)
    {
        transform(wordarr[i].begin(), wordarr[i].end(), wordarr[i].begin(), ::tolower); // from https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
    }
    
    //lines 99-102 is a for loop making each word lowercase.

    int NumOfWords = wordarr.size();
    std::vector<std::string> wordtitle = {}; //words found
    std::vector<int> wordcount = {}; //frequency of the words

    for(int i = 0;i<(wordarr.size());i++){
        wordtitle.push_back(wordarr[i]);
        wordcount.push_back(0);
    }
    
    
    for(int i = 0;i<NumOfWords;i++){
        for(int j = 0;j<NumOfWords;j++){
            if (!wordtitle[i].compare(wordarr[j])){ 
                wordcount[i] += 1;
            }
        }
    }

    //lines 110-122 is a for loop which is counting how many times a certin word shows up and 
    //the results are put in another vector called wordcount

    
    int maxim = 0;
    bool inornot = 0;
    std::vector<std::string> maxwordtitle = {}; //words found
    std::vector<int> maxwordcount = {}; //frequency of the words
    for(int i=0;i<NumOfWords;i++){
        if(wordcount[i] == maxim){
            for(int j = 0;j<maxwordcount.size();j++){
                if(wordtitle[i] == maxwordtitle[j]){
                    inornot = 1;
                }
            }
            if(inornot == 0){
                maxwordcount.push_back(wordcount[i]);
                maxwordtitle.push_back(wordtitle[i]);
                maxim = wordcount[i];
            }
        }
        else if(wordcount[i] > maxim) {
            maxwordcount.clear();
            maxwordtitle.clear();
            maxwordcount.push_back(wordcount[i]);
            maxwordtitle.push_back(wordtitle[i]);
            maxim = wordcount[i];
        }
    }

    //lines 128-152 this is a for loop going through the counts and finding the word that showed up the most. 

    sort(maxwordtitle,maxwordcount,maxwordcount.size());

    //line 56 sorts the the vector of the max counts.

    std::string tofile; 
    
    for(int i = 0;i<(maxwordcount.size());i++){
        tofile = tofile + maxwordtitle[i] + " " + std::to_string(maxwordcount[i]) + "\n";
    }

    //lines 162-164 add the word and count of the word to a string 


    std::ofstream out(argv[2]);
    out << tofile;

    //lines 169 and 170 copy the string to the output file. 
}