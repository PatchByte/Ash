#include "main.h"

#include "BaseRunTest.h"

#include "Compile/InnerClassTest.h"
#include "Compile/InstanceClassTest.h"

#include "Run/AshSection/Base64Test.h"
#include "Run/AshSection/BufferTest.h"
#include "Run/AshSection/BufferReferenceableTest.h"
#include "Run/AshSection/CRCTest.h"
#include "Run/AshSection/TemporaryFileTest.h"

#include "Run/AshLoggerSection/AshLoggerTagFilterTest.h"
#include "Run/AshLoggerSection/AshLoggerTest.h"

#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, AshBaseRunTest*> smTestRegister =
{
    { "Buffer Test", new BufferRunTest() },
    { "Base64 Test", new Base64RunTest() },
    { "CRC32 Test", new CRC32RunTest() },
    { "BufferRef Test", new BufferReferenceableRunTest() },
    { "Temporary File Test", new TemporaryFileTest() },
#if ASH_ENABLE_LOGGER
    { "Logger Tag Filter Test", new AshLoggerTagFilterTest() },
    { "Logger Test", new AshLoggerTest() }
#endif
};

int main()
{
    for(auto c : smTestRegister)
    {
        auto r = c.second->Do();

        if(r.WasSuccessful())
        {
            std::cout << "Test \"" << c.first << "\" passed." << std::endl;
        }
        else
        {
            std::cerr << "Test \"" << c.first << "\" failed because \"" << r.GetMessage() << "\"." << std::endl;
            return -1;
        }

        delete c.second;
    }

    return 0;
}
