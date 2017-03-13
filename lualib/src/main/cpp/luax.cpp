#include <string>
#include <jni.h>
#include <cstdio>
#include <cstdarg>
#include <ctype.h>
#include "lua/lua.hpp"

using namespace std;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); TypeName& operator=(const TypeName&);
typedef int (*LuaCFunc)(lua_State*);
const size_t kBufSize = 4096;

std::string strFormat(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);
    return std::string(buffer);
}

//lua utility functions
enum LuaType
{
    LuaNone = -1,
    LuaNil,
    LuaBoolean,
    LuaLightUserData,
    LuaNumber,
    LuaString,
    LuaTable,
    LuaFunction,
    LuaUserData,
    LuaThread,
    LuaNumTags
};

LuaType luaGetType(lua_State* plua_state, int index)
{
    return (LuaType)lua_type(plua_state, index);
}


void luaPop(lua_State* plua_state, int count)
{
    lua_pop(plua_state, count);
}

int luaGetTop(lua_State* plua_state)
{
    return lua_gettop(plua_state);
}

void luaGetGlobal(lua_State* plua_state, const std::string& name)
{
    lua_getglobal(plua_state, name.c_str());
}

void luaSetGlobal(lua_State* plua_state, const std::string& name)
{
    lua_setglobal(plua_state, name.c_str());
}

int luaCallFunc(lua_State* plua_state, int nargs, int nrets)
{
    return lua_pcall(plua_state, nargs, nrets, 0);
}

void luaAssert(lua_State* plua_state, bool assertion, const std::string& str)
{
    if (!plua_state)
        luaL_error(plua_state, "plua_state is null");

    if (!assertion)
        luaL_error(plua_state, "%s", str.c_str());
}

void luaError(lua_State* plua_state, const std::string& str)
{
    luaL_error(plua_state, "%s", str.c_str());
}

double luaGetDouble(lua_State* plua_state, int index)
{
    return lua_tonumber(plua_state, index);
}

double luaGetDouble(lua_State* plua_state, int index, double default_num)
{
    return lua_isnumber(plua_state, index) ? luaGetDouble(plua_state, index) : default_num;
}

int luaGetInteger(lua_State* plua_state, int index)
{
    return lua_tointeger(plua_state, index);
}

int luaGetInteger(lua_State* plua_state, int index, int default_int)
{
    return lua_isnumber(plua_state, index) ? luaGetInteger(plua_state, index) : default_int;
}

std::string luaGetString(lua_State* plua_state, int index)
{
    const char* result = lua_tostring(plua_state, index);
    if (!result)
        luaError(plua_state, strFormat("luaGetString from index %d failed.", index));

    return std::string(lua_tostring(plua_state, index));
}

std::string luaGetString(lua_State* plua_state, int index, const std::string& default_str)
{
    return lua_isstring(plua_state, index) ? luaGetString(plua_state, index) : default_str.c_str();
}

bool luaGetBoolean(lua_State* plua_state, int index)
{
    return bool(lua_toboolean(plua_state, index));
}

bool luaGetBoolean(lua_State* plua_state, int index, bool default_bool)
{
    return lua_isboolean(plua_state, index) ? luaGetBoolean(plua_state, index) : default_bool;
}

void luaPushDouble(lua_State* plua_state, double double_val)
{
    lua_pushnumber(plua_state, double_val);
}

void luaPushInteger(lua_State* plua_state, int int_val)
{
    lua_pushinteger(plua_state, int_val);
}

void luaPushString(lua_State* plua_state, const std::string& str_val)
{
    lua_pushstring(plua_state, str_val.c_str());
}

void luaPushNil(lua_State* plua_state)
{
    lua_pushnil(plua_state);
}

void luaPushBoolean(lua_State* plua_state, bool boolean)
{
    lua_pushboolean(plua_state, boolean);
}

//luaGetError
std::string luaGetError(lua_State* plua_state, int err)
{
    std::string error_str("");
    if (0 == err)
        return error_str;

    std::string err_type;
    switch (err)
    {
        case LUA_ERRSYNTAX: //compile-time error
            err_type = "syntax error during pre-compilation";
            break;
        case LUA_ERRMEM: //memory error
            err_type = "memory allocation error";
            break;
        case LUA_ERRRUN: //runtime-time error
            err_type = "runtime error";
            break;
        case LUA_YIELD: //thread suspend error
            err_type = "thread has been suspended";
            break;
        case LUA_ERRERR: //error while running
            err_type = "error while running the error handler function";
            break;
        default:
            err_type = "unknown";
            break;
    }

    error_str = string("error(") + string(err_type.c_str()) + ") " + string(lua_tostring(plua_state, -1));
    luaPop(plua_state, 1);

    return error_str;
}

//luaParseLine
int luaParseLine(lua_State* plua_state, const std::string& line, std::string& error_str)
{
    error_str = "";
    if (0 == plua_state)
        return -1;

    int err = luaL_loadbuffer(plua_state, line.c_str(), line.length(), "line");
    if (0 != err)
    {
        error_str = luaGetError(plua_state, err);
        return err;
    }

    err = lua_pcall(plua_state, 0, 0, 0);
    error_str = luaGetError(plua_state, err);

    return err;
}

//luaParseFile
int luaParseFile(lua_State* plua_state, const std::string& file, std::string& error_str)
{
    error_str = "";
    if (0 == plua_state)
        return -1;

    int err = luaL_loadfile(plua_state, file.c_str());
    if (0 != err)
    {
        error_str = luaGetError(plua_state, err);
        return err;
    }
    err = lua_pcall(plua_state, 0, LUA_MULTRET, 0);
    error_str = luaGetError(plua_state, err);
    return err;
}

//LuaState definition
class LuaState
{
public:
    LuaState();
    ~LuaState();
    inline lua_State* getState() const { return plua_state_; }
    inline std::string getError() const { return error_str; }
    void registerFunction(const std::string& func_name, LuaCFunc lua_reg_func);
    int parseLine(const std::string& line);
    int parseFile(const std::string& file);
    bool reset();

    //get operate
    inline int getType(int index) { return (int)luaGetType(getState(), index); }
    inline double getDouble(int index) { return luaGetDouble(getState(), index); }
    inline double getDouble(int index, double default_num) { return luaGetDouble(getState(), index, default_num); }
    inline int getInteger(int index) { return luaGetInteger(getState(), index); }
    inline int getInteger(int index, int default_int) { return luaGetInteger(getState(), index, default_int); }
    inline std::string getString(int index) { return luaGetString(getState(), index); }
    inline std::string getString(int index, const std::string& default_str) { return luaGetString(getState(), index, default_str); }
    inline bool getBoolean(int index) { return luaGetBoolean(getState(), index); }
    inline bool getBoolean(int index, bool default_bool) { return luaGetBoolean(getState(), index, default_bool); }

    //push operate
    inline void pushDouble(double value) { luaPushDouble(getState(), value); }
    inline void pushInteger(int value) { luaPushInteger(getState(), value); }
    inline void pushString(const std::string& value) { luaPushString(getState(), value); }
    inline void pushNil() { luaPushNil(getState()); }
    inline void pushBoolean(bool value) { luaPushBoolean(getState(), value); }

    //other operate
    inline void pop(int index) { luaPop(getState(), index); }
    inline int getTop() { return luaGetTop(getState()); }
    inline void getGlobal(const std::string& name) { luaGetGlobal(getState(), name); }
    inline void setGlobal(const std::string& name) { luaSetGlobal(getState(), name); }
private:
    bool init();
    void cleanup();
    bool loadLibs();
private:
    lua_State* plua_state_;
    std::string error_str;
private:
    DISALLOW_COPY_AND_ASSIGN(LuaState)
};

string getStringFromJni(JNIEnv *env, jstring str);

//LuaState implementation
LuaState::LuaState() :
        plua_state_(0)
{
    init();
}

LuaState::~LuaState()
{
    cleanup();
}

bool LuaState::init()
{
    if (plua_state_)
        return false;

    plua_state_ = luaL_newstate();
    loadLibs();

    return true;
}

void LuaState::cleanup()
{
    if (plua_state_)
        lua_close(plua_state_);

    plua_state_ = 0;
}

bool LuaState::loadLibs()
{
    if (0 != plua_state_)
    {
        luaL_openlibs(plua_state_);
        return true;
    }
    else
    {
        return false;
    }
}

bool LuaState::reset()
{
    cleanup();
    return init();
}

int LuaState::parseLine(const std::string& line)
{
    return luaParseLine(plua_state_, line, error_str);
}

int LuaState::parseFile(const std::string& file)
{
    return luaParseFile(plua_state_, file, error_str);
}

void LuaState::registerFunction(const std::string& func_name, LuaCFunc lua_reg_func) {
    lua_register(plua_state_, func_name.c_str(), lua_reg_func);
}

#ifdef __cplusplus

string getStringFromJni(JNIEnv *env, jstring str) {
    const char *char_str = env->GetStringUTFChars(str, 0);
    string stdstring(char_str);
    env->ReleaseStringUTFChars(str, char_str);
    return stdstring;
}

extern "C" {
#endif

JNIEXPORT jlong JNICALL
Java_com_jmengxy_lualib_Lua_newLuaState(JNIEnv *env, jclass type) {
    return reinterpret_cast<jlong>(new LuaState());
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_deleteLuaState(JNIEnv *env, jclass type, jlong luaStatePtr) {
    delete reinterpret_cast<LuaState*>(luaStatePtr);
}

JNIEXPORT jint JNICALL
Java_com_jmengxy_lualib_Lua_luaParseLine(JNIEnv *env, jclass type, jlong luaStatePtr,
                                         jstring file) {
    return (reinterpret_cast<LuaState*>(luaStatePtr))->parseLine(getStringFromJni(env, file));
}

JNIEXPORT jint JNICALL
Java_com_jmengxy_lualib_Lua_luaParseFile(JNIEnv *env, jclass type, jlong luaStatePtr,
                                         jstring file) {
    return (reinterpret_cast<LuaState*>(luaStatePtr))->parseFile(getStringFromJni(env, file));
}

JNIEXPORT jstring JNICALL
Java_com_jmengxy_lualib_Lua_luaGetError(JNIEnv *env, jclass type, jlong luaStatePtr, jint errCode) {
    return env->NewStringUTF((reinterpret_cast<LuaState*>(luaStatePtr))->getError().c_str());
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaGetGlobal(JNIEnv *env, jclass type, jlong luaStatePtr,
                                         jstring name) {
    reinterpret_cast<LuaState*>(luaStatePtr)->getGlobal(getStringFromJni(env, name));
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaSetGlobal(JNIEnv *env, jclass type, jlong luaStatePtr,
                                         jstring name) {
    reinterpret_cast<LuaState*>(luaStatePtr)->setGlobal(getStringFromJni(env, name));
}

JNIEXPORT jint JNICALL
Java_com_jmengxy_lualib_Lua_luaGetType(JNIEnv *env, jclass type, jlong luaStatePtr, jint index) {
    return reinterpret_cast<LuaState*>(luaStatePtr)->getType(index);
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaPop(JNIEnv *env, jclass type, jlong luaStatePtr, jint index) {
    reinterpret_cast<LuaState*>(luaStatePtr)->pop(index);
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaPushString(JNIEnv *env, jclass type, jlong luaStatePtr,
                                          jstring value) {
    reinterpret_cast<LuaState*>(luaStatePtr)->pushString(getStringFromJni(env, value));
}

JNIEXPORT jstring JNICALL
Java_com_jmengxy_lualib_Lua_luaGetString(JNIEnv *env, jclass type, jlong luaStatePtr, jint index, jstring defaultValue) {
    return env->NewStringUTF(reinterpret_cast<LuaState*>(luaStatePtr)->getString(index, getStringFromJni(env, defaultValue)).c_str());
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaPushBoolean(JNIEnv *env, jclass type, jlong luaStatePtr,
                                           jboolean value) {
    reinterpret_cast<LuaState*>(luaStatePtr)->pushBoolean(value);
}

JNIEXPORT jboolean JNICALL
Java_com_jmengxy_lualib_Lua_luaGetBoolean(JNIEnv *env, jclass type, jlong luaStatePtr, jint index, jboolean defaultValue) {
    return (jboolean) (reinterpret_cast<LuaState*>(luaStatePtr)->getBoolean(index, defaultValue));
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaPushInteger(JNIEnv *env, jclass type, jlong luaStatePtr, jint value) {
    reinterpret_cast<LuaState*>(luaStatePtr)->pushInteger(value);
}

JNIEXPORT jint JNICALL
Java_com_jmengxy_lualib_Lua_luaGetInteger(JNIEnv *env, jclass type, jlong luaStatePtr, jint index, jint defaultValue) {
    return (jint) (reinterpret_cast<LuaState*>(luaStatePtr)->getInteger(index, defaultValue));
}

JNIEXPORT void JNICALL
Java_com_jmengxy_lualib_Lua_luaPushDouble(JNIEnv *env, jclass type, jlong luaStatePtr,
                                        jdouble value) {
    reinterpret_cast<LuaState*>(luaStatePtr)->pushDouble(value);
}

JNIEXPORT jdouble JNICALL
Java_com_jmengxy_lualib_Lua_luaGetDouble(JNIEnv *env, jclass type, jlong luaStatePtr, jint index, jdouble defaultValue) {
    return (jdouble) (reinterpret_cast<LuaState*>(luaStatePtr)->getDouble(index, defaultValue));
}

#ifdef __cplusplus
}
#endif
