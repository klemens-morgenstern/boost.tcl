//
// Copyright (c) 2022 Klemens Morgenstern (klemens.morgenstern@gmx.net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/tcl/interpreter.hpp>
#include <boost/tcl/object.hpp>
#include <string>
#include <stdexcept>

#include "doctest.h"
#include "boost/tcl/exception.hpp"

TEST_SUITE_BEGIN("error");

static int
throw_cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
try {
    throw std::runtime_error("test-error");
}
catch (...)
{
    auto obj = boost::tcl::make_exception_object();
    Tcl_SetObjResult(interp, obj.get());
    return TCL_ERROR;
}

TEST_CASE("exception")
{
    {
        auto ip = boost::tcl::make_interpreter();
        Tcl_FindExecutable(doctest::getContextOptions()->binary_name.c_str());
        CHECK(Tcl_Init(ip.get()) == TCL_OK);
        Tcl_CreateObjCommand(ip.get(), "throw_test", throw_cmd, nullptr, nullptr);

        CHECK(Tcl_Eval(ip.get(), R"(throw_test)") == TCL_ERROR);
        CHECK_THROWS_WITH(boost::tcl::throw_result(ip.get()), "test-error");

    }
    Tcl_Finalize();
}

TEST_SUITE_END();