// Include guard to prevent double inclusion
#ifndef __CUSTOM_ERROR_CLASS_H__
#define __CUSTOM_ERROR_CLASS_H__

// Required includes for our exception class
#include <exception>
#include <string>

using namespace std;

// Our custom exception class that inherits from std::exception
class MyException : public exception {
    public:
        MyException(void);
        MyException(const char*);
        MyException(const string&);
        virtual const char* what() const noexcept override {
            return this->errString.c_str();
        }
    private:
        string errString;
};

#endif