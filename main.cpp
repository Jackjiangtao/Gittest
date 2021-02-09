#include <QCoreApplication>


#include"gitwarp.h"


#include<QDir>
#include<QUuid>
int main()
{
#if 0
    const char* path = "G:\\gittest\\1\\";
    GitWarp test(path);

    auto diff =test.diffBranch("new","master");


    size_t numD = diff.numDeltas();

    for(auto i =0;i<numD;++i)
    {
        auto diffData =diff.delta(i);

        auto delta =diffData.Getdiffdelta();
        auto diffHunk =Diff::m_hunk[delta];

        std::cout<< "old file path: " << delta->old_file.path <<"\n";
        std::cout<< "new file path: " << delta->new_file.path <<"\n";
        std::cout<< "file number: " << delta->nfiles <<"\n";
        std::cout<< "status: " << delta->status <<"\n";
        std::cout<< "flags: " << delta->flags <<"\n";
        std::cout<< "==================" << std::endl;


        for(auto iH =diffHunk.begin();iH!=diffHunk.end();++iH)
        {
            auto hunk =*iH;

            std::cout<< "old start: " << hunk.old_start <<"\n";
            std::cout<< "old lines: " << hunk.old_lines <<"\n";
            std::cout<< "new start: " << hunk.new_start <<"\n";
            std::cout<< "new lines: " << hunk.new_lines <<"\n";
            std::cout<< "header: " << hunk.header <<"\n";
            std::cout<< "=================" << std::endl;

            auto diffLine =Diff::m_line[hunk];
            for(auto iL =diffLine.begin();iL!=diffLine.end();++iL)
            {
                auto line =*iL;
                std::cout<< "origin: " << line.origin <<"\n";
                std::cout<< "old line number: " << line.old_lineno <<"\n";
                std::cout<< "new line number: " << line.new_lineno <<"\n";
                std::cout<< "num lines: " << line.num_lines <<"\n";
                std::cout<< "content offset: " << line.content_offset <<"\n";
                std::cout<< "=================" << std::endl;
            }
        }
    }

#else

    //QString tmpPath("G:/gittest/222222");

    {
        GitWarp test("http://xingjiangtao@192.168.10.15:9006/r/~xingjiangtao/lanyuan.git","origin");

        //auto error =test.cloneBranch("origin/Branch_GitBranch3");

        std::vector<std::string> tmp;
        QString rootPath ="src\\main\\java\\";
        tmp.push_back("src/main/java/org/apache/ibatis/datasource/pooled/PooledConnection.java");
                   //"src\main\java\org\apache\ibatis\datasource\pooled\PooledConnection.java"
        auto error =test.checkOutFile(tmp);
    }

#endif

    return 0;
}
