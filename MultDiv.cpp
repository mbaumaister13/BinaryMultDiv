#include <iostream>
#include <bitset>
#include <cmath>
#include <vector>
#include <boost/dynamic_bitset.hpp>
using namespace std;

//C++ Program to perform Shift-Add Multiplication & Restoring Division
//Written by Matthew Baumaister, 4/4/2020

bool fullAdder(bool b1, bool b2, bool& carry) {
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

void bitsetAdd(boost::dynamic_bitset<>& x, boost::dynamic_bitset<>& y, bool& carry) {
    for(int i = 0; i < x.size(); ++i) {
        x[i] = fullAdder(x[i], y[i], carry);
    }
}

bool fullSubstractor(bool b1, bool b2, bool& borrow) {
    bool diff;
    if(borrow) {
        diff = !(b1 ^ b2);
        borrow = !b1 || (b1 && b2);
    } else {
        diff = b1 ^ b2;
        borrow = !b1 && b2;
    }
    return diff;
}

void bitsetSubtract(boost::dynamic_bitset<>& x, boost::dynamic_bitset<>& y) {
    bool borrow = false;
    for(int i = 0; i < x.size(); ++i) {
        if(borrow) {
            if(x[i]) {
                x[i] = y[i];
                borrow = y[i];
            } else {
                x[i] = !y[i];
                borrow = true;
            }
        } else {
            if (x[i]) {
                x[i] = !y[i];
                borrow = false;
            } else {
                x[i] = y[i];
                borrow = y[i];
            }
        }
    }
}

int shiftAdd(int a, int b, int n) {
    int result = 0;
    bool carry = false, neg = false, bothNeg = false;
    
    if(a < 0) {
        neg = !neg;
        a = -a;
    }
    if(b < 0) {
        b = -b;
        neg = !neg;
    }
    if (a < 0 && b < 0) {
        bothNeg = true;
    }

    boost::dynamic_bitset<> x(n, a);
    boost::dynamic_bitset<> y(n, b);
    boost::dynamic_bitset<> p(n, 0);
    
    

    string s(n, ' ');
    cout << "C  " << "P" << s <<  "Y" << s << endl;
    cout << carry << "  " << p << " " << y << endl;
    for(int i = 0; i < n; ++i) {
        if(y[0] == 1) {
            cout << "  +" << x << " " << s << "          Y[0] = 1 -> add X" << endl;
            bitsetAdd(p, x, carry);
        }

        cout << "--------------------" << endl;
        cout << carry << "  " << p << " " << y << endl << endl;
       
        y >>= 1;
        y.set(n-1, p[0]);
        p >>= 1;
        p.set(n-1, carry | p[n-1]);
        cout << carry << "  " << p << " " << y << "          Shift right" << endl;
    }

    
    if(bothNeg) {
        cout << "  -" << x << " " << s << endl;
        bitsetSubtract(p, x);
        cout << "--------------------" << endl;
        cout << carry << "  " << p << " " << y << endl << endl;
    }
    
    boost::dynamic_bitset<> ans(n*2, 0);
    for(int i = 0; i < n*2; ++i) {
        if(i < n) {
            ans[i] = y[i];
        }  
        if(i > n-1) {
            ans[i] = p[i-n];
        }
    }
    cout << endl;
    if(neg) {
        cout << "Negative Result, showing 2's Complement" << endl;
        ans.flip();
    } 
    cout << "Binary Result: ";
    for(int i = ans.size() - 1; i >= 0; --i) {
        cout << ans[i];
        if(i == ans.size()/2) {
            cout << " ";
        }
    }

    cout << endl;
    if(!neg) {
        result = (int)ans.to_ulong();
    } else {
        ans.flip();
        result = -(int)ans.to_ulong();
    }
        
    return result;
}

int restoringDivision(int a, int b, int& rem, int n) {
    boost::dynamic_bitset<> q(n, a);
    boost::dynamic_bitset<> y(n+1, b);
    boost::dynamic_bitset<> ac(n+1, rem);
    int result = 0;

    cout << "  AC" << string(n+1, ' ') << "Q" << string(n, ' ') << endl;
    cout << "  " << ac << " " << q << endl;
    for(int i = 0; i < n; ++i) {
        bool carry = false;
        ac <<= 1;
        ac.set(0, q[n-1]);
        q <<= 1;
      
        cout << "  " << ac << " " << q << "     Shifting left" << endl;

        bitsetSubtract(ac, y);
        cout << "- " << y << "     Subtracting Y" << endl;

        cout << "----------------------" << endl;
        cout << "  " << ac << " " << q << endl << endl;

        if(ac[n-1] == 1) {
            cout << "+ " << y << "     Adding Y back" << endl;
            bitsetAdd(ac, y, carry);
            cout << "----------------------" << endl;
            cout << "  " << ac << " " << q << endl << endl;
        } else {
            q.set(0, 1);
            cout << "  " << ac << " " << q << "     Q[0] <= 1" << endl;
        }
    }
    rem = (int)ac.to_ulong();
    result = (int)q.to_ulong();
    return result;
}

int main() {
    while(true) {
        int choice = 0, n = 0, m = 0;
        cout << "What you wanna do?\nMultiply (0)\nDivide (1)\nLeave (Any other number)" << endl;
        cin >> choice;
        switch(choice) {
            case 0: {
                cout << "Enter first number to multiply: " << endl;
                cin >> n;
                cout << "Enter second number to multiply: " << endl;
                cin >> m;

                int numBits = 0, tempN = n, tempM = m;
                if(n < 0) {
                    tempN = -n;
                }
                if(m < 0) {
                    tempM = -m;
                }
                if(tempN > tempM) {
                    if(ceil(log2(tempN)) == log2(tempN)) {
                        numBits = ceil(log2(tempN)) + 1;
                    } else {
                        numBits = ceil(log2(tempN));
                    }
                }
                else {
                    if(ceil(log2(tempM)) == log2(tempM)) {
                        numBits = ceil(log2(tempM)) + 1;
                    } else {
                        numBits = ceil(log2(tempM));
                    }
                }

                int result = shiftAdd(n, m, numBits);
                cout << "Result: " << result << endl << endl;
                break;
            }
            case 1: {
                int ac = 0;
                cout << "Enter dividend: " << endl;
                cin >> n;
                cout << "Enter divisor: " << endl;
                cin >> m;
                int numBits = 0;
                if(n > m) {
                    numBits = ceil(log2(n)) + 1;
                }
                else if(m > n) {
                    numBits = ceil(log2(m)) + 1;
                } else {
                    numBits = ceil(log2(m)) + 1;
                }
                int result = restoringDivision(n, m, ac, numBits);
                cout << "Quotient: " << result << ", Remainder: " << ac << endl << endl;
                break;
            }
            default:
                return 0;
        }
    }   
    return 0;
}