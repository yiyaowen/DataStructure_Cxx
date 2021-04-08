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
#ifdef BuildTest
#include "Test/Test.hpp"
#else
#include "ADTLoader.hpp"
#endif

using namespace DataStructure_Cxx;

int main() {
#ifdef BuildTest
    Interactor::instance()->addInstruction("MyAdd", MyAdd::instance());
#else
    loadAllAdts();
#endif
    Interactor::instance()->run();
}