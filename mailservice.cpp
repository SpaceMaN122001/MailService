#include "mailservice.h"

MailService::MailService()
{
}

MailService::~MailService()
{
}


bool isPasswordCorrect(QString password)
{
    if(password.length() < 8)
        return false;

    std::string _password = password.toUtf8().constData();
    int uppercaseCount = 0;
    int amountOfNumbers = 0;

    for(char s : _password)
        if(std::isupper(s)) uppercaseCount++;

    for(char s : _password)
        if(isdigit(s)) amountOfNumbers++;

    if(uppercaseCount == 0) return false;

     if(amountOfNumbers == 0) return false;

    return true;
}

