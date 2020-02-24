#include <gtk/gtk.h>
#include <jni.h>

JNIEXPORT void JNICALL Java_gprint_show(JNIEnv *env, jobject *obj, jstring message)
{
 const char *str = 	(*env)->GetStringUTFChars( env, message, 0 );

	printf("%s\n", str);
}

