/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_cGIFunctionInfo
#define SELF(self) RVAL2GI_FUNCTION_INFO(self)

GType
gi_function_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIFunctionInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_symbol(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_function_info_get_symbol(info));
}

static VALUE
rg_flags(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_FUNCTION_INFO_FLAGS2RVAL(g_function_info_get_flags(info));
}

static VALUE
rg_property(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_function_info_get_property(info));
}

static VALUE
rg_vfunc(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_function_info_get_vfunc(info));
}

static void
fill_in_argument(GIArgInfo *arg_info, GArray *in_args, int argc, VALUE *argv)
{
    VALUE rb_argument;
    GIArgument argument;

    /* TODO: check argc */
    rb_argument = argv[in_args->len];
    RVAL2GI_CALL_ARGUMENT(&argument, arg_info, rb_argument);
    g_array_append_val(in_args, argument);
}

static void
fill_out_argument(GIArgInfo *arg_info, GArray *out_args)
{
    GIArgument argument;

    memset(&argument, 0, sizeof(argument));
    if (g_arg_info_is_caller_allocates(arg_info)) {
        argument.v_pointer = xmalloc(sizeof(gpointer));
    }
    g_array_append_val(out_args, argument);
}

static void
fill_argument(GIArgInfo *arg_info, GArray *in_args, GArray *out_args,
              int argc, VALUE *argv)
{
    switch (g_arg_info_get_direction(arg_info)) {
      case GI_DIRECTION_IN:
        fill_in_argument(arg_info, in_args, argc, argv);
        break;
      case GI_DIRECTION_OUT:
        fill_out_argument(arg_info, out_args);
        break;
      case GI_DIRECTION_INOUT:
        fill_in_argument(arg_info, in_args, argc, argv);
        fill_out_argument(arg_info, out_args);
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
retrieve_out_argument(GIArgInfo *arg_info, GIArgument *out_arg,
                      VALUE rb_out_args)
{
    GITypeInfo type_info;

    g_arg_info_load_type(arg_info, &type_info);
    rb_ary_push(rb_out_args, GI_ARGUMENT2RVAL(out_arg, &type_info));
    if (g_arg_info_is_caller_allocates(arg_info)) {
        xfree(out_arg->v_pointer);
    }
}

static VALUE
retrieve_out_arguments(GICallableInfo *callable_info, GArray *out_args)
{
    gint i, n_args;
    gint out_arg_index = 0;
    VALUE rb_out_args;

    if (out_args->len == 0) {
        return Qnil;
    }

    rb_out_args = rb_ary_new();
    n_args = g_callable_info_get_n_args(callable_info);
    for (i = 0; i < n_args; i++) {
        GIArgInfo arg_info;
        GIArgument *out_arg;
        g_callable_info_load_arg(callable_info, i, &arg_info);
        if (g_arg_info_get_direction(&arg_info) == GI_DIRECTION_IN) {
            continue;
            }
        out_arg = &g_array_index(out_args, GIArgument, out_arg_index);
        retrieve_out_argument(&arg_info, out_arg, rb_out_args);
        out_arg_index++;
    }

    return rb_out_args;
}

VALUE
rb_gi_function_info_invoke_raw(GIFunctionInfo *info, int argc, VALUE *argv,
                               GArray *in_args, GArray *out_args,
                               GIArgument *return_value)
{
    GICallableInfo *callable_info;
    gint i, n_args;
    VALUE rb_out_args;
    gboolean succeeded;
    GError *error = NULL;

    callable_info = (GICallableInfo *)info;
    n_args = g_callable_info_get_n_args(callable_info);
    for (i = 0; i < n_args; i++) {
        GIArgInfo arg_info;
        g_callable_info_load_arg(callable_info, i, &arg_info);
        fill_argument(&arg_info, in_args, out_args, argc, argv);
    }
    succeeded = g_function_info_invoke(info,
                                       (GIArgument *)(in_args->data),
                                       in_args->len,
                                       (GIArgument *)(out_args->data),
                                       out_args->len,
                                       return_value,
                                       &error);
    rb_out_args = retrieve_out_arguments(callable_info, out_args);
    if (!succeeded) {
        RG_RAISE_ERROR(error);
    }

    return rb_out_args;
}

static VALUE
rg_invoke(int argc, VALUE *argv, VALUE self)
{
    GIFunctionInfo *info;
    GICallableInfo *callable_info;
    GArray *in_args, *out_args;
    GIArgument return_value;
    VALUE rb_out_args;
    VALUE rb_return_value;

    info = SELF(self);
    in_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    out_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    /* TODO: use rb_protect() */
    rb_out_args = rb_gi_function_info_invoke_raw(info, argc, argv,
                                                 in_args, out_args,
                                                 &return_value);
    g_array_unref(in_args);
    g_array_unref(out_args);

    callable_info = (GICallableInfo *)info;
    rb_return_value = GI_RETURN_ARGUMENT2RVAL(&return_value, callable_info);

    if (NIL_P(rb_out_args)) {
        return rb_return_value;
    } else {
        return rb_ary_new3(2, rb_return_value, rb_out_args);
    }
}

void
rb_gi_function_info_init(VALUE rb_mGI, VALUE rb_cGICallableInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_FUNCTION_INFO, "FunctionInfo", rb_mGI,
				rb_cGICallableInfo);

    RG_DEF_METHOD(symbol, 0);
    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(property, 0);
    RG_DEF_METHOD(vfunc, 0);
    RG_DEF_METHOD(invoke, -1);

    G_DEF_CLASS(G_TYPE_I_FUNCTION_INFO_FLAGS, "FunctionInfoFlags", rb_mGI);

    rb_gi_method_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_constructor_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
