#include "Il2CppTools.h"
#include "il2cpp-class-internals.h"
#include "vm/Class.h"
#include "vm/Runtime.h"

bool IsDelegate(Il2CppClass *klass)
{
    return il2cpp::vm::Class::IsAssignableFrom(il2cpp_defaults.delegate_class, klass) && klass != il2cpp_defaults.delegate_class && klass != il2cpp_defaults.multicastdelegate_class;
}


