#ifndef CALDIFFINFO_H
#define CALDIFFINFO_H

#include"diff/qgitdiff.h"
#include<set>
#include<string>

class CalDiffInfo
{
public:
    CalDiffInfo(Diff d1,Diff d2);




private:
    void InitData();

    std::set<std::string> m_commonFile; //dif1 和 dif2 共有的文件

    std::set<std::string> m_dif2File;  //仅在dif2 种的 文件
    std::set<std::string> m_dif1File;  //仅在dif1 种的 文件
    Diff m_dif1;
    Diff m_dif2;
};

#endif // CALDIFFINFO_H
