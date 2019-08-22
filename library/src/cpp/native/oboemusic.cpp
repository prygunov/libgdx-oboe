#include "oboemusic.hpp"
#include "../audio/audioengine.hpp"
#include "../music/music.hpp"
#include "../utility/var.hpp"
#include "../jni/jvm_class.hpp"

OBOEMUSIC_METHOD(void, setCompletionCallback) (JNIEnv* env, jobject self, jobject callback) {
    auto context = jni_context::acquire_thread();
    auto old_callback = get_var_as<_jobject>(env, self, "onComplete");
    if (old_callback != NULL) {
        context->DeleteGlobalRef(old_callback) ;
    }

    auto new_callback = context->NewGlobalRef(callback);
    set_var_as(env, self, "onComplete", new_callback);

    auto instance = shared_ptr_var<music>(env, self, "music");

    instance->on_complete([new_callback] {
        auto context = jni_context::acquire_thread();
        auto callback_class = jvm_class(context->GetObjectClass(new_callback));
        callback_class.execute_method<void()>(new_callback, "invoke");
    });
}

OBOEMUSIC_METHOD(bool, isPlaying) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->is_playing();
}

OBOEMUSIC_METHOD(bool, isLooping) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->is_looping();
}

OBOEMUSIC_METHOD(void, pause) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->pause();
}

OBOEMUSIC_METHOD(void, setPan) (JNIEnv* env, jobject self, jfloat pan, jfloat volume) {
    auto instance = shared_ptr_var<music>(env, self, "music");
    instance->pan(pan);
    instance->volume(volume);
}

OBOEMUSIC_METHOD(jfloat, getPosition) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->position();
}

OBOEMUSIC_METHOD(void, setLooping) (JNIEnv* env, jobject self, jboolean loop) {
    return shared_ptr_var<music>(env, self, "music")->is_looping(loop);
}

OBOEMUSIC_METHOD(jfloat, getVolume) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->volume();
}

OBOEMUSIC_METHOD(void, play) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->play();
}

OBOEMUSIC_METHOD(void, stop) (JNIEnv* env, jobject self) {
    return shared_ptr_var<music>(env, self, "music")->stop();
}

OBOEMUSIC_METHOD(void, setVolume) (JNIEnv* env, jobject self, jfloat volume) {
    return shared_ptr_var<music>(env, self, "music")->volume(volume);
}

OBOEMUSIC_METHOD(void, setPosition) (JNIEnv* env, jobject self, jfloat position) {
    shared_ptr_var<music>(env, self, "music")->position(position);
}

OBOEMUSIC_METHOD(void, dispose) (JNIEnv* env, jobject self) {
    delete get_var_as<std::shared_ptr<music>>(env, self, "music");
    delete get_var_as<jobject>(env, self, "onComplete");
}

