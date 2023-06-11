#include "Base64Test.h"
#include <Ash/AshBase64.h>
#include <iostream>

ash::AshResult Base64RunTest::Do()
{
    constexpr int expectedVar = 40;
    int var = 40;
    std::string encodedVar = "";

    {
        // Encode
        auto encodedBuffer = new ash::AshBuffer(&var, sizeof(var));
        encodedVar = ash::AshBase64::Encode(encodedBuffer);
        delete encodedBuffer;
    }

    {
        // Decode
        auto decodedBuffer = ash::AshBase64::Decode(encodedVar);
        var = *decodedBuffer->GetBuffer<int>();
        delete decodedBuffer;
    }

    if(var != expectedVar)
    {
        return ash::AshResult(false, "Base64 var does not match.");
    }

    return ash::AshResult(true);
}