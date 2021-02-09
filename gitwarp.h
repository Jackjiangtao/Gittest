#ifndef GITWARP_H
#define GITWARP_H

#include<git2.h>

#include<iostream>
#include <QString>
#include"diff/qgitdiff.h"
#include"diff/qgitdiffdelta.h"
#include"diff/qgitdifffile.h"

#include<set>


class Diff;

class GitWarp
{
public:
    GitWarp(QString url,QString origin="origin",QString branch="master");

    const git_error *diffBranch(QString oldBranch, QString newBranch,Diff &temp);    //分支比较

    const git_error * cloneBranch(QString branch1="",bool bBare=false);  //下载指定分支

    const git_error *checkOutFile(std::vector<std::string> file);


    ~GitWarp();

    QString GetPath()
    {
        return m_path;
    }
private:
    void InitRepository();

private:
    QString m_branch;
    std::set<QString> m_tmpPath;
    const git_error* m_error;
    QString m_path;
    QString m_origin;
    QString m_url;
    git_repository * m_pRepository;
};

#endif // GITWARP_H
