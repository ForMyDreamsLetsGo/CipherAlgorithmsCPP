#include <iostream>
using std::cin, std::cout, std::endl,std::cerr;
#include<string>
using std::string;
#include<fstream>
using std::ifstream,std::ofstream;
#include<iterator>
using std::istreambuf_iterator;
#include<vector>
using std::vector;
#include<numeric>
using std::gcd;

unsigned long long RSAencrypt(unsigned CharDigit,const unsigned long long& public_key_n, unsigned long long public_key_e ){
    unsigned long long encryptedValue=1;
    while ((public_key_e)--)
    {
        encryptedValue*=CharDigit;
        encryptedValue%=public_key_n;
    }
    return encryptedValue;     
}

vector<unsigned long long> encrypt(const string & plainText){
    //Taking public key input
    cout<<"Please Enter the Encryption Key\n n:"<<endl;
    unsigned long long publicKey_n=0;
    cin>>publicKey_n;
    cin.clear();
    cout<<"e:"<<endl;
    unsigned long long publicKey_e;
    cin>>publicKey_e;
    
    cout<<"Encrypting the text......"<<endl;

/*here we convert each character into ascci value, encrypt 1 character-equivalent digit and push it to result vector*/
/*Due to time limitations we haven't used stringstreams to tokenise the text into seperate words. so the decryption result isn't gonna have spaces between words*/
    vector<unsigned long long> result;
    for (const auto & ch : plainText){
        result.emplace_back(RSAencrypt(ch, publicKey_n,publicKey_e));
    }
    return result;
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
    
    vector<unsigned long long> EncryptionResult=encrypt(plain_text);

    ofstream output("RSAencryption_output.txt");
     if (!output){ 
        cerr<<"error generating the output file";
        return -1;
    }

    for(const auto & i: EncryptionResult){
        output<<i<<'\t';
    }
    output.close();
    cout<<"Encrypted text successfully saved in RSAencryption_output.txt in the current drive "<<endl;


}