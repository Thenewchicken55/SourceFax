[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-f4981d0f882b2a3f0472912d15f9806d57e124e0fc890972558857b51b24a6f9.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=9799964)
# srcFacts

Calculates various counts on a source-code project, including files, functions,
comments, etc.

Input is a srcML form of the project source code. An example srcML file for srcFacts.cpp
is included.

The srcFacts main program includes code to directly parse XML interleaved with code
to collect the counts and code at the end to generate the report.

Notes:
* The integrated XML parser handles all parts of XML.
* Program should be fast. A run on the BIGDATA linux kernel example takes about 5 seconds
on an Macbook Air M1. Very little RAM is used.
