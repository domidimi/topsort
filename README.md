[![Build Status](https://travis-ci.org/domidimi/topsort.svg?branch=master)](https://travis-ci.org/domidimi/topsort)

<a href="https://scan.coverity.com/projects/domidimi-topsort">
  <img alt="Coverity Scan Build Status"
       src="https://img.shields.io/coverity/scan/6430.svg"/>
</a>

# Simple implementation of a Topological sort in C++11

This is just a simple test project.
For a sophisticated solution to the topological sort problem, please look at the
Boost library.

Vertices are the keys of an associative container. The value of that container
is a list of vertices to which there exists a connection.

## Examples

## TODO
Add address sanitizer checks

Add cppcheck

Add Valgrind
