/******************************************************************************
 * This file is part of the libqgit2 library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "qgitdiff.h"
#include "qgitdiffdelta.h"
#include<iostream>
#include<map>
#include<vector>
using namespace std;



std::map<const git_diff_delta *,std::vector<git_diff_binary >> Diff::m_binary;

std::map<const git_diff_delta *,std::vector< git_diff_hunk >> Diff::m_hunk;

std::map<git_diff_hunk ,std::vector< git_diff_line > ,cmp> Diff::m_line;



int file_cb(const git_diff_delta *delta, float progress, void *payload)
{
    return 0;
}

int binary_cb(const git_diff_delta *delta, const git_diff_binary *binary, void *payload)
{
    auto &value =Diff::m_binary[delta];
    value.push_back(*binary);
    return 0;
}

int hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
{

    auto &value =Diff::m_hunk[delta];
    value.push_back(*hunk);

    return 0;
}

int line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
{

    auto &value =Diff::m_line[*hunk];
    value.push_back(*line);
    return 0;
}

int notify_cb(const git_diff *diff_so_far, const git_diff_delta *delta_to_add, const char *matched_pathspec, void *payload)
{
    return 0;
}

int progress_cb(const git_diff *diff_so_far, const char *old_path, const char *new_path, void *payload)
{

    return 0;
}





Diff::Diff(git_diff *diff) :
    d(diff, git_diff_free)
{
    DiffInfo();
}

size_t Diff::numDeltas() const
{
    size_t ret = 0;
    if (!d.isNull()) {
        ret = git_diff_num_deltas(d.data());
    }
    return ret;
}

DiffDelta Diff::delta(size_t index) const
{
    const git_diff_delta *delta = 0;
    if (!d.isNull()) {
        delta = git_diff_get_delta(d.data(), index);
    }
    return DiffDelta(delta);
}

Diff::~Diff()
{

}

void Diff::DiffInfo()
{
    if(!d.data())
        return ;
    int error = 0;

    git_diff_options diff_opt = GIT_DIFF_OPTIONS_INIT;
    git_diff_find_options diff_find_opt = GIT_DIFF_FIND_OPTIONS_INIT;

    // diff copy or rename
    diff_find_opt.flags |= GIT_DIFF_FIND_RENAMES;
    diff_find_opt.flags |= GIT_DIFF_FIND_COPIES;
    diff_find_opt.flags |= GIT_DIFF_FIND_FOR_UNTRACKED;

    error = git_diff_find_similar(d.data(), &diff_find_opt);

    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return ;
    }
    error = git_diff_foreach(d.data(), file_cb, binary_cb, hunk_cb, line_cb, nullptr);
    if (error < 0)
    {
        const git_error *e = giterr_last();
        std::cout << "Error: " << error << " / " << e->klass << " : " << e->message << std::endl;
        return ;
    }

}


