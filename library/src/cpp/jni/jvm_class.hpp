#pragma once
#include "jni.h"
#include "jvm_signature.hpp"
#include "jni_context.hpp"
#include "jvm_object.hpp"
#include <memory>
#include <string_view>
#include <type_traits>

// TODO: cache fieldID and methodID
class jvm_class {
    public:
        jvm_class(std::string_view p_class_name) {
            auto context = jni_context::acquire_thread();
            m_class = jvm_object(context->FindClass(p_class_name.data()));
        }

        jvm_class(jclass p_class) : m_class(p_class) {}

        template <class... Args>
        jobject construct(Args... p_args) {
            auto context = jni_context::acquire_thread();
            auto constructor = find_method<void(Args...)>("<init>");
            return context->NewObject(m_class, constructor, p_args...);
        }

        template <class F>
        jmethodID find_method(std::string_view p_name) {
            auto context = jni_context::acquire_thread();
            return context->GetMethodID(m_class, p_name.data(), jvm_signature<F>());
        }

        template <class F>
        jfieldID find_field(std::string_view p_name) {
            auto context = jni_context::acquire_thread();
            return context->GetFieldID(m_class, p_name.data(), jvm_signature<F>());
        }

        template <class F>
        auto get_field(jobject p_obj, std::string_view p_name) {
            auto context = jni_context::acquire_thread();
            auto field = find_field<F>(p_name);

            if constexpr (std::is_same<F, int>::value) return context->GetIntField(p_obj, field);
            else if constexpr (std::is_same<F, bool>::value) return context->GetBooleanField(p_obj, field);
            else if constexpr (std::is_same<F, char>::value) return context->GetCharField(p_obj, field);
            else if constexpr (std::is_same<F, short>::value) return context->GetShortField(p_obj, field);
            else if constexpr (std::is_same<F, long>::value) return context->GetLongField(p_obj, field);
            else if constexpr (std::is_same<F, float>::value) return context->GetFloatField(p_obj, field);
            else if constexpr (std::is_same<F, double>::value) return context->GetDoubleField(p_obj, field);
            else return static_cast<F>(context->GetObjectField(p_obj, field));
        }

        template <class F, class... Args>
        auto execute_method(jobject p_obj, std::string_view p_name, Args... p_args) {
            using return_type = typename std::invoke_result<F, Args...>::type;
            auto context = jni_context::acquire_thread();
            auto method = find_method<F>(p_name);

            if constexpr (std::is_same<return_type, int>::value) {
                return context->CallIntMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, void>::value) {
                return context->CallVoidMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, bool>::value) {
                return context->CallBooleanMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, char>::value) {
                return context->CallCharMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, short>::value) {
                return context->CallShortMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, long>::value) {
                return context->CallLongMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, float>::value) {
                return context->CallFloatMethod(p_obj, method, p_args...);
            }
            else if constexpr (std::is_same<return_type, double>::value) {
                return context->CallDoubleMethod(p_obj, method, p_args...);
            }
            else {
                return static_cast<return_type>(context->CallObjectMethod(p_obj, method, p_args...));
            }
        }
    private:
        jvm_object<jclass> m_class;
};