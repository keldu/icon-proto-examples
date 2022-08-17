#!/bin/false

lang_translation_map = {
    "Float32":"float",
    "Int32":"int32_t"
};

def replace_variable(tmpl_text, variable_name, variable_value):
    return tmpl_text.replace("${"+variable_name+"}", variable_value);

def generate_file(ext, tmpl_folder, variable_map):
    if tmpl_folder[-1] == "/":
        tmpl_folder = tmpl_folder[:-1];

    names = ["header", "body", "footer"];
    for index, val in enumerate(names):
        names[index] = tmpl_folder + "/" + names[index] + ext + ".tmpl";

    print(names);

    output_name = variable_map["OUTPUT_NAME"];

    tmpl_text = [];

    for file_name in names:
        tmpl_file = open(file_name, "r");
        tmpl_text.append(tmpl_file.read());


    generated_text = [];

    text = tmpl_text[0];
    text = replace_variable(text, "OUTPUT_NAME", variable_map["OUTPUT_NAME"]);

    generated_text.append(text);

    type_list = variable_map["TYPES"];

    for t in type_list:
        generated_text.append(replace_variable(tmpl_text[1], "TYPE", lang_translation_map[t]));

    text = replace_variable(tmpl_text[2], "OUTPUT_NAME", variable_map["OUTPUT_NAME"]);
    generated_text.append(text);

    text = "";

    for gt in generated_text:
        text = text + gt;

    print(text);
    pass

def generate_header(tmpl_folder, variable_map):
    generate_file(".h", tmpl_folder, variable_map);
    pass

def generate_source(tmpl_folder, variable_map):
    generate_file(".cpp", tmpl_folder, variable_map);
    pass

def generate(tmpl_folder, variable_map):
    generate_header(tmpl_folder, variable_map);
    generate_source(tmpl_folder, variable_map);
    pass
