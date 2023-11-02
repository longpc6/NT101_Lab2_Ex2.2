// Include 
#include "include/arithmetic_assignment.hpp"
#include "include/assignment.hpp"
#include "include/binary_arithmetic.hpp"
#include "include/catch.hpp"
#include "include/constructors.hpp"
#include "include/conversion.hpp"
#include "include/increment_decrement.hpp"
#include "include/io_stream.hpp"
#include "include/math.hpp"
#include "include/random.hpp"
#include "include/relational.hpp"
#include "include/unary_arithmetic.hpp"
#include "include/utility.hpp"
#include <tuple>
#include <vector>

using namespace std;

// implement

// calculate a^d mod n
BigInt calculateModularExponentiation(BigInt a, BigInt d, BigInt n) {
    BigInt res(1);
    a %= n;
    
    while (d > 0) {
        if (d % 2 == 1) {
            res = (res * a) % n;
        }
        a = (a * a) % n;
        d /= 2;
    }

    return res;
}

bool checkMillerRabin(BigInt d, BigInt n) {
    BigInt a = BigInt(2) + rand() % (n - 4);
    BigInt x = calculateModularExponentiation(a, d, n);

    if (x == 1 || x == n - BigInt(1)) return true;

    while (d != n - BigInt(1)) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool isPrime(BigInt n, int k) {
    if(n % 2 == 0) return false;

    BigInt d = n - BigInt(1);
    while(d % 2 == (unsigned long long)0) d /= 2;

    for (int i = 0; i < k; i++) {
        if (!checkMillerRabin(d, n)) return false;
    }

    return true;
}

BigInt generateLargePrimeNumber(int length = 50) {
    int count = 0;
    while (true) {
        count++;
        BigInt largeNumber = big_random(length);
        if (largeNumber % 2 == 0) continue;
        if (isPrime(largeNumber, 50)) {
            cout << "Iteration for finding: " << count << endl;
            return largeNumber;
        }
    }
    return -1;
}

BigInt findE(BigInt phi, int length) {
    while (true) {
        BigInt e = big_random(length / 2);
        if (gcd(phi, e) == 1) {
            return e;
        }
    }
    return -1;
}

BigInt modInverse(BigInt a, BigInt m) {
    BigInt m0 = m;
    BigInt x0 = 0, x1 = 1;

    while (a > 1) {
        BigInt q = a / m;
        BigInt t = m;

        m = a % m;
        a = t;
        t = x0;

        x0 = (x1 - q * x0);
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

tuple<tuple<BigInt, BigInt>, tuple<BigInt, BigInt>> generateKeyPair(bool standardValueE = true, int numberOfDigits = 30) {
    BigInt p = generateLargePrimeNumber(numberOfDigits);
    BigInt q = generateLargePrimeNumber(numberOfDigits);

    BigInt n = p * q;
    BigInt phi_n = (p - 1) * (q - 1);

    BigInt e;

    if (standardValueE) e = 65537;
    else e = findE(n, numberOfDigits);

    BigInt d = modInverse(e, phi_n);
    
    tuple<BigInt, BigInt> publicKey = make_tuple(e, n);
    tuple<BigInt, BigInt> privateKey = make_tuple(d, n);
    
    return make_tuple(publicKey, privateKey);
}


int numberStringToInt(const string& numberString) {
    int number = 0;
    int sign = 1;
    size_t i = 0;

    if (numberString[i] == '-') {
        sign = -1;
        ++i;
    }

    for (; i < numberString.size(); ++i) {
        number = number * 10 + (numberString[i] - '0');
    }

    return number * sign;
}

// string messageToNumber(const string& message) {
//     string numberString;

//     for (char c : message) {
//         int asciiValue = static_cast<int>(c);
//         numberString += to_string(asciiValue);
//     }

//     return numberString;
// }

vector<BigInt> message_to_numbers(const string& message) {
    vector<BigInt> numeric_message;
    for (char c : message) {
        numeric_message.push_back(static_cast<BigInt>(c));
    }
    return numeric_message;
}

string numbers_to_message(const vector<BigInt>& numeric_message) {
    string message;
    int i = 0;
    for (BigInt num : numeric_message) {
        message += num.to_string()[i];
        i++;
    }
    return message;
}

vector<BigInt> encrypt(string message, BigInt e, BigInt n) {
    vector<BigInt> res;
    vector<BigInt> asciiMessage = message_to_numbers(message);
    
    for (BigInt num : asciiMessage) {
        BigInt resNumeric = calculateModularExponentiation(num, e, n);
        res.push_back(resNumeric);
    }

    return res;
}

string decrypt(vector<BigInt> encryptedMessage, BigInt d, BigInt n) {
    vector<BigInt> res;

    for (BigInt num : encryptedMessage) {
        BigInt resNumeric = calculateModularExponentiation(num, d, n);
        res.push_back(resNumeric);
    }

    return numbers_to_message(res);
}