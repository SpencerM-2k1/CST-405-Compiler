#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

typedef enum VarType {
    VarType_Int,
    VarType_Float,
    VarType_Error,
}VarType;

const char* varTypeToString(VarType type);

#endif