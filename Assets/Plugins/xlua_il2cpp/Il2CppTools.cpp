#include "Il2CppTools.h"
#include "il2cpp-class-internals.h"
#include "vm/Class.h"
#include "vm/Runtime.h"
#include "vm/Reflection.h"
#include "icalls/mscorlib/System/Type.h"
using namespace il2cpp::vm;
bool IsDelegate(Il2CppClass *klass)
{
    return il2cpp::vm::Class::IsAssignableFrom(il2cpp_defaults.delegate_class, klass) && klass != il2cpp_defaults.delegate_class && klass != il2cpp_defaults.multicastdelegate_class;
}

bool IsArray(Il2CppClass* klass)
{
    return klass->rank > 0;
    /*auto type = Reflection::GetTypeObject(Class::GetType(klass));
    return il2cpp::icalls::mscorlib::System::Type::IsArrayImpl(type);*/
}


