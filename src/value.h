#ifndef zhenzhu_value_h
#define zhenzhu_value_h

#include "common.h"
#include "zhenzhu.h"

#define OBJ_TYPE(value)        (AS_OBJ(value)->type)

// These macros promote a primitive C value to a full Zhenzhu Value. There are
// more defined below that are specific to the various Value representations.
#define BOOL_VAL(boolean)   ((boolean) ? TRUE_VAL : FALSE_VAL)
#define NUMBER_VAL(num)     (zzNumToValue(num))
#define OBJ_VAL(obj)        (zzObjectToValue((Obj*)(obj)))

#define IS_FIBER(value)        isObjType(value, OBJ_FIBER)
#define IS_VAR_FRAME(value)    isObjType(value, OBJ_VAR_FRAME)
#define IS_CALL_FRAME(value)   isObjType(value, OBJ_CALL_FRAME)
#define IS_MARK_FRAME(value)   isObjType(value, OBJ_MARK_FRAME)
#define IS_STRING(value)       isObjType(value, OBJ_STRING)
#define IS_CLOSURE(value)      isObjType(value, OBJ_CLOSURE)
#define IS_OP_CLOSURE(value)   isObjType(value, OBJ_OP_CLOSURE)
#define IS_CONTINUATION(value) isObjType(value, OBJ_CONTINUATION)

// These macros cast a Value to one of the specific object types. These do *not*
// perform any validation, so must only be used after the Value has been
// ensured to be the right type.
// AS_OBJ and AS_BOOL are implementation specific.
#define AS_VAR_FRAME(value)     ((ObjVarFrame*)AS_OBJ(value))
#define AS_CALL_FRAME(value)    ((ObjCallFrame*)AS_OBJ(value))
#define AS_MARK_FRAME(value)    ((ObjMarkFrame*)AS_OBJ(value))
#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_OP_CLOSURE(value)    ((ObjOpClosure*)AS_OBJ(value))
#define AS_CONTINUATION(value)  ((ObjContinuation)AS_OBJ(value))
#define AS_FIBER(v)             ((ObjFiber*)AS_OBJ(v))
#define AS_FOREIGN(v)           ((ObjForeign*)AS_OBJ(v))
#define AS_NUMBER(value)        (zzValueToNum(value))
#define AS_RANGE(v)             ((ObjRange*)AS_OBJ(v))
#define AS_STRING(v)            ((ObjString*)AS_OBJ(v))
#define AS_CSTRING(v)           (AS_STRING(v)->chars)

typedef enum {
    OBJ_CODE_BLOCK,
    OBJ_FIBER,
    OBJ_VAR_FRAME,
    OBJ_CALL_FRAME,
    OBJ_MARK_FRAME,
    OBJ_STRING,
    OBJ_CLOSURE,
    OBJ_OP_CLOSURE,
    OBJ_CONTINUATION,
} ObjType;

// Base struct for all heap-allocated object types.
typedef struct Obj Obj;
struct Obj {
    ObjType type;
    // Used during garbage collection
    bool isMarked;

    // All currently allocated objects are maintained in a linked list.
    // This allows all memory to be reachable during garbage collection.
    struct Obj* next;
};

#if ZHENZHU_POINTER_TAGGING

#include "value_ptr_tagged.h"

#elif ZHENZHU_NAN_TAGGING

#include "value_nan_tagged.h"

#else

#include "value_union.h"

#endif

DECLARE_BUFFER(Value, Value);
DECLARE_BUFFER(Byte, uint8_t);
DECLARE_BUFFER(Int, int);

typedef struct ObjCodeBlock {
    Obj obj;
    ByteBuffer code;
    ValueBuffer constants;
    IntBuffer lines;
} ObjCodeBlock;

typedef struct ObjString {
    Obj obj;
    int length;
    char* chars;
} ObjString;

typedef struct ObjVarFrame {
    Obj obj;
    Value* slots;
    int slotCount;
} ObjVarFrame;

typedef struct ObjCallFrame {
    ObjVarFrame vars;
    uint8_t* afterLocation;
} ObjCallFrame;

typedef struct ObjMarkFrame {
    ObjCallFrame call;
    // The identifier that will be searched for when trying to execute a particular operation. Designed to
    // enable efficiently finding sets of related operations that all get handled by the same handler expression.
    // Trying to execute an operation must specify the 'set' of operations the operation belongs to (markId), then
    // the index within the set of the operation itself (operations[i]).
    int markId;
    Value afterClosure;
    Value* operations;
    int operationCount;
} ObjMarkFrame;

typedef struct ObjFiber {
    Obj obj;
    uint8_t* ip;
    bool isRoot;

    // Value stack, which all instructions that consume and produce data operate upon.
    Value* stack;
    Value* stackTop;
    int stackCount;
    int stackCapacity;

    // Frame stack, which variable, function, and continuation instructions operate upon.
    ObjVarFrame** callStack;
    ObjVarFrame** callStackTop;
    int callStackCount;
    int callStackCapacity;

    struct ObjFiber* caller;
} ObjFiber;

typedef struct ObjClosure {
    Obj obj;
    uint8_t* funcLocation;
    Value* vars;
    int varCount;
} ObjClosure;

typedef struct ObjOpClosure {
    ObjClosure closure;
    int argCount;
} ObjOpClosure;

typedef struct ObjContinuation {
    Obj obj;
    uint8_t* resumeLocation;
    Value* savedStack;
    int savedStackCount;
    ObjVarFrame** savedFrames;
    int savedFramesCount;
} ObjContinuation;

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

// Logs a textual representation of the given value to the output
void printValue(Value value);
void printObject(Value object);

void zzFreeObj(ZZVM* vm, Obj* object);

#endif