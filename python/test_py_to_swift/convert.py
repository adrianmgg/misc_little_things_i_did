import importlib
import inspect
import os
import pkgutil
import re


module_to_convert_name = 'tests'  # change this variable

module_to_convert = importlib.import_module(module_to_convert_name)


class sample_empty_class(object):
    """
    empty class, used to determine what variables a class has by default
    """
    pass


def string_join_helper(delimiter, *args):
    """
    joins args on delimiter, skipping arguments which are either None or ''
    :type delimiter: str
    :type args: str | None
    """
    return delimiter.join([
        str(x) for x in args if (x is not None) and (len(str(x)) > 0)
    ])


def prefix_each_line(text, prefix):
    return re.sub(r"^", prefix, text, 0, re.MULTILINE)


def commentify(text):
    return prefix_each_line(text, '// ')


def indent(text, indent_level):
    return prefix_each_line(text, '\t' * indent_level)


def unindent(text):
    first_line_indent = re.match(r"^\n*( *)", text).group(1)
    return re.sub("^"+first_line_indent, "", text, 0, re.MULTILINE)


def format_doc(doc_str):
    if doc_str is None:
        return None
    else:
        return '/**\n' + unindent(doc_str).strip('\n') + '\n*/'


def convert_method(func, func_name, modifiers=None, args_front_slice=0):
    """
    :type func: function
    :type func_name: str
    :type modifiers: list of str | None
    :type args_front_slice: int
    """

    modifiers = modifiers or []
    modifiers = string_join_helper(' ', *modifiers)

    argspec = inspect.getargspec(func)
    defaults = argspec.defaults or []

    args_before_defaults = len(argspec.args) - len(defaults)

    arguments = [
        '{name}: {type}'.format(
            name=arg,
            type='Any'
        )
        for arg in argspec.args[:-args_before_defaults]
    ] + [
        '{name}: {type} = {default}'.format(
            name=arg,
            default=default,
            type='Any'
        )
        for arg, default in zip(argspec.args[-args_before_defaults:], defaults)
    ]

    arguments = arguments[args_front_slice:]  # remove self / cls arguments

    if argspec.varargs:
        arguments.append('_ {}: Any...'.format(argspec.varargs))
    # if hasattr(argspec, 'kwargs'):
    #     # TODO how do you do kwargs in swift?
    #     # just ignoring it for now
    #     pass

    signature = string_join_helper(' ', modifiers, func_name, 'func')

    # get source of function
    src = inspect.getsource(func)

    # remove docstring from source
    if func.__doc__ is not None:
        src = re.sub(
            pattern='^(\s*)([\'"]{3})' + re.escape(func.__doc__) + '([\'"]{3})\n?',
            repl='',
            string=src,
            count=1,
            flags=re.MULTILINE
        )

    body = commentify(unindent(src))

    if argspec.keywords is not None:
        body = '// FIXME kwargs not included in generated function\n\n' + body

    body = indent(indent_level=1, text=body)

    return string_join_helper('\n', format_doc(func.__doc__), signature + '{\n' + body + '\n}')


def convert_class(cls, cls_name):
    superclass = ': ' + cls.__bases__[0].__name__ if len(cls.__bases__) > 0 else None
    signature = string_join_helper(' ', 'class', cls_name, superclass)

    body = []

    for prop_name, prop in cls.__dict__.iteritems():
        if inspect.isfunction(prop):
            body.append(convert_method(
                func=prop,
                func_name=prop_name,
                args_front_slice=1
            ))
        elif isinstance(prop, staticmethod):
            body.append(convert_method(
                func=prop.__func__,
                func_name=prop_name,
                modifiers=['static'],
                args_front_slice=0
            ))
        elif isinstance(prop, classmethod):
            body.append(convert_method(
                func=prop.__func__,
                func_name=prop_name,
                modifiers=['class'],
                args_front_slice=1
            ))
        else:
            if prop_name in dir(object):
                continue
            if prop_name in dir(sample_empty_class):
                continue
            body.append(
                '// TODO: class had variable "{varname}" of type {vartype}'.format(
                    varname=prop_name,
                    vartype=type(prop).__name__
                )
            )

    body = map(lambda x: indent(x, 1), body)
    body = string_join_helper('\n\n', *body)

    return string_join_helper('\n', format_doc(cls.__doc__), signature + '{\n' + body + '\n}')


generated_files_root = 'swift_generated'


for module_loader, module_name, is_package in pkgutil.walk_packages(module_to_convert.__path__, module_to_convert.__name__ + '.'):  # type: (pkgutil.ImpImporter, str, bool)
    if not is_package:
        module = importlib.import_module(module_name)
        converted_classes = []
        for cls_name, cls in inspect.getmembers(module, inspect.isclass):
            if cls.__module__ != module_name:
                continue
            if isinstance(cls, type):
                converted_classes.append(convert_class(cls, cls_name))
        if len(converted_classes) > 0:
            module_dir_part = module_name.split('.')[:-1]
            module_name_part = module_name.split('.')[-1]
            generated_file_folder = os.path.join(generated_files_root, *module_dir_part)
            if not os.path.exists(generated_file_folder):
                os.makedirs(generated_file_folder)
            with open(os.path.join(generated_file_folder, module_name_part + '.swift'), 'w') as generated_file:
                generated_file.write(string_join_helper('\n\n', *converted_classes))
