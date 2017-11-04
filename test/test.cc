// Copyright 2013 <chaishushan#gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _DEFER_H_
#define _DEFER_H_

#include <functional>
#include <iostream>

//
// 需要支持C11 -std=c++11
// defer 语句定义一个延迟执行闭包函数的对象
//
// 基于用法:
//
//  FILE* fp = fopen("foo.txt", "rt");
//  if(fp == NULL) return false;
//  defer([&](){ printf("fclose(fp)\n"); fclose(fp); });
//
//  char* buf = new char[1024];
//  defer([&](){ printf("delete buf\n"); delete[] buf; });
//
//  defer([](){ printf("defer a: %d\n", __LINE__); });
//  defer([](){ printf("defer a: %d\n", __LINE__); });
//  defer([](){ printf("defer a: %d\n", __LINE__); });
//
//  {
//      defer([](){ printf("defer b: %d\n", __LINE__); });
//      defer([](){ printf("defer b: %d\n", __LINE__); });
//      defer([](){ printf("defer b: %d\n", __LINE__); });
//  }
//
//  defer([](){
//      printf("defer c:\n");
//      for(int i = 0; i < 3; ++i) {
//          defer([&](){ defer([&](){
//              printf("\ti = %d: begin\n", i);
//              defer([&](){ printf("\ti = %d\n", i); });
//              printf("\ti = %d: end\n", i);
//          });});
//      }
//  });
//
// 注意事项:
//
// 1. defer 定义的对象在超出作用域时执行闭包函数(析构函数)
// 2. defer 定义的对象在同一个文件内部标识符不同(根据行号生成)
// 3. defer 在全局作用域使用可能会出现重名现象(行号相同)
// 4. defer 在判断语句使用可能提前执行(作用域结束时)
// 5. defer 在循环语句内使用无效(作用域结束时)
// 6. defer 和Go语言的defer并不完全等价
//
// 更多参考:
//
// http://blog.korfuri.fr/post/go-defer-in-cpp/
// http://blog.korfuri.fr/attachments/go-defer-in-cpp/defer.hh
// http://blogs.msdn.com/b/vcblog/archive/2011/09/12/10209291.aspx
// http://golang.org/doc/effective_go.html#defer
// http://golang.org/ref/spec#Defer_statements
//

#define defer _DEFER_ACTION_MAKE /* ([&](){ ... }); */

// auto _defer_action_line???_ = _DeferredActionCtor([&](){ ... })
#define _DEFER_ACTION_MAKE auto \
                               _DEFER_ACTION_VAR(_defer_action_line, __LINE__, _) = _DeferredActionCtor
#define _DEFER_ACTION_VAR(a, b, c) _DEFER_TOKEN_CONNECT(a, b, c)
#define _DEFER_TOKEN_CONNECT(a, b, c) a##b##c

// 持有闭包函数
class _DeferredAction
{
  private:
    std::function<void()> func_;

    template <typename T>
    friend _DeferredAction _DeferredActionCtor(T &&p);

    template <typename T>
    _DeferredAction(T &&p) : func_(std::bind(std::forward<T>(p)))
    {
        std::cout << "NEW 1" << std::endl;
    }

    _DeferredAction();
    _DeferredAction(_DeferredAction const &);
    _DeferredAction &operator=(_DeferredAction const &);
    _DeferredAction &operator=(_DeferredAction &&);

  public:
    _DeferredAction(_DeferredAction &&other) : func_(std::forward<std::function<void()>>(other.func_))
    {
        other.func_ = nullptr;
        std::cout << "NEW 2" << std::endl;
    }
    ~_DeferredAction()
    {
        if (func_)
        {
            func_();
        }
        std::cout << "DELETE" << std::endl;
    }
};

template <typename T>
_DeferredAction _DeferredActionCtor(T &&p)
{
    return _DeferredAction(std::forward<T>(p));
}

#endif // _DEFER_H_

/* 
 * File:   main.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
// #include "defer.h"
#include <iostream>

void sayHello(const char *name)
{
    printf("hello %s\n", name);
}

void funTest()
{
    printf("funTest 1\n");
    defer([]() { printf("defer c: %d\n", __LINE__); });
    defer([]() { printf("defer c: %d\n", __LINE__); });
    defer([]() { printf("defer c: %d\n", __LINE__); });
    printf("funTest 2\n");
}

/*
 * 
 */
int main(void)
{
    defer([]() { printf("defer a: %d\n", __LINE__); });
    defer([]() { printf("defer a: %d\n", __LINE__); });
    defer([]() { printf("defer a: %d\n", __LINE__); });
    defer([]() { sayHello("jack"); });

    // {
    //     defer([]() { printf("defer b: %d\n", __LINE__); });
    //     defer([]() { printf("defer b: %d\n", __LINE__); });
    //     defer([]() { printf("defer b: %d\n", __LINE__); });
    //     std::cout << "in code scope" << std::endl;
    // }

    // std::cout << "\n---------------------------" << std::endl;

    // std::cout << "Done 1" << std::endl;
    // std::cout << "Done 2" << std::endl;
    // std::cout << "Done 3" << std::endl;

    // // throw "exception happen";

    // std::cout << "\n---------------------------" << std::endl;

    // funTest();

    return 0;
}