#include "main.h"

#include "compile/InnerClassTest.h"
#include "compile/InstanceClassTest.h"

#include "BaseRunTest.h"

#include "run/ash_section/Base64Test.h"
#include "run/ash_section/BufferTest.h"
#include "run/ash_section/CRCTest.h"
#include "run/ash_section/PrimitiveTest.h"
#include "run/ash_section/SerializerTest.h"
#include "run/ash_section/StreamTest.h"
#include "run/ash_section/StringObjectTest.h"
#include "run/ash_section/VectorTest.h"

#include "run/ash_logger_section/AshLoggerTagFilterTest.h"
#include "run/ash_logger_section/AshLoggerTest.h"

#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, AshBaseRunTest*> smTestRegister = 
{
    { "Buffer Test", new BufferRunTest() },
    { "Stream Test", new StreamRunTest() },
    { "Base64 Test", new Base64RunTest() },
    { "String Ascii Object Test", new StringAsciiObjectTest() },
    { "String Wide Object Test", new StringWideObjectTest() },
    { "Serializer Test", new SerializerRunTest() },
    { "CRC32 Test", new CRC32RunTest() },
    { "Primitive Test", new PrimitiveTest() },
    { "Vector Test", new VectorTest() },
#if ASH_ENABLE_LOGGER
    { "Logger Tag Filter Test", new AshLoggerTagFilterTest() },
    { "Logger Test", new AshLoggerTest() }
#endif
};

int main()
{
    if(InnerClassTest().GetVar() != InnerClassTest::GetExpectedVar())
    {
        std::cerr << "InnerClassTest GetVar test does not match up." << std::endl;
        return -1;
    }

    if(InstanceClassTest::GetInstance()->GetVar() != InstanceClassTest::GetExpectedVar())
    {
        std::cerr << "InstanceClassTest GetVar test does not match up." << std::endl;
    }

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