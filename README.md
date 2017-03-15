# LuaDroid
Lua interpreter on Android

How to import LuaDroid into Android project:

1. Add the JitPack repository to your build file
    - gradle
        Add it in your root build.gradle at the end of repositories:
        allprojects {
            repositories {
                ...
                maven { url 'https://jitpack.io' }
            }
    - maven
        <repositories>
            <repository>
                <id>jitpack.io</id>
                <url>https://jitpack.io</url>
            </repository>
        </repositories>}
2. Add the dependency
    - gradle
        dependencies {
                compile 'com.github.joshua-meng:LuaDroid:V1.0.0'
        }
    - maven
        <dependency>
            <groupId>com.github.joshua-meng</groupId>
            <artifactId>LuaDroid</artifactId>
            <version>V1.0.0</version>
        </dependency>
