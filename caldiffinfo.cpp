#include "caldiffinfo.h"
#include"diff/qgitdiffdelta.h"
#include"diff/qgitdifffile.h"

CalDiffInfo::CalDiffInfo(Diff d1,Diff d2):m_dif1(d1),m_dif2(d2)
{
    InitData();
}

void CalDiffInfo::InitData()
{
    if(m_dif1.numDeltas()==0 || m_dif2.numDeltas()==0)
    {
        return ;
    }

    size_t num1 = m_dif1.numDeltas();
    std::set<std::string> path1;
    for(auto i =0;i<num1;++i)
    {
        auto diffData =m_dif1.delta(i);
        auto delta =diffData.Getdiffdelta();

        if(delta)
        {
            auto newFile =delta->old_file.path;

            auto oldFile =ptr->oldFile();

            path1.insert(newFile);
        }

    }

    size_t num2 = m_dif2.numDeltas();
    std::set<std::string> path2;
    for(auto i =0;i<num2;++i)
    {
        auto diffData =m_dif2.delta(i);
        auto delta =diffData.Getdiffdelta();

        if(delta)
        {
            auto newFile =delta->old_file.path;

            auto oldFile =ptr->oldFile();

            path2.insert(newFile);
        }
    }

    for(auto it =pa)

}
