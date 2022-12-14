//
// Copyright (c) 2022 Klemens Morgenstern (klemens.morgenstern@gmx.net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TCL_BUILTIN_STRING_HPP
#define BOOST_TCL_BUILTIN_STRING_HPP

#include <boost/tcl/cast.hpp>
#include <boost/utility/string_view.hpp>

#include <string>

namespace boost::tcl
{

inline std::optional<string_view> tag_invoke(
        cast_tag<string_view>,
        Tcl_Interp *,
        boost::intrusive_ptr<Tcl_Obj> val)
{
    int sz;
    char * c = Tcl_GetStringFromObj(val.get(), &sz);
    return string_view(c, sz);
}

inline object_ptr tag_invoke(const struct convert_tag &, Tcl_Interp*, string_view sv)
{
    return Tcl_NewStringObj(sv.data(), sv.size());
}


inline bool tag_invoke(
        const equal_type_tag<string_view> & tag,
        const Tcl_ObjType & type)
{
    return type.name && type.name == string_view("string");
}

template<typename Traits, typename Allocator>
inline bool tag_invoke(
        const equal_type_tag<std::basic_string<char, Traits, Allocator>> & tag,
        const Tcl_ObjType & type)
{
    return type.name && type.name == string_view("string");
}

template<typename Traits>
inline bool tag_invoke(
        const equal_type_tag<basic_string_view<char, Traits>> & tag,
        const Tcl_ObjType & type)
{
    return type.name && type.name == string_view("string");
}

template<typename StringLike>
inline bool tag_invoke(
        const equivalent_type_tag<StringLike> & tag,
        const Tcl_ObjType & type,
        std::enable_if_t<
                std::is_convertible_v<StringLike, string_view> ||
                std::is_constructible_v<StringLike, const char*, std::size_t>, StringLike> * = nullptr)
{
    return type.name &&
           type.name == string_view("string");
}

template<typename StringLike>
inline std::optional<StringLike> tag_invoke(
        cast_tag<StringLike>,
        Tcl_Interp *,
        boost::intrusive_ptr<Tcl_Obj> val,
        std::enable_if_t<
            std::is_convertible_v<StringLike, string_view> ||
            std::is_constructible_v<StringLike, const char*, std::size_t>, StringLike> * = nullptr)
{
    std::optional<StringLike> res;
    int sz;
    char * c = Tcl_GetStringFromObj(val.get(), &sz);

    res.emplace(c, sz);
    return std::move(res);
}

}

#endif //BOOST_TCL_BUILTIN_STRING_HPP
