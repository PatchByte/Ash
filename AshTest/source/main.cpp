#include "main.h"

#include "BaseRunTest.h"

#include "Compile/InnerClassTest.h"
#include "Compile/InstanceClassTest.h"

#include "Run/AshSection/Base64Test.h"
#include "Run/AshSection/BufferTest.h"
#include "Run/AshSection/BufferReferenceableTest.h"
#include "Run/AshSection/CRCTest.h"
#include "Run/AshSection/PrimitiveTest.h"
#include "Run/AshSection/SerializerTest.h"
#include "Run/AshSection/StreamExpandableTest.h"
#include "Run/AshSection/StreamTest.h"
#include "Run/AshSection/StringObjectTest.h"
#include "Run/AshSection/TemporaryFileTest.h"
#include "Run/AshSection/VectorTest.h"

#include "Run/AshLoggerSection/AshLoggerTagFilterTest.h"
#include "Run/AshLoggerSection/AshLoggerTest.h"

#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, AshBaseRunTest*> smTestRegister = 
{
    { "Buffer Test", new BufferRunTest() },
    { "Stream Test", new StreamRunTest() },
    { "Stream expandable Test", new StreamExpandableTest() },
    { "Base64 Test", new Base64RunTest() },
    { "String Ascii Object Test", new StringAsciiObjectTest() },
    { "String Wide Object Test", new StringWideObjectTest() },
    { "Serializer Test", new SerializerRunTest() },
    { "CRC32 Test", new CRC32RunTest() },
    { "Primitive Test", new PrimitiveTest() },
    { "Vector Test", new VectorTest() },
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