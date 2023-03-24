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

## xmlstats

By default, the build also builds the application *xmlstats*.

To build only the xmlstats application:

```console
make xmlstats
```

To run xmlstats with the demo file using make:

```console
make run_xmlstats
```

To run xml stats with the demo file on the command line:

```console
./xmlstats < data/demo.xml
```

## identity

By default, the build also builds the application *identity*.

To build only the identity application:

```console
make identity
```

To run identity with the demo file using make:

```console
make run_identity
```

The target runidentity does not output to standard output, but is redirected
to the file *democopy.xml*. It is equivalent to next command.

To run xml stats with the demo file on the command line:

```console
./identity < data/demo.xml > democopy.xml
```

If you want to compare the output to the input for *identity*, you can use
the following command:

```console
make run_identity_check
```

This is equivalent to:

```console
make run_identity
diff data/demo.xml democopy.xml
```

This runs a diff between the input file and the output file. Note that the
output can be very large.

