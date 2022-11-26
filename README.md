# VbfEditor

Open source library and tool for repacking VBF files.
VBF(Versatile Binary File) format is widely used in automotive software by Ford,
Volvo, Jaguar\Land Rover manufactures.   

## Usage
```
Usage:
  vbfeditor [OPTION...] positional parameters

  -p, --pack        Pack VBF file
  -u, --unpack      Unpack VBF file
  -I, --info        Show info about VBF file
  -i, --input arg   Input file
  -o, --output arg  Output directory (default: "")
  -v, --version     Print version
  -h, --help        Print help
```

### Show VBF info
Use `-I` option to display info about VBF file sections:  
```
vbfeditor -I path_to.vbf 
Found 2 sections
  # |    Offset    |  Start addr  |  Length 
  0 |  0x000004e7  |  0x00004000  |  0x0009ffa0 (655264)
  1 |  0x000a0491  |  0x000b6890  |  0x00000f10 (3856)
```

### Unpack VBF

Use `-u` option to extract VBF sections to binary files:  
`vbfeditor -u path_to.vbf -o unpacked/vbf/dir/`  

Extracted content will be placed to specified folder:
```
unpacked/vbf/dir/
├── path_to.vbf_ascii_head.txt
├── path_to.vbf_config.json
├── path_to.vbf_section_1_4000_9ffa0.bin
└── path_to.vbf_section_2_b6890_f10.bin
```

### Pack VBF 
Use `-p` option to repack modified binary sections to VBF container:  
`vbfeditor -p vbf_config.json -o path/to/repacked.vbf`
