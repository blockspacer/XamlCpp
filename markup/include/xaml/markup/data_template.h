#ifndef XAML_MARKUP_DATA_TEMPLATE_H
#define XAML_MARKUP_DATA_TEMPLATE_H

#include <xaml/converter.h>
#include <xaml/markup/template_base.h>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_data_template, { 0xa416dc5c, 0x8476, 0x4ec0, { 0xae, 0x92, 0x9d, 0x49, 0x90, 0xaa, 0xdc, 0x3f } })

#define XAML_DATA_TEMPLATE_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_TEMPLATE_BASE_VTBL(type));                  \
    XAML_PROP(converter, type, xaml_converter**, xaml_converter*);     \
    XAML_PROP(converter_parameter, type, xaml_object**, xaml_object*); \
    XAML_PROP(converter_language, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_data_template, xaml_template_base)
{
    XAML_DECL_VTBL(xaml_data_template, XAML_DATA_TEMPLATE_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_data_template_new(xaml_data_template**) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_data_template_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_data_template_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_DATA_TEMPLATE_H
