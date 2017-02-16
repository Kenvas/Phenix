#pragma once

#include "_namespace/begin"

class Device
{
public:

    virtual void SetValue(int code, int value) noexcept;
};

#include "_namespace/end"