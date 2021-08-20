{
  "targets": [
    {
      "target_name": "main",
      "sources": [ "main.cpp" ], 
      "cflags": ["-Wall", "-Wextra", "-pedantic", "-ansi", "-O3"],
      "include_dirs" : ["<!(node -e \"require('nan')\")"]
    }
  ]
}