#ifndef MODEL_PARAMETER_VALUE_H
#define MODEL_PARAMETER_VALUE_H

#ifndef STANDALONE
#include <napi.h>
#endif

namespace soundboard {
namespace model {

// audio unit parameter value
struct ParameterValue {
    // parameter id
    std::string id;

    // value
    float value;

#ifndef STANDALONE
    // construct js object
    Napi::Object CreateObject(const Napi::Env &env) const {
        Napi::Object object = Napi::Object::New(env);
        object.Set(Napi::String::New(env, "id"), Napi::String::New(env, id));
        object.Set(Napi::String::New(env, "value"), Napi::Number::New(env, value));
        return object;
    }
#endif
};

} // namespace model
} // namespace soundboard

#endif // MODEL_PARAMETER_VALUE_H
