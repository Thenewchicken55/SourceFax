/*
    refillContent.hpp

    Include file for the refillContent function
*/

#ifndef INCLUDED_REFILLCONTENT_HPP
#define INCLUDED_REFILLCONTENT_HPP

#include<string_view>

/*
    Refill the content preserving the existing data.

    @param[in, out] content View of the content
    @return Number of bytes read
    @retval 0 EOF
    @retval -1 Read error
*/
[[nodiscard]] int refillContent(std::string_view& content);

#endif