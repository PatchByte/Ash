#include "main.h"

#include "compile/InnerClassTest.h"
#include "compile/InstanceClassTest.h"

#include "run/Base64Test.h"
#include "run/BaseRunTest.h"
#include "run/BufferTest.h"
#include "run/CRCTest.h"
#include "run/PrimitiveTest.h"
#include "run/SerializerTest.h"
#include "run/StreamTest.h"
#include "run/StringObjectTest.h"

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
    { "Primitive Test", new PrimitiveTest() }
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