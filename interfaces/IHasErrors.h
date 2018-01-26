#pragma once

#include <string>

class IHasErrors
{
public:
    virtual std::string GetErrors();
protected:
    std::string errors;
};