package com.jmeng.luadroid;

public class Lua {
    static {
        System.loadLibrary("luax");
    }

    public native String getResult(String input);
}
