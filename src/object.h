#ifndef breeze_object_h
#define breeze_object_h

#include "chunk.h"
#include "common.h"
#include "value.h"

#define ALLOCATE_OBJ(type, object_type)                                        \
  (type *)allocate_object(sizeof(type), object_type)

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_CLOSURE(value) is_obj_type(value, ObjClosureType)
#define IS_FUNCTION(value) is_obj_type(value, ObjFunctionType)
#define IS_NATIVE(value) is_obj_type(value, ObjNativeType)
#define IS_STRING(value) is_obj_type(value, ObjStringType)

#define AS_CLOSURE(value) ((ObjClosure *) AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_NATIVE(value) (((ObjNative *)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum {
  ObjNativeType,
  ObjFunctionType,
  ObjStringType,
  ObjClosureType,
} ObjType;

struct Obj {
  ObjType type;
  struct Obj *next;
};

typedef struct {
  Obj obj;
  int32_t arity;
  int32_t upvalues_len;
  Chunk chunk;
  ObjString *name;
} ObjFunction;

typedef Value (*NativeFn)(int32_t args_len, Value *args);

typedef struct {
  Obj obj;
  NativeFn function;
} ObjNative;

struct ObjString {
  Obj obj;
  uint32_t len;
  const char *chars;
  uint32_t hash;
};

typedef struct {
  Obj obj;
  ObjFunction *function;
} ObjClosure;

ObjClosure *new_closure(ObjFunction *);
ObjFunction *new_function();
ObjNative *new_native(NativeFn);
ObjString *take_string(char *, uint32_t);
ObjString *copy_string(const char *, uint32_t);

static inline bool is_obj_type(Value value, ObjType type) {
  return IS_OBJ(value) && (AS_OBJ(value)->type == type);
}

void print_object(Value value);

#endif // !breeze_object_h
