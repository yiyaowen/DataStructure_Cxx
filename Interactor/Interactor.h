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

#include "Common.h"

#include <iostream>
#include <string>
#include <unordered_map>

using namespace  std;

namespace DataStructure_Cxx {

    // 加载函数实例的宏，参见 Interactor::addInstruction 方法即可明白此宏的作用
#define LoadFunc(func_name) Interactor::instance()->addInstruction(#func_name, func_name::instance())

    // Interactor，即命令交互器，负责提示用户输入、解析命名字符串、动态调用函数、反馈函数执行结果等
    class Interactor {
    private:
        Interactor() = default;
        static Interactor* m_instance;

    public:
        static inline Interactor* instance() {
            if (m_instance == nullptr) {
                m_instance = new Interactor;
            }
            return m_instance;
        }

    private:
        // 可调用的函数指令，这个 map 在初始化的时候即应该定义好，运行时不会改动
        unordered_map<string, Function*> availableInstructions;
        // [数据结构的字符串名称] : [程序维护的标准数据结构样本]（用来复制产生用户创建的数据结构）
        unordered_map<string, ADTObject*> availableADTs;
        // [用户命名的标识符] : [实际存储的数据结构对象]
        unordered_map<string, ADTObject*> userCreatedADTs;
        // [用户命名的变量名称] : [实际存储的变量]
        unordered_map<string, ElemType*> userCreatedVariables;

    public:
        void run();
        vector<string> extractInstructionStr(const string& argStr, string& instName);
        void invoke(Function* func, const vector<string>& args);

        void addInstruction(const string& name, Function* func);
        void addAdtType(const string& name, ADTObject* obj);

        // 下面关于 ADT、Variable 的 create、delete、get 方法均会处理异常，
        // 所以无需在调用这些方法前对传入的字符串参数进行额外判断
        void createADT(const string& name, const string& adtType);
        void deleteADT(const string& name);
        ADTObject* getADT(const string& name);

        void createVariable(const string& name);
        void deleteVariable(const string& name);
        ElemType* getVariable(const string& name);

        // 处理诸如"退出程序"等控制命令
        bool handleControlInstruction(const string& instStr);
        // 处理诸如"创建"、"删除"等操作命令
        bool handleOperationInstruction(const string& instStr);
        // 当用户直接输入变量名时，显示其内容
        bool handleVariableInstruction(const string& instStr);

        void listUserCreatedAdts();
        void listUserCreateVariables();
        void showHelpText();
    };

    // 以下是自定义的异常，用来给用户提供多样、精确的反馈结果

    class InstructionNotFoundException : public exception {
    private:
        string msg;

    public:
        explicit InstructionNotFoundException(const string& instName) : exception() {
            msg = "Instruction \"";
            msg += instName;
            msg += "\" not found.";
        }

        const char * what() const noexcept override {
            return msg.c_str();
        }
    };

    class InstructionInvalidFormatException : public exception {
    public:
        const char * what() const noexcept override {
            return "Invalid instruction format. Examples: Add(1, 2), Quit().";
        }
    };

    class InstructionInvalidArgumentException : public exception {
    private:
        string msg;

    public:
        explicit InstructionInvalidArgumentException(const string& argName) : exception() {
            msg = "Invalid argument: \"";
            msg += argName;
            msg += "\".";
        }

        const char * what() const noexcept override {
            return msg.c_str();
        }
    };

    class InstructionInvalidArgumentCountException : public exception {
    private:
        string msg;

    public:
        InstructionInvalidArgumentCountException(size_t need, size_t given) : exception() {
            msg = "Invalid argument count: ";
            msg += to_string(need) + " is needed, but " + to_string(given) + " is given.";
        }

        const char * what() const noexcept override {
            return msg.c_str();
        }
    };

    class OperateObjectFailedException : public exception {
    private:
        string msg;

    public:
        OperateObjectFailedException(
            const string& operateWord, const string& objType,
            const string& name, const string& reason = "")
        {
            msg =  operateWord + " " + objType + " \"" + name + "\" failed.";
            if (!reason.empty()) {
                msg += (" Reason: " + reason);
            }
        }

        const char * what() const noexcept override {
            return msg.c_str();
        }
    };

    class ConflictUserDefinedNameException : public exception {
    private:
        string msg;

    public:
        explicit  ConflictUserDefinedNameException(const string& name) {
            msg = "ADT/Variable with name \"" + name + "\" already exists.";
        }

        const char * what() const noexcept override {
            return msg.c_str();
        }
    };
}