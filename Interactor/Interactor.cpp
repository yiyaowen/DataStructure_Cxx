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

#include "Interactor.h"

#include <iostream>
#include <regex>

using namespace std;

namespace DataStructure_Cxx {
    Interactor* Interactor::m_instance;

    void Interactor::run() {
        cout << "DataStructure_Cxx Interactor " << DSCxx_VERSION << endl;
        string instStr;
        while (true) {
            cout << ">> ";
            getline(cin, instStr);
            try {
                if (handleControlInstruction(instStr) ||
                    handleOperationInstruction(instStr) ||
                    handleVariableInstruction(instStr))
                {
                    continue;
                }
                string instName;
                auto instArgs = extractInstructionStr(instStr, instName);
                if (availableInstructions.find(instName) == availableInstructions.end()) {
                    throw InstructionNotFoundException(instName);
                }
                invoke(availableInstructions.at(instName), instArgs);
            }
            catch (const invalid_argument& iae) {
                InstructionInvalidArgumentException iiae(iae.what());
                cout << iiae.what() << endl;
            }
            catch (const exception& e) {
                cout << e.what() << endl;
            }
        }
    }

    vector<string> Interactor::extractInstructionStr(const string& instStr, string& instName) {
        regex instRegex(R"((\w+)\((\s*(\w+)\s*,)*(\s*(\w+)\s*)\))");
        regex instRegexVoidArg(R"((\w+)\(\))");
        smatch strMatch;
        vector<string> instArgs;
        if (!regex_match(instStr, strMatch, instRegex)) {
            if (!regex_match(instStr, strMatch, instRegexVoidArg)) {
                throw InstructionInvalidFormatException();
            }
            else {
                instName = strMatch[1];
                return instArgs;
            }
        }
        instName = strMatch[1];
        string argListStr = instStr.substr(instName.size());
        regex singleArg(R"(\w+)");
        sregex_iterator pos(argListStr.begin(), argListStr.end(), singleArg);
        sregex_iterator end;
        while (pos != end) {
            instArgs.push_back(pos->str(0));
            ++pos;
        }
        return instArgs;
    }

    void Interactor::invoke(Function *func, const vector<string>& args) {
        func->status = func->invoke(args);
        func->output();
    }

    void Interactor::addInstruction(const string &name, Function *func) {
        availableInstructions.insert({ name, func });
    }

    void Interactor::createADT(const string &name, const string& adtType) {
        if (userCreatedADTs.find(name) != userCreatedADTs.end()) {
            throw ConflictUserDefinedNameException(name);
        }
        try {
            userCreatedADTs.insert({ name, availableADTs.at(adtType)->copy() });
        }
        catch (const exception& e) {
            throw OperateObjectFailedException("Create", "ADT", name,
                "Target ADT type not supported.");
        }
    }

    void Interactor::deleteADT(const string &name) {
        try {
            delete userCreatedADTs.at(name);
            userCreatedADTs.erase(name);
        }
        catch (const exception& e) {
            throw OperateObjectFailedException("Delete", "ADT", name);
        }
    }

    ADTObject* Interactor::getADT(const string &name) {
        try {
            return userCreatedADTs.at(name);
        }
        catch(const exception& e) {
            throw OperateObjectFailedException("Search", "ADT", name,
                "Target ADT not exists.");
        }
    }

    void Interactor::createVariable(const string &name) {
        if (userCreatedVariables.find(name) != userCreatedVariables.end()) {
            throw ConflictUserDefinedNameException(name);
        }
        try {
            auto newVar = new ElemType;
            userCreatedVariables.insert({ name, newVar });
        }
        catch(const exception& e) {
            throw OperateObjectFailedException("Create", "Variable", name);
        }
    }

    void Interactor::deleteVariable(const string &name) {
        try {
            delete userCreatedVariables.at(name);
            userCreatedVariables.erase(name);
        }
        catch(const exception& e) {
            throw OperateObjectFailedException("Delete", "Variable", name,
                "Target variable not exists.");
        }
    }

    ElemType* Interactor::getVariable(const string &name) {
        try {
            return userCreatedVariables.at(name);
        }
        catch(const exception& e) {
            throw OperateObjectFailedException("Search", "Variable", name,
                "Target variable not exists.");
        }
    }

    void Interactor::addAdtType(const string &name, ADTObject *obj) {
        availableADTs.insert({ name, obj });
    }

    bool Interactor::handleControlInstruction(const string &instStr) {
        if (instStr.size() != 2 || instStr.at(0) != '/') return false;
        switch (instStr.at(1)) {
            case 'q':
                std::exit(DSCxx_OK);
            case '?':
                showHelpText();
                break;
            default:
                return false;
        }
        return true;
    }

    bool Interactor::handleOperationInstruction(const string &instStr) {
        regex newInstRegex(R"(new (\w+) (\w+))"); // 格式：new [adtType] [name] 或者 new var [name]
        regex deleteInstRegex(R"(delete (\w+) (\w+))"); // 格式 delete adt|var [name]
        regex listInstRegex(R"(list (adt|var))");
        smatch strMatch;
        if (regex_match(instStr, strMatch, newInstRegex)) {
            string type = strMatch[1];
            string name = strMatch[2];
            if (type == "var") {
                createVariable(name);
            }
            else {
                createADT(name, type);
            }
            return true;
        }
        else if (regex_match(instStr, strMatch, deleteInstRegex)) {
            string type = strMatch[1];
            string name = strMatch[2];
            if (type == "adt") {
                deleteADT(name);
            }
            else if (type == "var") {
                deleteVariable(name);
            }
            else {
                return false;
            }
            return true;
        }
        else if (regex_match(instStr, strMatch, listInstRegex)) {
            if (strMatch[1] == "adt") {
                listUserCreatedAdts();
            }
            else if (strMatch[1] == "var") {
                listUserCreateVariables();
            }
            else {
                return false;
            }
            return true;
        }
        return false;
    }

    bool Interactor::handleVariableInstruction(const string &instStr) {
        regex r(R"((\w+)\s*=\s*(\w+))");
        smatch sm;
        if (regex_match(instStr, sm, r)) {
            auto left = getVariable(sm[1]);
            // 这里要防止获取右边的参数时直接抛出异常，因为有可能这是一个整数字面值
            try {
                *left = *getVariable(sm[2]);
            }
            catch (const OperateObjectFailedException& oofe) {
                *left = stoi(sm[2]);
            }
            return true;
        }
        else if (userCreatedVariables.find(instStr) != userCreatedVariables.end()) {
            std::cout << *(userCreatedVariables.at(instStr)) << endl;
            return true;
        }
        return false;
    }

    void Interactor::listUserCreatedAdts() {
        for (auto& elem : userCreatedADTs) {
            cout << elem.second->str() << " " << elem.first << endl;
        }
    }

    void Interactor::listUserCreateVariables() {
        for (auto& elem : userCreatedVariables) {
            cout << elem.first << " = " << *(elem.second) << endl;
        }
    }

    void Interactor::showHelpText() {
        cout << "\t/q\tQuit" << endl;
        cout << "\t/?\tHelp" << endl;
    }
}
