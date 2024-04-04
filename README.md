# FlarialDumper

FlarialDumper to help update Flarial client, is a tool designed to extract offsets that change from version to version from client based on signatures. It can parse a configuration file in JSON format, search for signatures within an executable, and output the corresponding offsets in log file as well as in a separate file with custom format.

## Features

- Supports signature scanning, regular and vtable offset extraction.
- Flexible configuration via JSON file.
- Output to both console and log files.
- Offset output in hexadecimal format.
- Ability to specify the path to the executable, configuration file, and output file via command-line arguments.

## Installation

1. Clone the repository:

```
git clone https://github.com/yourusername/FlarialDumper.git
```

2. Compile the source code using your preferred compiler.

## Usage

```
./FlarialDumper <path_to_executable> <path_to_config_file> <output_file_name>
```

- `<path_to_executable>`: Path to the executable file to be analyzed.
- `<path_to_config_file>`: Path to the JSON configuration file containing signature information.
- `<output_file_name>`: Name of the output file where offset information will be saved.

## Configuration File Format

The configuration file should be in JSON format and contain an array of objects, each representing a signature to be scanned. Each object should have the following fields:

- `"name"`: Name of the signature.
- `"sig"`: Signature to be scanned.
- `"offset"` (optional): Offset to be added to the signature address (default is 3).
- `"type"` (optional): Type of signature (`"sig"` or `"vtable"`).
- `"output"` (optional): Custom output format for the offset.

Example configuration file:

```json
[
  {
    "name": "ClientInstance::grabMouse",
    "sig": "48 8B 80 ? ? ? ? EB ? 48 8B 0B",
    "type": "vtable",
    "output": "ADD_OFFSET(\"%NAME%\", %OFFSET%);"
  },
  {
    "name": "Actor::getCategories",
    "sig": "C4 20 5F C3 CC CC 8B 81 ? ? ? ? C3",
    "type": "sig",
    "offset": 8,
    "output": "ADD_OFFSET(\"%NAME%\", %OFFSET%);"
  }
]
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.