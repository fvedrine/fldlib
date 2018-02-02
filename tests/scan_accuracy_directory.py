#!/usr/bin/python
import os
import re
import datetime
import json
import sys

def find_print(c_file):
    result = set()
    with open(c_file, "r") as searchfile:
        for line in searchfile:
            if "PRINT" in line:
                res = re.search("PRINT", line)
                line_length = len(line)
                index = res.end()
                count_paren = 0
                while index < line_length and line[index] != '(':
                    index += 1
                if index < line_length and line[index] == '(':
                    count_paren = 1
                    index = index+1
                    start_index = index
                    while index < line_length and count_paren > 0:
                        if line[index] == '(':
                             count_paren += 1
                        elif line[index] == ')':
                             count_paren -= 1
                        index += 1
                    if count_paren == 0:
                        end_index = index-1
                        result.add(line[start_index:end_index])
    return result

def convert_value(float_text):
    if float_text == "+oo" or float_text == "-oo" or float_text == "oo":
        result = -float('inf') if float_text == "-oo" else float('inf')
    elif float_text == "+NaN" or float_text == "-NaN" or float_text == "NaN":
        result = -float('inf') if float_text == "-NaN" else float('inf')
    else:
        result = float(float_text)
    return result

def minmax_fluctuat_result(pattern, ouput_file):
    has_result = False
    min_val = 0
    max_val = 0
    if os.path.isfile(ouput_file):
        with open(ouput_file, "r") as searchfile:
            for line in searchfile:
                found = re.search(pattern, line)
                if found:
                    min_txt = found.group("min")
                    max_txt = found.group("max")
                    min_cur_val = convert_value(min_txt)
                    max_cur_val = convert_value(max_txt)
                    if has_result:
                        if min_cur_val < min_val:
                            min_val = min_cur_val
                        if max_cur_val > max_val:
                            max_val = max_cur_val
                    else:
                        min_val = min_cur_val
                        max_val = max_cur_val
                    has_result = True
    return (has_result, min_val, max_val)

class AccuracyResult:
  def __init__(self, name, float_min, float_max, real_min, real_max,
                           err_min, err_max, rel_min, rel_max):
     self.tname = name
     self.float_min = float_min
     self.float_max = float_max
     self.real_min = real_min
     self.real_max = real_max
     self.err_min = err_min
     self.err_max = err_max
     self.rel_min = rel_min
     self.rel_max = rel_max

class VariableAccuracyResult:
  def __init__(self, variable_name, tool):
     self.name = variable_name
     self.tools = [ tool ]
  def add(self, tool):
     self.tools.append(tool)

class FileAccuracyResult:
  def __init__(self, dir_name, file_name, variables_result):
     self.dir = dir_name
     self.file = file_name
     self.variables = variables_result

class FinalResult:
  def __init__(self, time_stamp, results_by_file):
     self.time_stamp = time_stamp
     self.content = results_by_file

class AccuracyJSONEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, FinalResult):
            return o.__dict__
        if isinstance(o, FileAccuracyResult):
            return o.__dict__
        if isinstance(o, VariableAccuracyResult):
            return o.__dict__
        if isinstance(o, AccuracyResult):
            return o.__dict__
        if isinstance(o, datetime.datetime):
            return o.isoformat()
        return json.JSONEncoder.default(self, o)

def find_fluctuat_results(directory, variable):
    result = None
    pattern = re.compile(r"""\s*(?P<subdiv1>[-+]?\d+)
                             \s+(?P<subdiv2>[-+]?\d+)
                             \s+(?P<min>([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo))
                             \s+(?P<max>([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo))
                          """, re.VERBOSE)
    filename = os.path.join(directory, "output")
    filename_float = os.path.join(filename, variable+".evolval")
    (has_float_result, min_val, max_val) = minmax_fluctuat_result(pattern, filename_float)
    filename_real = os.path.join(filename, variable+".evolvalexacte")
    (has_real_result, min_real, max_real) = minmax_fluctuat_result(pattern, filename_real)
    filename_error = os.path.join(filename, variable+".evolerr")
    (has_error_result, min_error, max_error) = minmax_fluctuat_result(pattern, filename_error)
    filename_rel_error = os.path.join(filename, variable+".evolrelerr")
    (has_rel_error_result, min_rel_error, max_rel_error) = minmax_fluctuat_result(pattern, filename_rel_error)
    if has_float_result or has_real_result or has_error_result or has_rel_error_result:
        if not has_float_result:
            min_val = float('nan')
            max_val = float('nan')
        if not has_real_result:
            min_real = float('nan')
            max_real = float('nan')
        if not has_error_result:
            min_error = float('nan')
            max_error = float('nan')
        else:
            temp_error = -max_error
            max_error = -min_error
            min_error = temp_error
        if not has_rel_error_result:
            min_rel_error = float('nan')
            max_rel_error = float('nan')
        else:
            temp_error = -max_rel_error
            max_rel_error = -min_rel_error
            min_rel_error = temp_error
        result = AccuracyResult("fluctuat", min_val, max_val, min_real, max_real,
                                            min_error, max_error, min_rel_error, max_rel_error)
    return result

def locate_c_file_in_results(list_dir_result, subdir, c_base_file):
    for result in list_dir_result:
        if result.dir == subdir and result.file == c_base_file:
            return result
    return None

def find_variables_in_file(subdir, c_base_file, file_done, find_variable_results,
        list_dir_result, cur_dir, srcdir):
    c_file = os.path.join(srcdir, subdir, c_base_file)
    if os.path.isfile(c_file):
        variables = find_print(c_file)
        if variables:
            variables_result = locate_c_file_in_results(list_dir_result, subdir, c_base_file)
            if variables_result:
                find_variable_results(variables, subdir, file_done, variables_result.variables)
            else:
                variables_result = []
                find_variable_results(variables, subdir, file_done, variables_result)
                if len(variables_result) > 0:
                    list_dir_result.append(FileAccuracyResult(cur_dir, c_base_file, variables_result))

def find_fluctuat_variable_results(variables, subdir, file_fluctuat_done, variables_result):
    for variable in variables:
        variable_result = None
        results = find_fluctuat_results(
            os.path.join(subdir, file_fluctuat_done[:-14] + "_fluctuat"), variable)
        if not results is None:
            if variable_result:
                variable_result.add(results)
            else:
                variable_result = VariableAccuracyResult(variable, results)
        if variable_result:
            locate_result, var_result = locate_var_in_results(variables_result,
                    variable, "fluctuat")
            if var_result:
                var_result.tools.append(variable_result.tools[0])
            else:
                variables_result.append(variable_result)

def locate_var_in_results(variables_result, var_name, tool_name):
    for accuracy_result in variables_result:
        if (accuracy_result.name == var_name):
            for tool in accuracy_result.tools:
                if (tool.tname == tool_name):
                    return (tool, accuracy_result)
            return (None, accuracy_result)
    return (None, None)

def find_diagnosis_results(variables, subdir, file_diagnosis_done, variables_result, kind, kind2):
    file_name = os.path.join(subdir,
            file_diagnosis_done[:-len(".diagnosis_"+kind+"_done")]
                + "_diag_"+kind2+"_out");
    if os.path.isfile(file_name):
        pattern = re.compile(r"""(?P<var_name>[a-zA-Z0-9_][^:]*)\s*:
            \s+i=\[(?P<val_min>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*,
               \s*(?P<val_max>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*\](\{[^\}]*\})?\s*,
            \s+r=\[(?P<real_min>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*,
               \s*(?P<real_max>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*\]({[^}]*})?\s*,
            \s+e=\[(?P<err_min>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*,
               \s*(?P<err_max>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*\]({[^}]*})?\s*,
            \s+re=\[(?P<relerr_min>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*,
               \s*(?P<relerr_max>[-+]?((\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)|([-+]?oo)|([-+]?NaN))\s*\](\{[^\}]*\})?
            """, re.VERBOSE)
        cur_file = open(file_name, "r")
        for cur_line in cur_file:
            if cur_line.startswith("unstable test:"):
                continue
            found = re.match(pattern, cur_line)
            if found:
                var_name = found.group("var_name")
                cur_val_min = convert_value(found.group("val_min"))
                cur_val_max = convert_value(found.group("val_max"))
                cur_real_min = convert_value(found.group("real_min"))
                cur_real_max = convert_value(found.group("real_max"))
                cur_err_min = convert_value(found.group("err_min"))
                cur_err_max = convert_value(found.group("err_max"))
                cur_relerr_min = convert_value(found.group("relerr_min"))
                cur_relerr_max = convert_value(found.group("relerr_max"))
                locate_result, var_result = locate_var_in_results(variables_result,
                        var_name, "diagnosis_" + kind)
                if locate_result:
                    if cur_val_min < locate_result.float_min:
                        locate_result.float_min = cur_val_min
                    if cur_val_max > locate_result.float_max:
                        locate_result.float_max = cur_val_max
                    if cur_real_min < locate_result.real_min:
                        locate_result.real_min = cur_real_min
                    if cur_real_max > locate_result.real_max:
                        locate_result.real_max = cur_real_max
                    if cur_err_min < locate_result.err_min:
                        locate_result.err_min = cur_err_min
                    if cur_err_max > locate_result.err_max:
                        locate_result.err_max = cur_err_max
                    if cur_relerr_min < locate_result.rel_min:
                        locate_result.rel_min = cur_relerr_min
                    if cur_relerr_max > locate_result.rel_max:
                        locate_result.rel_max = cur_relerr_max
                elif var_result:
                    var_result.tools.append(AccuracyResult("diagnosis_" + kind,
                            cur_val_min, cur_val_max, cur_real_min, cur_real_max,
                            cur_err_min, cur_err_max, cur_relerr_min, cur_relerr_max))
                else:
                    variables_result.append(VariableAccuracyResult(var_name,
                        AccuracyResult("diagnosis_" + kind,
                            cur_val_min, cur_val_max, cur_real_min, cur_real_max,
                            cur_err_min, cur_err_max, cur_relerr_min, cur_relerr_max)))
        cur_file.close();

def find_diagnosis_exact_results(variables, subdir, file_diagnosis_done, variables_result):
    return find_diagnosis_results(variables, subdir, file_diagnosis_done, variables_result, "exact", "exact")

def find_diagnosis_interval_results(variables, subdir, file_diagnosis_done, variables_result):
    return find_diagnosis_results(variables, subdir, file_diagnosis_done, variables_result, "interval", "int")

def find_diagnosis_affine_results(variables, subdir, file_diagnosis_done, variables_result):
    return find_diagnosis_results(variables, subdir, file_diagnosis_done, variables_result, "affine", "aff")

def find_diagnosis_affine_scenario_results(variables, subdir, file_diagnosis_done, variables_result):
    return find_diagnosis_results(variables, subdir, file_diagnosis_done, variables_result, "affine_scenario", "aff_scenario")

time_stamp = datetime.datetime.now()
list_dir = []

srcdir = '.'
if len( sys.argv ) > 1:
    srcdir = sys.argv[1]

print "scan directories for accuracy results"
for subdir, dirs, files in os.walk('.'):
    cur_dir = subdir
    for file in files:
        # for fluctuat
        if re.match(".*\.fluctuat_done", file):
            print "process " + file
            find_variables_in_file(subdir, file[:-14] + ".c",
                    file, find_fluctuat_variable_results, list_dir, cur_dir, srcdir)
        # for diagnosis_exact
        if re.match(".*\.diagnosis_exact_done", file):
            print "process " + file
            find_variables_in_file(subdir, file[:-len(".diagnosis_exact_done")] + ".c",
                    file, find_diagnosis_exact_results, list_dir, cur_dir, srcdir)
        # for diagnosis_interval
        if re.match(".*\.diagnosis_interval_done", file):
            print "process " + file
            find_variables_in_file(subdir, file[:-len(".diagnosis_interval_done")] + ".c",
                    file, find_diagnosis_interval_results, list_dir, cur_dir, srcdir)
        # for diagnosis_affine
        if re.match(".*\.diagnosis_affine_done", file):
            print "process " + file
            find_variables_in_file(subdir, file[:-len(".diagnosis_affine_done")] + ".c",
                    file, find_diagnosis_affine_results, list_dir, cur_dir, srcdir)
        # for diagnosis_affine_scenario
        if re.match(".*\.diagnosis_affine_scenario_done", file):
            print "process " + file
            find_variables_in_file(subdir, file[:-len(".diagnosis_affine_scenario_done")] + ".c",
                    file, find_diagnosis_affine_scenario_results, list_dir, cur_dir, srcdir)
if list_dir:
    print "export accuracy table"
    with open("accuracy_table.json","w") as file: 
        file.write(json.dumps(FinalResult(time_stamp, list_dir), cls=AccuracyJSONEncoder, sort_keys=True, indent=2))
    print "export accuracy table done"
else:
    print "no accuracy table exported"

