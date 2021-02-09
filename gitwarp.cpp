#include "gitwarp.h"

#include"diff/qgitdiff.h"
#include <QDir>
#include"git2.h"
#include<QUuid>
#include <QCoreApplication>


GitWarp::GitWarp(QString url,QString origin /*origin*/,QString branch /* master*/) :
    m_url(url),m_origin(origin),m_pRepository(nullptr),m_error(nullptr),m_branch(branch)
{
    m_path =QDir::tempPath();
    m_path+="/"+QUuid::createUuid().toString();
    m_tmpPath.insert(m_path);

    InitRepository();
}




const git_error* GitWarp::diffBranch(QString oldBranch, QString newBranch,Diff &temp)
{
    if(m_error)
    {
        return m_error;
    }

    try
    {

        if(!oldBranch.startsWith(m_origin))
        {
            oldBranch =m_origin+"/"+oldBranch;
        }

        if(!newBranch.startsWith(m_origin))
        {
            newBranch =m_origin+"/"+newBranch;
        }
        int error = 0;

        QString tmpName ="^{tree}";

        oldBranch +=tmpName;
        newBranch +=tmpName;

        git_object *branch1 = NULL;
        error = git_revparse_single(&branch1,m_pRepository , oldBranch.toStdString().c_str());
        if (error < 0)
        {
            const git_error *e = giterr_last();
            std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
            temp =Diff(nullptr);
            return e;
        }

        git_tree *tree1 =reinterpret_cast<git_tree *>(branch1);

        git_object *branch2 = NULL;
        error = git_revparse_single(&branch2, m_pRepository,newBranch.toStdString().c_str());

        if (error < 0)
        {
            const git_error *e = giterr_last();
            std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
            temp =Diff(nullptr);
            return e;
        }

        git_tree *tree2 =reinterpret_cast<git_tree *>(branch2);

        git_diff *diff = NULL;

        git_diff_options diff_opt = GIT_DIFF_OPTIONS_INIT;
        git_diff_find_options diff_find_opt = GIT_DIFF_FIND_OPTIONS_INIT;

        error = git_diff_tree_to_tree(&diff, m_pRepository, tree1, tree2, NULL);
        if (error < 0)
        {
            const git_error *e = giterr_last();
            std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
            temp =Diff(nullptr);
            return e;
        }

        temp =Diff(diff) ;

        return nullptr;
    }

    catch (...)
    {
        temp =Diff(nullptr);
        return nullptr;
    }
}


const git_error * GitWarp::cloneBranch(QString branch1/*=""*/,bool bBare /*=false*/)
{

    git_repository *repo = 0;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
    if(!branch1.isEmpty())
    {
        opts.checkout_branch=branch1.toStdString().c_str();
    }

    auto path =QDir::tempPath();
    path+="/"+QUuid::createUuid().toString();

    m_tmpPath.insert(path);
    if(bBare)
    {
        opts.bare=1;
        if(!path.endsWith("/"))
            path+="/.git";
        else {
            path+=".git";
        }
        QDir dir(path);
        if(!dir.exists())
        {
            if(dir.mkdir(path))
            {
                std::cout << "make dir error "<< std::endl;
                return nullptr;
            }
        }
    }

    opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    auto error=git_clone(&repo, m_url.toStdString().c_str(), path.toStdString().c_str(), &opts);

    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return e;

    }

    return nullptr;
}





const git_error *GitWarp::checkOutFile(std::vector<std::string> file)
{
    if(m_error)
    {
        return m_error;
    }

    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;

    opts.checkout_strategy = GIT_CHECKOUT_SAFE |
            GIT_CHECKOUT_RECREATE_MISSING;


    opts.notify_flags = GIT_CHECKOUT_NOTIFY_ALL;

    char **paths=nullptr ;
    paths=new char*[file.size()];

    for (auto i =0;i<file.size();++i)
    {
        auto str =file[i];

        char *tmp=new char[str.size()+1];
        strcpy(tmp,str.c_str());
        paths[i] =tmp;
    }


    opts.paths.strings = paths;
    opts.paths.count = file.size();
    int error = git_checkout_head(m_pRepository, &opts);

    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        for (int i=0; i<file.size(); i++)
        {
            delete[] paths[i];
            paths[i]=nullptr;
        }

        delete[] paths;
        paths=nullptr;
        return e;
    }

    for (int i=0; i<file.size(); i++)
    {
        delete[] paths[i];
        paths[i]=nullptr;
    }

    delete[] paths;
    paths=nullptr;
    return nullptr;

}

GitWarp::~GitWarp()
{
    if(m_pRepository)
    {
        git_repository_free(m_pRepository);
        m_pRepository = NULL;
        git_libgit2_shutdown();
    }

    for(auto it =m_tmpPath.begin();it!=m_tmpPath.end();++it)
    {
       QDir dir(*it);

       if(dir.removeRecursively())
       {
           dir.rmdir(*it);
       }
    }
}

void GitWarp::InitRepository()
{
    git_libgit2_init();

    git_remote* remote = nullptr;


    git_reference* origin_master = nullptr;


    git_remote_callbacks remote_conn_opt = GIT_REMOTE_CALLBACKS_INIT;

    // git init
    int error = git_repository_init(&m_pRepository, m_path.toStdString().c_str(), 0);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }

    // git open
    error = git_repository_open(&m_pRepository, m_path.toStdString().c_str());
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }

    error = git_remote_create(&remote, m_pRepository, m_origin.toStdString().c_str(), m_url.toStdString().c_str());
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }

    error = git_remote_connect(remote, GIT_DIRECTION_FETCH, &remote_conn_opt, nullptr, nullptr);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }


    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;


    error =git_remote_fetch(remote, NULL, &fetch_opts, NULL);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }

    QString remoteBranch =m_origin+"/"+m_branch;
    error = git_branch_lookup(&origin_master, m_pRepository, remoteBranch.toStdString().c_str(), GIT_BRANCH_REMOTE);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }

    error = git_repository_set_head(m_pRepository, git_reference_name(origin_master));
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        m_error =e;
        return ;
    }
}
