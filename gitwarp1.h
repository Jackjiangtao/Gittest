#ifndef GITWARP_H
#define GITWARP_H

#include<git2.h>

#include<iostream>
#include <QString>


//const char* path = "G:\\gittest\\dsdsd\\";

//int file_cb(const git_diff_delta *delta, float progress, void *payload)
//{
//    std::cout<< "old file path: " << delta->old_file.path <<"\n";
//    std::cout<< "new file path: " << delta->new_file.path <<"\n";
//    std::cout<< "file number: " << delta->nfiles <<"\n";
//    std::cout<< "status: " << delta->status <<"\n";
//    std::cout<< "flags: " << delta->flags <<"\n";
//    std::cout<< "progress: " << progress << "\n";
//    std::cout<< "==================" << std::endl;

//    return 0;
//}

//int binary_cb(const git_diff_delta *delta, const git_diff_binary *binary, void *payload)
//{
//    std::cout<< "old file path: " << delta->old_file.path <<"\n";
//    std::cout<< "new file path: " << delta->old_file.path <<"\n";
//    std::cout<< "=================" << std::endl;

//    return 0;
//}

//int hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
//{
//    std::cout<< "old start: " << hunk->old_start <<"\n";
//    std::cout<< "old lines: " << hunk->old_lines <<"\n";
//    std::cout<< "new start: " << hunk->new_start <<"\n";
//    std::cout<< "new lines: " << hunk->new_lines <<"\n";
//    std::cout<< "header: " << hunk->header <<"\n";
//    std::cout<< "=================" << std::endl;

//    return 0;
//}

//int line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
//{
//    std::cout<< "origin: " << line->origin <<"\n";
//    std::cout<< "old line number: " << line->old_lineno <<"\n";
//    std::cout<< "new line number: " << line->new_lineno <<"\n";
//    std::cout<< "num lines: " << line->num_lines <<"\n";
//    std::cout<< "content offset: " << line->content_offset <<"\n";
//    std::cout<< "=================" << std::endl;

//    return 0;
//}

//int notify_cb(const git_diff *diff_so_far, const git_diff_delta *delta_to_add, const char *matched_pathspec, void *payload)
//{
//    std::cout<< "***notify cb***" <<"\n";
//    std::cout<< "old file: " << delta_to_add->old_file.path <<"\n";
//    std::cout<< "new file: " << delta_to_add->new_file.path <<"\n";
//    std::cout<< "matched path spec: " << (matched_pathspec != nullptr ? matched_pathspec : "null") <<"\n";
//    std::cout<< "*****************" << std::endl;

//    return 0;
//}

//int progress_cb(const git_diff *diff_so_far, const char *old_path, const char *new_path, void *payload)
//{
//    std::cout<< "****pro cb****" <<"\n";
//    std::cout<< "old path: " << old_path <<"\n";
//    std::cout<< "new path: " << new_path <<"\n";
//    std::cout<< "*****************" << std::endl;

//    return 0;
//}



//void test_diff_index__1(void)
//{
//    git_libgit2_init();

//    git_repository* rep = nullptr;
//    git_reference* head = nullptr;
//    git_reference* new_branch = nullptr;
//    const git_oid* commit_id = nullptr;
//    git_commit* last_commit = nullptr;

//    git_branch_iterator* branch_iterator = nullptr;
//    git_reference* tmp_branch = nullptr;
//    git_branch_t branch_type;

//    git_reference* lookup_branch = nullptr;

//    git_reference* renamed_branch = nullptr;

//    int error = 0;



//    // git open
//    error = git_repository_open(&rep, path);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
//    }

//    git_object *branch1 = NULL;
//    error = git_revparse_single(&branch1, rep, "master^{tree}");

//    git_tree *tree1 =reinterpret_cast<git_tree *>(branch1);

//    git_object *branch2 = NULL;
//    error = git_revparse_single(&branch2, rep, "new^{tree}");

//    git_tree *tree2 =reinterpret_cast<git_tree *>(branch2);

//    git_diff *diff = NULL;

//    git_diff_options diff_opt = GIT_DIFF_OPTIONS_INIT;
//    git_diff_find_options diff_find_opt = GIT_DIFF_FIND_OPTIONS_INIT;

//    git_diff_tree_to_tree(&diff, rep, tree1, tree2, NULL);


//    // diff copy or rename
//    diff_find_opt.flags |= GIT_DIFF_FIND_RENAMES;
//    diff_find_opt.flags |= GIT_DIFF_FIND_COPIES;
//    diff_find_opt.flags |= GIT_DIFF_FIND_FOR_UNTRACKED;

//    error = git_diff_find_similar(diff, &diff_find_opt);

//    error = git_diff_foreach(diff, file_cb, binary_cb, hunk_cb, line_cb, nullptr);

//    // delete branch
//    //    git_branch_lookup(&lookup_branch, rep, "new", GIT_BRANCH_LOCAL);
//    //    if (lookup_branch != nullptr && !git_branch_is_head(lookup_branch))
//    //    {
//    //        git_branch_delete(lookup_branch);
//    //    }

//    // rename branch
//    //    git_branch_lookup(&lookup_branch, rep, "new", GIT_BRANCH_LOCAL);
//    //    git_branch_move(&renamed_branch, lookup_branch, "new3", 0);

//    // checkout branch
//    //    git_branch_lookup(&lookup_branch, rep, "new", GIT_BRANCH_LOCAL);
//    //    git_repository_set_head(rep, git_reference_name(lookup_branch));

//    git_reference_free(new_branch);
//    git_reference_free(head);
//    git_commit_free(last_commit);

//SHUTDOWN:
//    git_repository_free(rep);
//    git_libgit2_shutdown();
//}

//void test_clone()
//{
//    git_libgit2_init();

//    git_repository* rep = nullptr;
//    git_clone_options opt = GIT_CLONE_OPTIONS_INIT;

//    int error = 0;


//    //opt.checkout_branch="33333";

//    error = git_clone(&rep, "https://github.com/Jackjiangtao/videoMerge.git", path, &opt);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;

//        goto SHUTDOWN;
//    }


//SHUTDOWN:
//    git_repository_free(rep);
//    git_libgit2_shutdown();
//}

class Diff;


class GitWarp
{
public:
    GitWarp(QString path="");

    Diff diffBranch(QString branch1, QString branch2);    //分支比较

    bool clone(QString url,QString path,QString branch1="",bool bBare=false);


    bool cloneFile();
    ~GitWarp();
private:
    void InitRepository(QString path);
    QString m_Path;
    git_repository * m_pRepository;
};

#endif // GITWARP_H
