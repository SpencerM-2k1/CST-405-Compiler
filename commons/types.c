#include "types.h"

inline const char* varTypeToString(VarType type) {
    switch (type)
    {
        case (VarType_Int):
            return "VarType_Int";
            break;
        case (VarType_Float):
            return "VarType_Float";
            break;
        case (VarType_Error):
            return "VarType_Error"; //Assigned when program detects invalid var type
            break;
        default:
            return "UNHANDLED_VARTYPE_ERROR";   //Should only occur when type is UNASSIGNED
            break;                              //(i.e. garbage data from unassigned type, different in nature than "VarType_Error")
    }
}