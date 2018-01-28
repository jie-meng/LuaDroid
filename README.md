# LuaDroid -- Lua interpreter for Android

[![Build Status](https://travis-ci.org/jie-meng/LuaDroid.svg?branch=master)](https://travis-ci.org/jie-meng/LuaDroid)

## How to import LuaDroid into Android project

1. Add the JitPack repository to your build file
    - gradle

        Add it in your root build.gradle at the end of repositories:
        
        ```
        allprojects {
            repositories {
                ...
                maven { url 'https://jitpack.io' }
            }
        }
        ```
        
    - maven
    
        ```
        <repositories>
            <repository>
                <id>jitpack.io</id>
                <url>https://jitpack.io</url>
            </repository>
        </repositories>
        ```
        
2. Add the dependency
    - gradle
        
        ```
        dependencies {
                compile 'com.github.jie-meng:LuaDroid:V1.0.0'
        }
        ```
        
    - maven
        
        ```
        <dependency>
            <groupId>com.github.jie-meng</groupId>
            <artifactId>LuaDroid</artifactId>
            <version>V1.0.0</version>
        </dependency>
        ```

## Quick start

```
//create instance
lua = new Lua();

//get
String s = lua.getString("s", "");
int i = lua.getInteger("i", 0);
double d = lua.getDouble("d", 0.0);
boolean b = lua.getBoolean("b", false);

//set
lua.setString("s", "test_string");
lua.setInteger("i", 21);
lua.setDouble("d", 3.14);
lua.setBoolean("b", true);

//others
int type = lua.getType("s");
boolean b = lua.isInteger("i");
Pair<Boolean, String> result = lua.parseLine("s = string.lower('Text')");
Pair<Boolean, String> result = lua.parseFile("/home/user/test.lua");

//free resource
lua.close();
```
