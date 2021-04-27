#include<sstream>
#include<iostream>
#include<cstdio>
#include<cstring>
#include"quickjspp.hpp"
int main(int argc,char*argv[]){
	qjs::Runtime runtime;
	qjs::Context context(runtime);
	JSRuntime*rt;
	JSContext*ctx;
	rt=runtime.rt;
	ctx=context.ctx;
	js_std_init_handlers(rt);
	JS_SetModuleLoaderFunc(rt,nullptr,js_module_loader,nullptr);
	js_std_add_helpers(ctx,0,nullptr);
	js_init_module_std(ctx,"std");
	js_init_module_os(ctx,"os");
	context.eval(R"(
		import * as std from 'std';
		import * as os from 'os';
		globalThis.std = std;
		globalThis.os = os;
	)","<input>",JS_EVAL_TYPE_MODULE);
	{
		auto& module=context.addModule("Config");
		module.function("get_os",[](){
#ifdef _WIN32
				return "windows";
#else
				return "linux";
#endif
		});
	}
	try{
		context.evalFile("./js/a.js",JS_EVAL_TYPE_MODULE);
	}catch(qjs::exception){
		auto exc=context.getException();
		std::cerr<<(std::string)exc<<std::endl;
		if((bool)exc["stack"])
			std::cerr<<(std::string)exc["stack"]<<std::endl;
	}

}
