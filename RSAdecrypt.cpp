#include <iostream>
using std::cin, std::cout, std::endl,std::cerr;
#include<string>
using std::string,std::stoull;
#include<fstream>
using std::ifstream,std::ofstream;
#include<sstream>
using std::stringstream;
#include<iterator>
using std::istreambuf_iterator;
#include<vector>
using std::vector;

char RSAdecrypt(const unsigned long long & characters, const unsigned long long & publicKey,unsigned long long privateKey ){
    unsigned long long result=1;
    while (privateKey--)
    {
        result*=characters;
        result%=publicKey;
    }
    return result;   
}

string decrypt(const vector<unsigned long long> & EncryptedText){
    // Taking Key inputs 
    cout<<"Enter the public Key N:"<<'\t';
    unsigned long long publicKey_N;
    cin>>publicKey_N;
    cin.clear();
    cout<<endl<<"Enter the Private Key:"<<'\t';
    unsigned long long privateKey;
    cin>>privateKey;
    cin.clear();
    cout<<endl;

    string plainText;
    for(const auto & ch:EncryptedText){
       plainText+= RSAdecrypt(ch, publicKey_N,privateKey);
    }
    return plainText;
}

int main(int argc, char**argv){
    if(argc!=2){
        cerr<<"Usage: "<<argv[0]<<"  name of file containing data to decrypt";
        return -1;
    }
    ifstream input(argv[1]);
    if (!input){ 
        cerr<<"error opening this file";
        return -1;
    }

    string Encrypted_text {istreambuf_iterator<char>(input),istreambuf_iterator<char>()}; //default iterator refers to end of input iterator
                     // note the brace(list) initialisation. initialisation using "()" doesn't work.
    input.close();

    //using the stringstream to break the input and fill up the encrypted vector

    stringstream BreakText(Encrypted_text);
    vector<unsigned long long> EncryptedLetters;
    string tokens;
    while (BreakText>>tokens)
    {
        EncryptedLetters.emplace_back(stoull(tokens));
    }

    string decryptedText= decrypt(EncryptedLetters);
    
    ofstream output("RSAdecryption_output.txt");
     if (!output){ 
        cerr<<"error generating the output file";
        return -1;
    }

    output<<decryptedText;
    output.close();
    cout<<"Decrypted text successfully saved in RSAdecryption_output.txt in the current drive "<<endl;
    
}