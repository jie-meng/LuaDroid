#include <string>
#include <jni.h>
#include "lua/lua.hpp"

using namespace std;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); TypeName& operator=(const TypeName&);
typedef int (*LuaCFunc)(lua_State*);

//lua utility functions
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
extern "C" {
#endif

JNIEXPORT jlong JNICALL
Java_com_jmeng_luadroid_Lua_newLuaState(JNIEnv *env, jclass type) {

    return reinterpret_cast<jlong>(new LuaState());
}

JNIEXPORT void JNICALL
Java_com_jmeng_luadroid_Lua_deleteLuaState(JNIEnv *env, jclass type, jlong luaStatePtr) {

    if (0 != luaStatePtr)
        delete reinterpret_cast<LuaState*>(luaStatePtr);
}

JNIEXPORT jint JNICALL
Java_com_jmeng_luadroid_Lua_luaParseLine(JNIEnv *env, jclass type, jlong luaStatePtr,
                                         jstring line_) {
    if (0 == luaStatePtr)
        return -1;

    const char *line = env->GetStringUTFChars(line_, 0);
    string lineStr(line);
    env->ReleaseStringUTFChars(line_, line);

    auto plua_state = reinterpret_cast<LuaState*>(luaStatePtr);
    return plua_state->parseLine(lineStr);
}

JNIEXPORT jint JNICALL
Java_com_jmeng_luadroid_Lua_luaParseFile(JNIEnv *env, jclass type, jlong luaStatePtr,
                                         jstring file_) {
    if (0 == luaStatePtr)
        return -1;

    const char *file = env->GetStringUTFChars(file_, 0);
    string fileStr(file);
    env->ReleaseStringUTFChars(file_, file);

    auto plua_state = reinterpret_cast<LuaState*>(luaStatePtr);
    return plua_state->parseFile(fileStr);
}

JNIEXPORT jstring JNICALL
Java_com_jmeng_luadroid_Lua_luaGetError(JNIEnv *env, jclass type, jlong luaStatePtr, jint errCode) {

    if (0 == luaStatePtr || -1 == errCode)
        return env->NewStringUTF("Invalid luaState");

    return env->NewStringUTF(luaGetError(reinterpret_cast<lua_State*>(luaStatePtr), errCode).c_str());
}

#ifdef __cplusplus
}
#endif
