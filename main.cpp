#include "implementFunctions.cpp"

using namespace std;


int main() {
    BigInt publicKey = 65537;
    BigInt privateKey("408404357317903355029332458964750117580730194837646566655201");
    BigInt _modulus("456485935899707203645621335117033939469185360011370425298209");

    string msg = "";
    cout << "Type your message:\n";
    cin.ignore();
    cin >> msg;
    vector<BigInt> encryptedMessage = encrypt(msg, publicKey, _modulus);
    
    string decryptedMessage = decrypt(encryptedMessage, privateKey, _modulus);

    cout << "Original Message: " << msg << endl;

    cout << "Encrypted Message: ";
    for (BigInt num : encryptedMessage)
        cout << num;

    cout << "\nDecrypted Message: " << decryptedMessage << endl;


    
    return 0;
}