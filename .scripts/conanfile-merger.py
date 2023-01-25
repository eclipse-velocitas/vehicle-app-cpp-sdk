import importlib.util
import sys
import re
import argparse
import inspect

def read_conanfile_txt(path: str) -> dict:
    file_dict = dict()
    current_key = None

    prog = re.compile(r"\[(\w+)\]")

    with open(path, "r") as conanfile:
        for line in conanfile:
            line = line.strip()

            match = prog.match(line)

            if match:
                current_key = match.group(1)
                file_dict[current_key] = list()
            elif len(line) > 0:
                file_dict[current_key].append(line)

    return file_dict


def load_module_at(path: str, custom_module_name: str):
    spec = importlib.util.spec_from_file_location(custom_module_name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[custom_module_name] = module
    spec.loader.exec_module(module)
    return module


def read_conanfile_py(path: str) -> dict:
    dependencies = dict()

    prog = re.compile(r"self.options\[\"(\w+)\"\].(\w+)\s*=\s*(\w+)")
    custom_module_name = "conanfile"
    for name, obj in inspect.getmembers(load_module_at(path, custom_module_name), inspect.isclass):
        if obj.__module__ == custom_module_name:
            for name2, obj2 in inspect.getmembers(obj):
                if name2 == "requires":
                    dependencies["requires"] = list(obj2)
                if name2 == "configure":
                    dependencies["options"] = list()
                    for line in inspect.getsourcelines(obj2)[0]:
                        line = line.strip()

                        m = prog.match(line)

                        if m:
                            dependency = m.group(1)
                            key = m.group(2)
                            value = m.group(3)
                            dependencies["options"].append(f"{dependency}:{key}={value}")

    return dependencies


def read_conanfile(path: str) -> dict:
    conanfile = None
    if path.endswith(".txt"):
        conanfile = read_conanfile_txt(path)
    elif path.endswith(".py"):
        conanfile = read_conanfile_py(path)
    else:
        raise "Invalid file extension for conanfile!"

    return conanfile


def merge_dicts(a: dict, b: dict) -> dict:
    joined_keys = list(set(list(a.keys()) + list(b.keys())))

    joined_dict = dict()

    for key in joined_keys:
        joined_dict[key] = set()

        if key in a:
            for item in a[key]:
                joined_dict[key].add(item)

        if key in b:
            for item in b[key]:
                joined_dict[key].add(item)

    return joined_dict


def write_conan_dict(path: str, dict: dict[str, list[str]]):
    with open(path, "w") as file:
        for key, items in dict.items():
            file.write(f"[{key}]\n")
            for item in items:
                file.write(f"{item}\n")
            file.write("\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Merge 2 conan files into one.')
    parser.add_argument('file1', type=str, help='The first file.')
    parser.add_argument('file2', type=str, help='The second file.')
    parser.add_argument('--output', type=str, required=True, help='Path to the output file')
    args = parser.parse_args()

    conanfile1 = read_conanfile(args.file1)
    conanfile2 = read_conanfile(args.file2)
    conanfile_joined = merge_dicts(conanfile1, conanfile2)
    write_conan_dict(args.output, conanfile_joined)
