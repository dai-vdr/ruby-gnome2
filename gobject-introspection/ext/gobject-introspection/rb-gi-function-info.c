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
#define SELF(self) RVAL2GI_BASE_INFO(self)

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
fill_gi_argument_by_ruby(GIArgument *argument, GIArgInfo *arg_info,
                         VALUE rb_argument)
{
    GITypeInfo type_info;

    g_arg_info_load_type(arg_info, &type_info);
    switch (g_type_info_get_tag(&type_info)) {
      case GI_TYPE_TAG_VOID:
        break;
      case GI_TYPE_TAG_BOOLEAN:
        argument->v_boolean = RVAL2CBOOL(rb_argument);
        break;
      case GI_TYPE_TAG_INT8:
        argument->v_int8 = NUM2INT(rb_argument);
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_uint8 = NUM2UINT(rb_argument);
        break;
      case GI_TYPE_TAG_INT16:
        argument->v_int16 = NUM2INT(rb_argument);
        break;
      case GI_TYPE_TAG_UINT16:
        argument->v_uint16 = NUM2UINT(rb_argument);
        break;
      case GI_TYPE_TAG_INT32:
        argument->v_int32 = NUM2INT(rb_argument);
        break;
      case GI_TYPE_TAG_UINT32:
        argument->v_uint32 = NUM2UINT(rb_argument);
        break;
      case GI_TYPE_TAG_INT64:
        argument->v_int64 = NUM2LONG(rb_argument);
        break;
      case GI_TYPE_TAG_UINT64:
        argument->v_uint64 = NUM2ULONG(rb_argument);
        break;
      case GI_TYPE_TAG_FLOAT:
        argument->v_float = NUM2DBL(rb_argument);
        break;
      case GI_TYPE_TAG_DOUBLE:
        argument->v_double = NUM2DBL(rb_argument);
        break;
      case GI_TYPE_TAG_GTYPE:
        /* TODO: support GLib::Type and String as GType name. */
        argument->v_size = NUM2ULONG(rb_argument);
        break;
      case GI_TYPE_TAG_UTF8:
        /* TODO: support UTF-8 convert like rb_argument.encode("UTF-8"). */
        argument->v_string = (gchar *)RVAL2CSTR(rb_argument);
        break;
      case GI_TYPE_TAG_FILENAME:
        argument->v_string = (gchar *)RVAL2CSTR(rb_argument);
        break;
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        /* TODO */
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static VALUE
gi_argument_to_ruby(GIArgument *argument, GITypeInfo *type_info)
{
    VALUE rb_argument = Qnil;

    switch (g_type_info_get_tag(type_info)) {
      case GI_TYPE_TAG_VOID:
        rb_argument = Qnil;
        break;
      case GI_TYPE_TAG_BOOLEAN:
        rb_argument = CBOOL2RVAL(argument->v_boolean);
        break;
      case GI_TYPE_TAG_INT8:
        rb_argument = INT2NUM(argument->v_int8);
        break;
      case GI_TYPE_TAG_UINT8:
        rb_argument = UINT2NUM(argument->v_uint8);
        break;
      case GI_TYPE_TAG_INT16:
        rb_argument = INT2NUM(argument->v_int16);
        break;
      case GI_TYPE_TAG_UINT16:
        rb_argument = UINT2NUM(argument->v_uint16);
        break;
      case GI_TYPE_TAG_INT32:
        rb_argument = INT2NUM(argument->v_int32);
        break;
      case GI_TYPE_TAG_UINT32:
        rb_argument = UINT2NUM(argument->v_uint32);
        break;
      case GI_TYPE_TAG_INT64:
        rb_argument = LONG2NUM(argument->v_int64);
        break;
      case GI_TYPE_TAG_UINT64:
        rb_argument = ULONG2NUM(argument->v_uint64);
        break;
      case GI_TYPE_TAG_FLOAT:
        rb_argument = DBL2NUM(argument->v_float);
        break;
      case GI_TYPE_TAG_DOUBLE:
        rb_argument = DBL2NUM(argument->v_double);
        break;
      case GI_TYPE_TAG_GTYPE:
        rb_argument = rbgobj_gtype_new(argument->v_size);
        break;
      case GI_TYPE_TAG_UTF8:
        rb_argument = CSTR2RVAL(argument->v_string);
        break;
      case GI_TYPE_TAG_FILENAME:
        /* TODO: set encoding */
        rb_argument = CSTR2RVAL(argument->v_string);
        break;
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        /* TODO */
        break;
      case GI_TYPE_TAG_ERROR:
        RG_RAISE_ERROR(argument->v_pointer);
        break;
      case GI_TYPE_TAG_UNICHAR:
        /* TODO */
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return rb_argument;
}

static void
fill_in_argument(GIArgInfo *arg_info, GArray *in_args, int argc, VALUE *argv)
{
    VALUE rb_argument;
    GIArgument argument;

    /* TODO: check argc */
    rb_argument = argv[in_args->len];
    fill_gi_argument_by_ruby(&argument, arg_info, rb_argument);
    g_array_append_val(in_args, argument);
}

static void
fill_out_argument(GIArgInfo *arg_info, GArray *out_args)
{
    GIArgument argument;
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

static VALUE
rg_invoke(int argc, VALUE *argv, VALUE self)
{
    GIFunctionInfo *info;
    GICallableInfo *callable_info;
    gint i, n_args;
    GArray *in_args, *out_args;
    GIArgument return_value;
    GITypeInfo return_value_info;
    gboolean succeeded;
    GError *error = NULL;

    info = SELF(self);
    callable_info = (GICallableInfo *)info;
    n_args = g_callable_info_get_n_args(callable_info);
    in_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    out_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
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
                                       &return_value,
                                       &error);
    g_array_unref(in_args);
    g_array_unref(out_args);
    if (!succeeded) {
        RG_RAISE_ERROR(error);
    }
    g_callable_info_load_return_type(callable_info, &return_value_info);

    return gi_argument_to_ruby(&return_value, &return_value_info);
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
}
