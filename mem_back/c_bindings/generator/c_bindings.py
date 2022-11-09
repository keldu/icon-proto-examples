#!/bin/false

import jinja2;

lang_translation_map = {
    "Float32": "float",
    "Float64": "double",
    "Int8"   : "int8_t",
    "Int16"  : "int16_t",
    "Int32"  : "int32_t",
    "Int64"  : "int64_t",
    "UInt8"  : "uint8_t",
    "UInt16" : "uint16_t",
    "UInt32" : "uint32_t",
    "UInt64" : "uint64_t"
};

def generate_file(tmpl_path, output_name, variable_map):
    file = open(tmpl_path, "r");

    content = file.read();

    template = jinja2.Template(content);

    result = template.render(types=variable_map["types"]);
    
    out_file = open(output_name, 'w');
    out_file.write(result);

    pass

def generate(tmpl_folder, variable_map):
    generate_file(tmpl_folder+"c.h.tmpl", tmpl_folder + "../binding.h", variable_map);
    generate_file(tmpl_folder+"c.c.tmpl", tmpl_folder + "../binding.cpp", variable_map);

    pass
