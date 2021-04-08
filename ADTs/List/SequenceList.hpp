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
#include "ListPreDef.hpp"

#include <cstdlib>
#include <functional>

namespace DataStructure_Cxx {
    class SequenceList : public ADTObject {
    public:
        ElemType *elem = nullptr;   // 存储空间基址
        int length;                 // 当前实际的长度（即有多少非空的元素）
        int listsize;               // 当前分配的存储容量（以 sizeof(ElemType) 为单位）
        ADTObject *copy() override {
            auto pastedObj = new SequenceList;
            pastedObj->elem = elem;
            pastedObj->length = length;
            pastedObj->listsize = listsize;
            return pastedObj;
        }

        string str() override {
            return "SequenceList";
        }
    };

    class InitSequenceList : public Function {
    ENABLE_SINGLETON(InitSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            pList->elem = (ElemType *) malloc(LIST_INIT_SIZE * sizeof(ElemType));
            if (!pList->elem) exit(DSCxx_OVERFLOW);
            pList->length = 0;
            pList->listsize = LIST_INIT_SIZE;
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(InitSequenceList);

    class DestroySequenceList : public Function {
    ENABLE_SINGLETON(DestroySequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            free(pList->elem);
            pList->elem = nullptr;
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(DestroySequenceList)

    class ClearSequenceList : public Function {
    ENABLE_SINGLETON(ClearSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            // 清除操作实际上是：首先销毁，然后再初始化
            DestroySequenceList::instance()->invoke(args);
            InitSequenceList::instance()->invoke(args);
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(ClearSequenceList)

    class IsSequenceListEmpty : public Function {
    ENABLE_SINGLETON(IsSequenceListEmpty)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            // 直接判断 length 即可
            if (pList->length == 0) {
                return DSCxx_TRUE;
            } else {
                return DSCxx_FALSE;
            }
        }
    };

    SINGLETON_MEMBER(IsSequenceListEmpty)

    class SequenceListLength : public Function {
    ENABLE_SINGLETON(SequenceListLength)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(1)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            return pList->length;
        }
    };

    SINGLETON_MEMBER(SequenceListLength)

    class GetElemInSequenceList : public Function {
    ENABLE_SINGLETON(GetElemInSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            int i = stoi(args[1]);
            ElemType *pVar = (ElemType *) Interactor::instance()->getVariable(args[2]);
            if (i < 1 || i > pList->length) {
                return DSCxx_ERROR;
            }
            *pVar = *(pList->elem + i - 1);
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(GetElemInSequenceList)

    // 这里有一点小问题，理想中的 Locate 方法应该能够传入一个用户自定义的函数来改变判断的准则，
    // 但是目前实现稍微有些困难（可能得对 Interactor 做一个大较大的重构），所以暂且先默认为相等判断
    // TODO: 实现可传入用户自定义判断准则的 Locate 方法
    class LocateElemInSequenceList : public Function {
    ENABLE_SINGLETON(LocateElemInSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(2)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            ElemType *pVar = (ElemType *) Interactor::instance()->getVariable(args[1]);
            // 查找第一个值与 pVar 相等的元素的位置
            // 若找到，则返回该位置；否则返回 0
            int i = 1;
            auto p = pList->elem;
            while (i <= pList->length && *p != *pVar) {
                ++i;
                ++p;
            }
            return (i <= pList->length) ? i : 0;
        }
    };

    SINGLETON_MEMBER(LocateElemInSequenceList)

    class PriorElemInSequenceList : public Function {
    ENABLE_SINGLETON(PriorElemInSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            ElemType *pPre = (ElemType *) Interactor::instance()->getVariable(args[2]);
            // 用 pPre 返回 pCur 在线性表中的前驱
            // 先查找 pCur 的位置，然后再设定 pPre
            int location = LocateElemInSequenceList::instance()->invoke({args[0], args[1]});
            if (location <= 1) {
                return DSCxx_ERROR;
            }
            *pPre = pList->elem[location - 2]; // 索引比位序小 1，前一个元素还需减 1，所以需要减 2
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(PriorElemInSequenceList)

    class NextElemInSequenceList : public Function {
    ENABLE_SINGLETON(NextElemInSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            ElemType *pNext = (ElemType *) Interactor::instance()->getVariable(args[2]);
            // 用 pPre 返回 pCur 在线性表中的前驱
            // 先查找 pCur 的位置，然后再设定 pPre
            int location = LocateElemInSequenceList::instance()->invoke({args[0], args[1]});
            if (location <= 1) {
                return DSCxx_ERROR;
            }
            *pNext = pList->elem[location]; // 位序比索引大 1，后一个元素还需加 1，正好抵消
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(NextElemInSequenceList)

    class SequenceListInsert : public Function {
    ENABLE_SINGLETON(SequenceListInsert)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            int i = stoi(args[1]);
            ElemType *pVar = (ElemType *) Interactor::instance()->getVariable(args[2]);
            // 执行插入后，新插入的元素在新的线性表中的位置为 i，所以 i 最小为 1，最大可为 length + 1
            if (i < 1 || i > pList->length + 1) {
                return DSCxx_ERROR;
            }
            // 如果存储空间已满，则需要先增加分配，再插入元素
            if (pList->length >= pList->listsize) {
                auto newBase = (ElemType *) realloc(pList->elem,
                                                    (pList->listsize + LISTINCREMENT) * sizeof(ElemType));
                if (!newBase) exit(DSCxx_OVERFLOW);
                pList->elem = newBase;
                pList->listsize += LISTINCREMENT;
            }
            // 获取插入位置
            auto q = &(pList->elem[i - 1]); // 位置 i 对应的索引为 i - 1，所以需将 i - 1 及其后的元素全部后移一位
            for (auto p = &(pList->elem[pList->length - 1]); p >= q; --p) {
                *(p + 1) = *p;
            }
            *q = *pVar;
            ++pList->length;
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(SequenceListInsert)

    class SequenceListDelete : public Function {
    ENABLE_SINGLETON(SequenceListDelete)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(3)
            SequenceList *pList = (SequenceList *) Interactor::instance()->getADT(args[0]);
            if (pList->elem == nullptr) {
                return DSCxx_ERROR;
            }
            int i = stoi(args[1]);
            // 删除的元素会用 pVar 返回
            ElemType *pVar = (ElemType *) Interactor::instance()->getVariable(args[2]);
            if (i < 1 || i > pList->length) {
                return DSCxx_ERROR;
            }
            auto p = &(pList->elem[i - 1]);
            *pVar = *p;
            auto q = pList->elem + pList->length - 1;
            for (++p; p <= q; ++p) {
                *(p - 1) = *p;
            }
            --pList->length;
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(SequenceListDelete)

    // 因为暂不支持传入用户自定义函数，所以此方法为空实现
    // TODO: 在支持传入用户自定义函数后，需要实现 Traverse 方法
    class SequenceListTraverse : public Function {
    ENABLE_SINGLETON(SequenceListTraverse)

    public:
        Status invoke(const vector<string> &args) override {
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(SequenceListTraverse)

//    template<typename ElemType>
//    Status ListTraverse_Sq(SqList<ElemType>& L, std::function<Status(ElemType&)> visit)
//    {
//        // 对顺序线性表中每个数据元素调用 visit()
//        // 直接使用 for 循环遍历
//        for (int i = 0; i < L.length; ++i) {
//            if (visit(L.elem[i]) != OK) return ERROR;   // 一旦 visit() 失败，则操作失败
//        }
//        return OK;
//    }

    class UnionSequenceList : public Function {
    ENABLE_SINGLETON(UnionSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            CHECK_ARG_COUNT(2)
            SequenceList *pListTarget = (SequenceList *) Interactor::instance()->getADT(args[0]);
            SequenceList *pListSource = (SequenceList *) Interactor::instance()->getADT(args[1]);
            if (pListSource->elem == nullptr || pListTarget->elem == nullptr) {
                return DSCxx_ERROR;
            }
            // 将所有在线性表 Source 中但不在 Target 中的数据元素插入到 Target 中
            // 这里先创建一些临时变量用于调用符合 Function 规范的函数，最后记得要删除这些变量
            Interactor::instance()->createVariable("_e");
            Interactor::instance()->createVariable("_len");
            ElemType *_len = Interactor::instance()->getVariable("_len");

            // 此处本可以直接使用 length 成员，但出于演示目的使用了 Length 函数
            //*_len = pListTarget->length;
            //auto sourceLen = pListSource->length;
            *_len = SequenceListLength::instance()->invoke({args[0]});
            auto sourceLen = SequenceListLength::instance()->invoke({args[1]});

            for (int i = 1; i <= sourceLen; ++i) {
                GetElemInSequenceList::instance()->invoke({args[1], to_string(i), "_e"});
                // 如果 Target 中不存在和 _e 相同（相等）的元素，则将其插入到 Target 的尾部
                if (!LocateElemInSequenceList::instance()->invoke({args[0], "_e"})) {
                    SequenceListInsert::instance()->invoke({args[0], "_len", "_e"});
                }
            }

            // 最后记得删除创建的临时变量
            Interactor::instance()->deleteVariable("_e");
            Interactor::instance()->deleteVariable("_len");
            return DSCxx_OK;
        }
    };

    SINGLETON_MEMBER(UnionSequenceList)

    class MergeSequenceList : public Function {
    ENABLE_SINGLETON(MergeSequenceList)

    public:
        Status invoke(const vector<string> &args) override {
            SequenceList *pListSourceA = (SequenceList *) Interactor::instance()->getADT(args[0]);
            SequenceList *pListSourceB = (SequenceList *) Interactor::instance()->getADT(args[1]);
            SequenceList *pListTarget = (SequenceList *) Interactor::instance()->getADT(args[2]);
            // 已知线性表 SourceA 和 SourceB 中的数据元素按值非递减排列
            // 归并 SourceA 和 SourceB 得到新的线性表 Target，Target 的数据元素也按值非递减排列
            InitSequenceList::instance()->invoke({args[2]});

            // 创建临时变量
            Interactor::instance()->createVariable("_i");
            auto _i = Interactor::instance()->getVariable("_i");
            Interactor::instance()->createVariable("_j");
            auto _j = Interactor::instance()->getVariable("_j");
            Interactor::instance()->createVariable("_k");
            auto _k = Interactor::instance()->getVariable("_k");
            Interactor::instance()->createVariable("_ai");
            auto _ai = Interactor::instance()->getVariable("_ai");
            Interactor::instance()->createVariable("_bj");
            auto _bj = Interactor::instance()->getVariable("_bj");

            *_i = *_j = 1;
            *_k = 0;

            auto aLen = pListSourceA->length;
            auto bLen = pListSourceB->length;

            // 首先将 SourceA 和 SourceB 中较小的元素按顺序插入到 Target 中
            while ((*_i <= aLen) && (*_j <= bLen)) {
                GetElemInSequenceList::instance()->invoke({args[0], "_i", "_ai"});
                GetElemInSequenceList::instance()->invoke({args[1], "_j", "_bj"});
                ++*_k; // 递增在 Target 中插入位置的索引值
                if (*_ai <= *_bj) {
                    SequenceListInsert::instance()->invoke({args[2], "_k", "_ai"});
                    ++*_i; // 递增当前在 SourceA 中定位的索引值
                } else {
                    SequenceListInsert::instance()->invoke({args[2], "_k", "_bj"});
                    ++*_j; // 递增当前在 SourceB 中定位的索引值
                }
            }

            // 如果还有剩余元素，则也将其插入 Target 中
            while (*_i <= aLen) {
                ++*_i;
                ++*_k;
                GetElemInSequenceList::instance()->invoke({args[0], "_i", "_ai"});
                SequenceListInsert::instance()->invoke({args[2], "_k", "_ai"});
            }
            while (*_j <= bLen) {
                ++*_j;
                ++*_k;
                GetElemInSequenceList::instance()->invoke({args[1], "_j", "_bj"});
                SequenceListInsert::instance()->invoke({args[2], "_k", "_bj"});
            }

            // 删除临时变量
            Interactor::instance()->deleteVariable("_i");
            Interactor::instance()->deleteVariable("_j");
            Interactor::instance()->deleteVariable("_k");
            return DSCxx_OK;
        }
    };
    SINGLETON_MEMBER(MergeSequenceList)

}