#include<iostream>
using std::cout, std::cin,std::cerr,std::endl;
#include <fstream>
using std::ifstream, std::ofstream;
#include <string>
using std::string, std::getline;
#include<cctype>
using std::isupper, std::isalpha;

string EncryptCaesarCipher(const string & plain_text, const int shift){
    string Encrypted_text="";

    for(const char & ch: plain_text){
        if (isalpha(ch)){
            char base= isupper(ch)?'A':'a';
            Encrypted_text+= static_cast<char>((ch-base+shift)%26 +base);
        }
        else{
            Encrypted_text+=ch;
        }

    }
    return Encrypted_text;
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

    ofstream output("caesar_cipher_output.txt");
    if (!output){ 
        cerr<<"error generating the output file";
        return -1;
    }

    char repeat='Y';
    while(repeat=='Y'|| repeat=='y'){
        cout<<"Enter the Shift Value"<<endl;
        int shift=0;
        cin>>shift;
        cin.ignore();

        string plain_text;
        while(getline(input,plain_text)){
            output<<EncryptCaesarCipher(plain_text,shift)<<endl;
        }

        cout<< "Encrypted Text saved into output.txt in the same folder!"<<endl;
        cout<<"Do you want to re-encrypt with a new shift value?(Y) or save the current encryption?(N)"<<endl;
        cin>>repeat;
        cin.ignore();
    }

    input.close();
    output.close();

    cout<<"file successfully closed!";

    return 0;
}

