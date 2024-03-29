#include<fstream>
using std::ofstream, std::ifstream;
#include <cctype>
using std::isalpha;
#include<iostream>
using std::cout,std::endl,std::cerr;
#include<string>
using std::stoi,std::string;
#include <array>
using std::array;
#include<sstream>
using std::stringstream;
#include<iterator>
using std::istreambuf_iterator;

//array has been used instead of vectors for efficiency as we need a fixed size container.
// size of matrix has been fixed at 3x3 as a design decision
using matrix= array<array<int,3>,3>;

//turning upper case, removing special case , making every word divisibke by 3 by appending Z.
stringstream preProcessText(string & text){
    string onlyCapitals;
    for (char & ch:text){
        if (isalpha(ch)|| ch==' '|| ch=='\t' || ch == '\n'){
        ch=toupper(ch);
        onlyCapitals+=ch;
        }
    }

    //appending each word with z to make its length divisible by 3(choosen length of vector in which string is to be divided)
    stringstream raw(onlyCapitals);
    stringstream processed;
    string word;
    while (raw>>word)
    {
       auto shortfall= 3-(word.length()%3); //shortfall in number of characters to be divisible by 3
       shortfall==3?shortfall=0:shortfall;
       for (auto i=0; i<shortfall;++i){
            word+='Z';
       }
       processed<<word<<" ";
    }
    return processed;
}

void generateKeyMatrix(matrix & key, const string &  key_text){
    char base='A';
    unsigned string_index=0; 
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j <3; j++)
        {
            key[i][j]=(key_text[string_index++]-base);
        }
        
    }
    
}

//design decision : neglecting anything else than english alphabets.
string hillEncrypt(const matrix& key,stringstream & textStream){
    string encrypted_text;
    array<unsigned,3> textVector; 
    char base='A';
    string text;
    
    while (textStream>>text)
    {   
        auto size=text.length();
        for (unsigned i = 0; i <size ; i+=3)
        {
            textVector[0]=(text[i]-base);
            textVector[1]=(text[i+1]-base);
            textVector[2]=(text[i+2]-base);
        

        unsigned sum=0;
        for(unsigned row=0;row<3;++row ){
            if (sum!=0){encrypted_text+=static_cast<char>((sum%26)+base);}
            sum=0;
            for (unsigned col=0;col<3;++col){
                sum+=key[row][col]*textVector[col];
            }
        }
        encrypted_text+=static_cast<char>((sum%26)+base); // to add the last character left due to exit from for loop
        }
        encrypted_text+=" ";
    }
    return encrypted_text;
}

int main(int argc, char**argv){
    if (argc!=3){
        cout<<"Usage: "<<argv[0]<<"name of Key file(3x3)[SPACE]name of text file"<<endl;
        return -1;
    }
    ifstream key_file(argv[1]);
    if (!key_file){ 
        cerr<<"error opening the key file"<<endl;
        return -1;
    }
    string key_text;
    std::getline(key_file,key_text);

    key_file.close();

    auto processedKeyStream=preProcessText(key_text);
    string keyText;
    processedKeyStream>>keyText;
    matrix key;
    generateKeyMatrix(key,keyText);
    // // for (size_t i = 0; i < 3; i++)
    // {
    //     for (size_t j = 0; j <3; j++)
    //     {
    //         cout<<key[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
   

    ifstream text_file(argv[2]);
    if (!text_file){ 
        cerr<<"error opening the text file"<<endl;
        return -1;
    }
    string plainText{istreambuf_iterator<char>(text_file),istreambuf_iterator<char>()};
    stringstream processedTextStream= preProcessText(plainText);

    text_file.close();

    ofstream output("hillcipher_output.txt");
     if (!output){ 
        cerr<<"error generating the output file";
        return -1;
    }
    output<<hillEncrypt(key,processedTextStream);
    output.close();
    cout<<"Encrypted text successfully saved in hillcipher_output.txt in the current drive "<<endl;

}