#include <node.h>

// C standard library
extern "C"{
#include "gpio_lib.h"
}

#define WIRING_DEFINE_CONSTANT(NAME, VALUE) (target)->Set( \
        v8::String::NewSymbol(NAME), \
        v8::Integer::New(VALUE), \
        static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete) \
);

#define PD0    SUNXI_GPD(0)

using namespace v8;


Handle<Value> GPIO_init(const Arguments& args) {

    HandleScope scope;

    int32_t result;
    result = sunxi_gpio_init();

    if(result == SETUP_DEVMEM_FAIL){
	return ThrowException(
            Exception::TypeError(String::New("SETUP_DEVMEM_FAIL Error"))
        );
    }

    if(result == SETUP_MALLOC_FAIL){
        return ThrowException(
            Exception::TypeError(String::New("SETUP_MALLOC_FAIL Error"))
        );
    }

    if(result == SETUP_MMAP_FAIL){
        return ThrowException(
            Exception::TypeError(String::New("SETUP_MMAP_FAIL Error"))
        ); 
    }

    return scope.Close(Integer::New(SETUP_OK));
}

Handle<Value> GPIO_cleanup(const Arguments& args) {
    HandleScope scope;

    sunxi_gpio_cleanup();

    return scope.Close(Undefined());

}

Handle<Value> GPIO_getcfg(const Arguments& args) {
    HandleScope scope;
    
    if (args.Length() < 1){
        return ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
    }

    if (!args[0]->IsNumber()){
        return ThrowException(
             Exception::TypeError(String::New("Wrong arguments"))
        );
    }

    int32_t result;
    int32_t gpio = args[0]->ToInteger()->Value();

    result = sunxi_gpio_get_cfgpin(gpio);

    return scope.Close(Integer::New(result));
}

Handle<Value> GPIO_input(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 1){
        return ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
    }

    if (!args[0]->IsNumber()){
        return ThrowException(
             Exception::TypeError(String::New("Wrong arguments"))
        );
    }

    int32_t result;
    int32_t gpio = args[0]->ToInteger()->Value();

    result = sunxi_gpio_input(gpio);

    if(result == -1){
        return ThrowException(
             Exception::TypeError(String::New("Reading pin failed"))
        );
    }

    return scope.Close(Integer::New(result));

}



Handle<Value> GPIO_output(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 2){
        return ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );	
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()){
        return ThrowException(           
	     Exception::TypeError(String::New("Wrong arguments"))
        );
    }

    int32_t gpio = args[0]->ToInteger()->Value();
    int32_t value = args[1]->ToInteger()->Value();

    if( value != 0 && value != 1) {
        return ThrowException(                        
	     Exception::TypeError(String::New("Invalid output state"))
        );
    }

    if(sunxi_gpio_get_cfgpin(gpio) != SUNXI_GPIO_OUTPUT) {
	return ThrowException( 
             Exception::TypeError(String::New("Invalid output state"))
        );
    }

    sunxi_gpio_output(gpio, value);

    return scope.Close(Undefined());

}

Handle<Value> GPIO_setcfg(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 2){
        return ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()){
        return ThrowException(
             Exception::TypeError(String::New("Wrong arguments"))
        );
    }

    int32_t gpio = args[0]->ToInteger()->Value();
    int32_t direction = args[1]->ToInteger()->Value();

    if(direction != 0 && direction != 1 && direction != 2) {
        return ThrowException(
             Exception::TypeError(String::New("Invalid direction"))
        );
    }

    sunxi_gpio_set_cfgpin(gpio, direction);
    return scope.Close(Undefined());

}


void RegisterModule(Handle<Object> target) {

    NODE_SET_METHOD(target, "init", GPIO_init);
    NODE_SET_METHOD(target, "cleanup", GPIO_cleanup);
    NODE_SET_METHOD(target, "output", GPIO_output);
    NODE_SET_METHOD(target, "setcfg", GPIO_setcfg);
    NODE_SET_METHOD(target, "input", GPIO_input);
    NODE_SET_METHOD(target, "getcfg", GPIO_getcfg);

    WIRING_DEFINE_CONSTANT("HIGH", HIGH)
    WIRING_DEFINE_CONSTANT("LOW", LOW)
    WIRING_DEFINE_CONSTANT("PD0", PD0)
    WIRING_DEFINE_CONSTANT("IN", INPUT)
    WIRING_DEFINE_CONSTANT("OUT", OUTPUT)
    WIRING_DEFINE_CONSTANT("PER", PER)
}

NODE_MODULE(gpio, RegisterModule);
