package com.jmeng.luadroid;

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

    public String getString(String name) {
        luaGetGlobal(luaState, name);
        String s = luaGetString(luaState, 1);
        luaPop(luaState, -1);
        return s;
    }

    public void setString(String name, String value) {
        luaPushString(luaState, value);
        luaSetGlobal(luaState, name);
    }

    public boolean getBoolean(String name) {
        luaGetGlobal(luaState, name);
        boolean b = luaGetBoolean(luaState, 1);
        luaPop(luaState, -1);
        return b;
    }

    public void setBoolean(String name, boolean value) {
        luaPushBoolean(luaState, value);
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

    private static native void luaPushString(long luaStatePtr, String str);

    private static native String luaGetString(long luaStatePtr, int index);

    private static native void luaPushBoolean(long luaStatePtr, boolean data);

    private static native boolean luaGetBoolean(long luaStatePtr, int index);
}
