# Build

This file contains instructions on how to build and run the srcfacts program.

This project does not support an in-source build. The instructions are based
on a build subdirectory. You can also use a sibling directory of the source.

The commands are given in terms of `make`. If you setup cmake with `ninja`
as your build tool (highly recommended), then just replace `make` with `ninja`
in those commands.

## Setup

Create a build directory and move into it:

```console
mkdir build
cd build
```

Once in your build directory run cmake with a path to the (parent) source directory:

```console
cmake ..
```

Running cmake with the path to the source directory is only needed the first time.
From then on, you can directly run cmake in the build directory:

```console
cmake .
```


You can then perform the build:

```console
make
```

## Run Demo

An example input file is provided. To run with make:

```console
make run
```

To run on the command line:

```console
./srcfacts < data/demo.xml
```

You can also time it:

```console
time ./srcfacts < data/demo.xml
```

## Tracing

Tracing shows each parsing event on a separate output line.
Trace is off by default. To turn tracing on:

```console
cmake .. -DTRACE=ON
```

To turn tracing back off:

```console
cmake .. -DTRACE=OFF
```

## BigData

The included demo file is quite small. In order to check scalability, a much larger example
can be used. This larger example is the srcML file for linux-6.0 and consists of 55,174 linux source-code
files. The zip archive download is not that big (over 280 MB), but the unzipped file is extremely large
at over 4 GB.

To download the linux kernel example, use the following cmake option:

```console
cmake . -DDOWNLOAD_BIGDATA=ON
```

This option does not directly download the file, but provides instructions and a warning about the
resulting file size. To confirm the download, use the following cmake option:

```console
cmake . -DDOWNLOAD_BIGDATA_CONFIRM=ON
```

The resulting file, linux-6.0.xml, is placed in the data subdirectory of your build.

To run this linux kernel example with your program, use the following make command:

```console
make run_bigdata
```

To run on the command line:

```console
./srcfacts < data/linux-6.0.xml
```

You can also time it:

```console
time ./srcfacts < data/linux-6.0.xml
```
