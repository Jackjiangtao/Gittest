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

#ifndef LIBQGIT2_DIFF_H
#define LIBQGIT2_DIFF_H


#include<QSharedPointer>
#include<QString>
#include <git2.h>


#include <map>

struct cmp{
    bool operator() (const git_diff_hunk& a,const git_diff_hunk& b)
    {
        return a.new_lines>b.new_lines;
    }
};





class DiffDelta;

/**
 * This class represents a diff.
 */
class  Diff
{
public:
    Diff(git_diff *diff = 0);

    size_t numDeltas() const;

    DiffDelta delta(size_t index) const;

    ~Diff();
public:

    static std::map<const git_diff_delta *,std::vector<git_diff_binary> > m_binary;

    static std::map<const git_diff_delta *, std::vector< git_diff_hunk > > m_hunk;

    static std::map<git_diff_hunk ,std::vector< git_diff_line >,cmp > m_line;

    QSharedPointer<git_diff> d;

private:
   void DiffInfo();

};



#endif // LIBQGIT2_DIFF_H
