#include<iostream>
using std::cout,std::endl,std::cin,std::cerr;
#include<string>
using std::string,std::getline;
#include<fstream>
using std::ifstream,std::ofstream;
#include<unordered_map>
using std::unordered_map;
#include<cctype>
using std::isalpha, std::toupper;
#include<iterator>
using std::istreambuf_iterator;
#include <array>
using std::array;
#include<utility>
using std::pair;
#include<sstream>
using std::stringstream;

//we use a hash map to store locations of alphabets instead of searching everytime in the array
using charMap= unordered_map<char,pair<unsigned,unsigned>>;
using matrix = array<array<char,5>,5>;

//text is processed to capitalise all alphabets and process 'j/J' before actual encryption
void preProcessText(string & text){
    for (char & ch:text){
        if (isalpha(ch)){ch=toupper(ch);}
        if (ch=='J'){ch='I';}
    }
}

void generatePlayfairMatrix(const string & key, matrix & playfairMatrix, charMap & charPositions){
          string KeyWithDuplicates = key + "ABCDEFGHIKLMNOPQRSTUVWXYZ";
          auto it =KeyWithDuplicates.begin();
          unsigned i=0,j=0;
          while(i<5){
             j=0;
             while (j<5){
                if (charPositions.insert({*it,{i,j}}).second){
                         playfairMatrix[i][j]=*it;
                         ++j;
                         ++it;
                         if (j==5){++i;}
         }
             else{++it;}
      }  
  }
}

string encryptText(const string & plain_text, const matrix& playfairMatrix, const charMap& charPositions){
    string encryptedText;
    stringstream words(plain_text);
    string word;

    while (words>>word)
    {    
    //storing the size of plain_text in a variable to avoid the overhead of calculating it for every iteration
    auto text_size=word.length();
    encryptedText+=" "; //There should be space after every word.

    for(string::size_type i=0; i<text_size;i+=2){
        char firstChar=word[i];
        while(!isalpha(firstChar))
        {
            encryptedText+=firstChar;
            ++i; 
            if(i==text_size){break;}
            firstChar=word[i];
        }
        if(i==text_size){break;}
        
        char secondChar= i+1<text_size?word[i+1]:'X';

        // Design decision: in case of strings like a,a,a, it is encrypted as ax,ax,ax
        if (firstChar==secondChar || !isalpha(secondChar)){
            secondChar='X';
            --i;
        }

        auto it1=charPositions.find(firstChar);
        auto it2=charPositions.find(secondChar);
        auto end=charPositions.end();

        //The check shouldn't be normally required cause we have made sure beforehand that both chars are alphabets.
        if(it1!=end && it2!=end){
            auto [row1,col1]=it1->second; //structured binding . introduced in C++17
            auto [row2,col2]=it2->second;

            if (row1==row2){
                encryptedText+=playfairMatrix[row1][(col1+1)%5];
                encryptedText+=playfairMatrix[row1][(col2+1)%5];
            }
            else if (col1==col2)
            {
                encryptedText+=playfairMatrix[(row1+1)%5][col1];
                encryptedText+=playfairMatrix[(row2+1)%5][col1];
            }
            else{
                encryptedText+=playfairMatrix[row1][col2];
                encryptedText+=playfairMatrix[row2][col1];
            }
            
        }
        else{ cerr<<"Some unexpected error encountered while encrypting "<<it1->first<<" and "<<it2->first<<endl;}

        
    }
    }
    return encryptedText;
}

int main(int argc, char**argv){
    if(argc!=2){
        cerr<<"Usage: "<<argv[0]<<"  name of input file";
        return -1;
    }
    ifstream input(argv[1]);
    if (!input){ 
        cerr<<"error opening this file";
        return -1;
    }

    string plain_text {istreambuf_iterator<char>(input),istreambuf_iterator<char>()}; //default iterator refers to end of input iterator
                     // note the brace(list) initialisation. initialisation using "()" doesn't work.
    input.close();

    preProcessText(plain_text);

    cout<<"Enter the Playfair Key('j/J' will be replaced with 'I')"<<endl;
    string key;
    cin>>key;
    if(cin.fail()){
        cout<<"A problem encountered in processing the input!\n Please Re-input! "<<endl;
        cin.clear();
        cin>>key;
        if(cin.fail()){ cout<<"Sorry! input keeps encountering some error. Please re-start the program";}
    }
    preProcessText(key);
    matrix playfairMatrix;
    charMap charPositions;
    generatePlayfairMatrix(key,playfairMatrix,charPositions);
    
    ofstream output("playfair_output.txt");
    if (!output){ 
        cerr<<"error generating the output file";
        return -1;
    }
    output<<encryptText(plain_text,playfairMatrix,charPositions);
    output.close();
    cout<<"Encrypted text stored successfully in 'playfair_output.txt'! "<<endl;
    // for (int i=0;i<5;++i){
    //     cout<<endl;
    //     for(int j=0;j<5;++j){
    //         cout<<playfairMatrix[i][j]<<'\t';
    //     }
    // }

    
}