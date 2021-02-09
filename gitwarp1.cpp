#include "gitwarp.h"

#include"diff/qgitdiff.h"
#include <QDir>
#include<QUuid>


struct log_state {
    git_repository *repo;
    const char *repodir;
    git_revwalk *walker;
    int hide;
    int sorting;
    int revisions;
};


struct log_options {
    int show_diff;
    int show_log_size;
    int skip, limit;
    int min_parents, max_parents;
    git_time_t before;
    git_time_t after;
    const char *author;
    const char *committer;
    const char *grep;
};


GitWarp::GitWarp(QString path) :m_Path(path),m_pRepository(nullptr)
{

}




Diff GitWarp::diffBranch(QString Name1, QString Name2)
{
    int error = 0;

    QString tmpName ="^{tree}";

    Name1 +=tmpName;
    Name2 +=tmpName;

    git_object *branch1 = NULL;
    error = git_revparse_single(&branch1,m_pRepository , Name1.toStdString().c_str());
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return Diff();
    }

    git_tree *tree1 =reinterpret_cast<git_tree *>(branch1);

    git_object *branch2 = NULL;
    error = git_revparse_single(&branch2, m_pRepository,Name2.toStdString().c_str());

    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return Diff();
    }

    git_tree *tree2 =reinterpret_cast<git_tree *>(branch2);

    git_diff *diff = NULL;

    git_diff_options diff_opt = GIT_DIFF_OPTIONS_INIT;
    git_diff_find_options diff_find_opt = GIT_DIFF_FIND_OPTIONS_INIT;

    git_diff_tree_to_tree(&diff, m_pRepository, tree1, tree2, NULL);


    return Diff(diff);
}


bool GitWarp::clone(QString url,QString path,QString branch1/*=""*/,bool bBare /*=false*/)
{
    git_libgit2_init();
    git_repository *repo = 0;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
    if(!branch1.isEmpty())
    {
        opts.checkout_branch=branch1.toStdString().c_str();
    }

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
            dir.mkdir(path);
        }
    }

    opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    auto error=git_clone(&repo, url.toStdString().c_str(), path.toStdString().c_str(), &opts);

    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return false;

    }
    m_pRepository =repo;
    return true;
}



bool GitWarp::cloneFile()
{
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;

    opts.checkout_strategy = GIT_CHECKOUT_SAFE |
            GIT_CHECKOUT_RECREATE_MISSING;
    opts.target_directory = "grtrtrtfgf";

    char *path[]={"README.md"};

    opts.paths.strings =path;
    opts.paths.count =1;
    opts.notify_flags = GIT_CHECKOUT_NOTIFY_ALL;


    auto error= git_checkout_index(m_pRepository, NULL, &opts);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return false;

    }
}





GitWarp::~GitWarp()
{
    git_repository_free(m_pRepository);
    git_libgit2_shutdown();
}

void GitWarp::InitRepository(QString path)
{

    git_libgit2_init();

    git_repository* rep = nullptr;

    auto error = git_repository_open(&rep, path.toStdString().c_str());
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return ;
    }

    if (m_pRepository)
    {
        delete m_pRepository;
        m_pRepository =nullptr;
    }

    m_pRepository =rep;
}
