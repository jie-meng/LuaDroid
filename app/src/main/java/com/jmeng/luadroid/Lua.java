package com.jmeng.luadroid;

import android.util.Pair;

import java.io.Closeable;
import java.io.IOException;

import static android.util.Pair.create;

public final class Lua {

    private long luaState = 0;

    public Lua() {
        init();
    }

    public boolean inValid() {
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
}
