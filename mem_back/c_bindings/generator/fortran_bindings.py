#!/bin/false

import jinja2;


def generate_file(tmpl_path, output_name, variable_map):
    file = open(tmpl_path, "r");

    content = file.read();

    template = jinja2.Template(content);

    result = template.render(types=variable_map["types"]);
    
    out_file = open(output_name, 'w');
    out_file.write(result);

    pass

def generate(tmpl_folder, variable_map):
    generate_file(tmpl_folder+"fortran.f90.tmpl", tmpl_folder + "../binding.f90", variable_map);

    pass

