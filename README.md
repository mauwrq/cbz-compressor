# cbz-compressor

A command-line tool that shrinks comic book archives (`.cbz`). It resizes every page to a consistent width and re-encodes it as WebP, which can cut the file size by almost half without visible loss on screen.

```
> cbz-compressor "Vol 1.cbz"
Unzipping   [##############################] 209/209    0.9s
Scaling     [##############################] 209/209    13.0s
Converting  [##############################] 209/209    42.3s
Zipping     [##############################] 100%     2.4s
Done: D:\Comics\compressed\Vol 1.cbz
Size: 203.4 MB -> 120.5 MB (41% smaller)
Time: 58.8s (0.28s per page)
```

## Features

- Scales pages down to display size and converts them to WebP, shrinking the file with minimal visible quality loss
- Shows per-stage progress, timings and size savings, or nothing with `--quiet`

## Usage

```sh
cbz-compressor <file.cbz>       # writes compressed/<file.cbz> next to the input
cbz-compressor -q <file.cbz>    # same, without progress output
cbz-compressor -w 1600 <file.cbz>   # scale pages down to 1600 px wide instead of 1264
```

## Building

Requires CMake, a C++17 compiler, OpenCV, libzip, Boost.Program_options and GoogleTest.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build                    # run the tests
```
