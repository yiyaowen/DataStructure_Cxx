/*
 * Copyright (c) 2021 yiyaowen
 *
 * 数据结构:C语言版/严蔚敏,吴伟民编著.（计算机系列教材）
 * --北京：清华大学出版社，1997.4 ISBN 978-7-302-02368-5
 *
 * 此为《数据结构（C语言版）》中抽象数据结构和常见算法的实现，
 * 为了优化程序结构，在某些地方可能作出了经过考量的修改和优化。
 *
 * 使用本代码时请列出原始出处和作者名称，例如：
 * Author: yiyaowen
 * From: https://github.com/yiyaowen/DataStructure_Cxx
 *
 * Also see: https://github.com/yiyaowen/DataStructure_Cxx
 *
 */
#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace DataStructure_Cxx {

#define DSCxx_VERSION "v1.0"

    constexpr int DSCxx_TRUE          =  1;
    constexpr int DSCxx_FALSE         =  0;
    constexpr int DSCxx_OK            =  1;
    constexpr int DSCxx_ERROR         =  0;
    constexpr int DSCxx_INFEASIBLE    = -1;
    constexpr int DSCxx_OVERFLOW      = -2;

    using Status = int;
    // 为了方便实现，将所有的变量都规定为整数
    // 事实上，一个整数可以代表一个地址、标识符，如句柄、指针，所以这样简化是没有问题的
    using ElemType = int;

#define StatusToString(STATUS) \
    ((STATUS) == 1 ? "TRUE/OK (value: 1)" : (STATUS) == 0 ? "FALSE/ERROR (value: 0)" : \
    (STATUS) == -1 ? "INFEASIBLE (value: -1)" : (STATUS) == -2 ? "OVERFLOW (value: -2)" : to_string(STATUS))

    // 所有数据结构类都应该继承 ADTObject 并重写 copy、str 功能
    class ADTObject {
    public:
        virtual ADTObject* copy() {
            auto pastedObj = new ADTObject;
            return pastedObj;
        }

        virtual string str() {
            return "ADTObject";
        }
    };

    class UnimplementedException : public exception {
    public:
        const char * what() const noexcept override {
            return "Virtual function unimplemented";
        }
    };

#define ENABLE_SINGLETON(class_name) \
    private: \
        class_name() = default; \
        static class_name* m_instance; \
    public: \
        static class_name* instance() { \
            if (m_instance == nullptr) { \
                m_instance = new class_name; \
            } \
            return m_instance; \
       }

#define CHECK_ARG_COUNT(arg_count) \
    if (args.size() != arg_count) {        \
        throw InstructionInvalidArgumentCountException(arg_count, args.size()); \
    }

#define SINGLETON_MEMBER(class_name) class_name* class_name::m_instance = nullptr;

    // 所有的指令类都应该继承 Function 并重写必要的虚函数，从而符合 Interactor 的调用规范
    class Function {
    public:
        Status status = DSCxx_OK;

        virtual Status invoke(const vector<string>& args) {
            throw UnimplementedException();
        }

        virtual void output() {
            cout << "Status = " << StatusToString(status) << endl;
        }
    };
}
