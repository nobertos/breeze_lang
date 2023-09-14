#ifndef breeze_object_h
#define breeze_object_h

#include "common.h"
#include "value.h"

#define ALLOCATE_OBJ(type, object_type)\
      (type*)allocate_object(sizeof(type), object_type)

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) is_obj_type(value, ObjStringType)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))

#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum {
  ObjStringType
} ObjType;

struct Obj {
  ObjType type;
  struct Obj* next;
};

struct ObjString {
  Obj obj;
  uint32_t len;
  const char *chars;
};

ObjString* take_string(char* chars, uint32_t len);
ObjString* copy_string(const char* chars, uint32_t len); 

static inline bool is_obj_type(Value value, ObjType type) {
  return IS_OBJ(value)&& (AS_OBJ(value)->type == type);
}

void print_object(Value value);



#endif // !breeze_object_h
