#pragma once

#include <algorithm>
#include <iterator>

namespace golv
{

/**
 *   find the longest subsequence of equal entries.
 */
template<class InIt>
std::pair<InIt, InIt> longest_subsequence(InIt _beg, InIt _end)
{
    auto count = 0;
    auto beg = _beg, end = beg;
    std::pair<InIt, InIt> res{beg, end};
    while(_end != end) {
        end = std::find_if(
            beg + 1, _end, [&beg](auto val) { return val != *beg; });
        if(std::distance(beg, end) > count) {
            res   = {beg, end};
            count = std::distance(beg, end);
        }
        beg = end;
    }
    return res;
}

}   // namespace golv