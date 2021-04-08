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
#include "Interactor.h"

#include <cstdlib>

namespace DataStructure_Cxx
{
    class Triplet : public ADTObject {
    public:
        ElemType* p = nullptr;
        ADTObject* copy() override {
            auto pastedObj = new Triplet;
            pastedObj->p = p;
            return pastedObj;
        }
        string str() override {
            return "Triplet";
        }
    };

    class InitTriplet : public Function {
        ENABLE_SINGLETON(InitTriplet)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(4)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            pTriplet->p = (ElemType*)std::malloc(3 * sizeof(ElemType));
            if (!pTriplet->p) exit(DSCxx_OVERFLOW);
            for (size_t i = 0; i < 3; ++i) {
                pTriplet->p[i] = stoi(args[i + 1]);
            }
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(InitTriplet)

    class DestroyTriplet : public Function {
        ENABLE_SINGLETON(DestroyTriplet)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            free(pTriplet->p);
            pTriplet->p = nullptr;
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(DestroyTriplet)

    class GetElemInTriplet : public Function {
        ENABLE_SINGLETON(GetElemInTriplet)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            int i = stoi(args[1]);
            ElemType* pVar = Interactor::instance()->getVariable(args[2]);
            if (i < 1 || i > 3) {
                return DSCxx_ERROR;
            }
            *pVar = pTriplet->p[i - 1];
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(GetElemInTriplet)

    class PutElemIntoTriplet : public Function {
        ENABLE_SINGLETON(PutElemIntoTriplet)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            int i = stoi(args[1]);
            ElemType value = stoi(args[2]);
            if (i < 1 || i > 3) {
                return DSCxx_ERROR;
            }
            pTriplet->p[i - 1] = value;
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(PutElemIntoTriplet)

    class IsTripletAscending : public Function {
        ENABLE_SINGLETON(IsTripletAscending)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            if (pTriplet->p[0] <= pTriplet->p[1] && pTriplet->p[1] <= pTriplet->p[2]) {
                return DSCxx_TRUE;
            }
            else {
                return DSCxx_FALSE;
            }
        }
    };
    SINGLETON_MEMBER(IsTripletAscending)

    class IsTripletDescending : public Function {
        ENABLE_SINGLETON(IsTripletDescending)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            if (pTriplet->p[0] >= pTriplet->p[1] && pTriplet->p[1] >= pTriplet->p[2]) {
                return DSCxx_TRUE;
            }
            else {
                return DSCxx_FALSE;
            }
        }
    };
    SINGLETON_MEMBER(IsTripletDescending)

    class GetMaxInTriplet : public Function {
        ENABLE_SINGLETON(GetMaxInTriplet)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(2)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            ElemType* pVar = Interactor::instance()->getVariable(args[1]);
            *pVar = (pTriplet->p[0] >= pTriplet->p[1]) ?
                    (pTriplet->p[0] >= pTriplet->p[2]) ? pTriplet->p[0] : pTriplet->p[2] :
                    (pTriplet->p[1] >= pTriplet->p[2]) ? pTriplet->p[1] : pTriplet->p[2];
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(GetMaxInTriplet)

    class GetMinInTriplet : public Function {
    ENABLE_SINGLETON(GetMinInTriplet)
    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(2)
            Triplet* pTriplet = (Triplet*)Interactor::instance()->getADT(args[0]);
            if (pTriplet->p == nullptr) {
                return DSCxx_ERROR;
            }
            ElemType* pVar = Interactor::instance()->getVariable(args[1]);
            *pVar = (pTriplet->p[0] <= pTriplet->p[1]) ?
                    (pTriplet->p[0] <= pTriplet->p[2]) ? pTriplet->p[0] : pTriplet->p[2] :
                    (pTriplet->p[1] <= pTriplet->p[2]) ? pTriplet->p[1] : pTriplet->p[2];
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(GetMinInTriplet)
}
