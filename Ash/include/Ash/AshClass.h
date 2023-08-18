#pragma once

// Class internal objects are classes that can only be accessed by the parent
// It helps with the compile time and access management

#define ASH_CLASS_ADD_INTERNAL_OBJECT(name) class Class##name##InternalObject* classInternal##name; friend class Class##name##InternalObject;
#define ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(name) class Class##name##InternalObject
#define ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(name) { classInternal##name = new Class##name##InternalObject(); }
#define ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(name) if(classInternal##name != nullptr) { delete classInternal##name; classInternal##name = nullptr; }
#define ASH_CLASS_DECLARE_FRIEND(name) friend class name;

#define ASH_CLASS_IMPLEMENT_SIMPLE_INSTANCE_FUNCTION(_Class) static _Class * GetInstance() { static _Class _Class##Instance = _Class (); return & _Class##Instance ; }