package com.jmengxy.lualib;

import android.util.Pair;

import static android.util.Pair.create;

public final class Lua {

    public static final int LUA_TYPE_NONE = -1;
    public static final int LUA_TYPE_NIL = 0;
    public static final int LUA_TYPE_BOOLEAN = 1;
    public static final int LUA_TYPE_LIGHTUSERDATA = 2;
    public static final int LUA_TYPE_NUMBER = 3;
    public static final int LUA_TYPE_STRING = 4;
    public static final int LUA_TYPE_TABLE = 5;
    public static final int LUA_TYPE_FUNCTION = 6;
    public static final int LUA_TYPE_USERDATA = 7;
    public static final int LUA_TYPE_THREAD = 8;
    public static final int LUA_TYPE_NUMTAGS = 9;

    private static final String ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED = "Lua local object is destroyed!";

    static {
        System.loadLibrary("luax");
    }

    private long luaState = 0;

    public Lua() {
        luaState = newLuaState();
    }

    private static native long newLuaState();

    private static native void deleteLuaState(long luaStatePtr);

    private static native int luaParseLine(long luaStatePtr, String line);

    private static native int luaParseFile(long luaStatePtr, String file);

    private static native String luaGetError(long luaStatePtr, int errCode);

    private static native void luaGetGlobal(long luaStatePtr, String name);

    private static native void luaSetGlobal(long luaStatePtr, String name);

    private static native int luaGetType(long luaStatePtr, int index);

    private static native void luaPop(long luaStatePtr, int index);

    private static native void luaPushString(long luaStatePtr, String value);

    private static native String luaGetString(long luaStatePtr, int index, String defaultValue);

    private static native void luaPushBoolean(long luaStatePtr, boolean value);

    private static native boolean luaGetBoolean(long luaStatePtr, int index, boolean defaultValue);

    private static native void luaPushInteger(long luaStatePtr, int value);

    private static native int luaGetInteger(long luaStatePtr, int index, int defaultValue);

    private static native void luaPushDouble(long luaStatePtr, double value);

    private static native double luaGetDouble(long luaStatePtr, int index, double defaultValue);

    public Pair<Boolean, String> parseLine(String line) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        int errCode = luaParseLine(luaState, line);
        String message = luaGetError(luaState, errCode);

        return create(0 == errCode, message);
    }

    public Pair<Boolean, String> parseFile(String file) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        int errCode = luaParseFile(luaState, file);
        String message = luaGetError(luaState, errCode);

        return create(0 == errCode, message);
    }

    //return LUA_TYPE_XXX
    public int getType(String name) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaGetGlobal(luaState, name);
        int type = luaGetType(luaState, 1);
        luaPop(luaState, -1);
        return type;
    }

    public String getString(String name, String defaultValue) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaGetGlobal(luaState, name);
        String s = luaGetString(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return s;
    }

    public void setString(String name, String value) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaPushString(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public boolean getBoolean(String name, boolean defaultValue) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaGetGlobal(luaState, name);
        boolean value = luaGetBoolean(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return value;
    }

    public void setBoolean(String name, boolean value) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaPushBoolean(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public int getInteger(String name, int defaultValue) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaGetGlobal(luaState, name);
        int value = luaGetInteger(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return value;
    }

    public void setInteger(String name, int value) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaPushInteger(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public double getDouble(String name, double defaultValue) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaGetGlobal(luaState, name);
        double value = luaGetDouble(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return value;
    }

    public void setDouble(String name, double value) {
        if (0 == luaState) {
            throw new RuntimeException(ERROR_LUA_LOCAL_OBJECT_IS_DESTROYED);
        }

        luaPushDouble(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public void close() {
        if (luaState != 0) {
            deleteLuaState(luaState);
            luaState = 0;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }
}
