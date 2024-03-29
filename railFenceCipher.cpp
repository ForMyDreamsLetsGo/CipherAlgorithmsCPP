#include<fstream>
using std::ofstream, std::ifstream;
#include<iostream>
using std::cout,std::endl,std::cerr,std::cin;
#include<string>
using std::string;
#include<sstream>
using std::stringstream;
#include<iterator>
using std::istreambuf_iterator,std::iterator;
#include <vector>
using std::vector;

string encryptText(const string & plainText, const unsigned & Key){
    // below vector is supposed to contain pointers to successive elements of the plaintext 
    vector<decltype(plainText.begin())> rails;
    auto textBegin = plainText.begin();
    
    unsigned i=0;
    while (i<Key)
    {
        rails.emplace_back(textBegin++);
        ++i;
    }

    auto railBegin=rails.begin();
    auto railsEnd=rails.end();
    auto textEnd=plainText.end();
    string encryptedText;

    for (auto i= railBegin; i<railsEnd;++i){
        auto stringIt= *i;
        for(auto j= stringIt;j<textEnd;j+=(2*Key-2)){
            encryptedText+= *j;
        }
    }
   return encryptedText; 
}

int main(int argc, char**argv){
    if (argc!=2){
        cout<<"Usage: "<<argv[0]<<"name of text file"<<endl;
        return -1;
    }
   
   //input plain text
    ifstream text_file(argv[1]);
    if (!text_file){ 
        cerr<<"error opening the text file"<<endl;
        return -1;
    }
    string plainText{istreambuf_iterator<char>(text_file),istreambuf_iterator<char>()};
    text_file.close();

    //input Key
    cout<<"Please enter the key(no. of rails)"<<endl;
    unsigned key;
    cin>>key;
    if (!cin)
    {
        cerr<<"A problem in Input stream detected!"<<endl;
    }
   
    ofstream output("railcipher_output.txt");
     if (!output){ 
        cerr<<"error generating the output file";
        return -1;
    }
    output<<encryptText(plainText,key);
    output.close();
    cout<<"Encrypted text successfully saved in hillcipher_output.txt in the current drive "<<endl;
}
