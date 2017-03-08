#include <string>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

JNIEXPORT jstring JNICALL
Java_com_jmeng_luadroid_Lua_getResult(JNIEnv *env, jobject instance, jstring input_) {
    const char *input = env->GetStringUTFChars(input_, 0);

    string s(input);
    s.append("****");

    env->ReleaseStringUTFChars(input_, input);

    return env->NewStringUTF(s.c_str());
}

#ifdef __cplusplus
}
#endif