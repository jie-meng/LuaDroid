package com.jmeng.lualib;

import android.util.Pair;

import static android.util.Pair.create;

public final class Lua {

    private long luaState = 0;

    public Lua() {
        init();
    }

    public boolean isValid() {
        return luaState != 0;
    }

    public void init() {
        close();
        luaState = newLuaState();
    }

    public Pair<Boolean, String> parseLine(String line) {
        int errCode = luaParseLine(luaState, line);
        String message = luaGetError(luaState, errCode);

        return create(0 == errCode, message);
    }

    public Pair<Boolean, String> parseFile(String file) {
        int errCode = luaParseFile(luaState, file);
        String message = luaGetError(luaState, errCode);

        return create(0 == errCode, message);
    }

    public String getString(String name, String defaultValue) {
        luaGetGlobal(luaState, name);
        String s = luaGetString(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return s;
    }

    public void setString(String name, String value) {
        luaPushString(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public boolean getBoolean(String name, boolean defaultValue) {
        luaGetGlobal(luaState, name);
        boolean value = luaGetBoolean(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return value;
    }

    public void setBoolean(String name, boolean value) {
        luaPushBoolean(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public int getInteger(String name, int defaultValue) {
        luaGetGlobal(luaState, name);
        int value = luaGetInteger(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return value;
    }

    public void setInteger(String name, int value) {
        luaPushInteger(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public double getDouble(String name, double defaultValue) {
        luaGetGlobal(luaState, name);
        double value = luaGetDouble(luaState, 1, defaultValue);
        luaPop(luaState, -1);
        return value;
    }

    public void setDouble(String name, double value) {
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

    static {
        System.loadLibrary("luax");
    }

    private static native long newLuaState();

    private static native void deleteLuaState(long luaStatePtr);

    private static native int luaParseLine(long luaStatePtr, String line);

    private static native int luaParseFile(long luaStatePtr, String file);

    private static native String luaGetError(long luaStatePtr, int errCode);

    private static native void luaGetGlobal(long luaStatePtr, String name);

    private static native void luaSetGlobal(long luaStatePtr, String name);

    private static native void luaPop(long luaStatePtr, int index);

    private static native void luaPushString(long luaStatePtr, String value);

    private static native String luaGetString(long luaStatePtr, int index, String defaultValue);

    private static native void luaPushBoolean(long luaStatePtr, boolean value);

    private static native boolean luaGetBoolean(long luaStatePtr, int index, boolean defaultValue);

    private static native void luaPushInteger(long luaStatePtr, int value);

    private static native int luaGetInteger(long luaStatePtr, int index, int defaultValue);

    private static native void luaPushDouble(long luaStatePtr, double value);

    private static native double luaGetDouble(long luaStatePtr, int index, double defaultValue);
}
