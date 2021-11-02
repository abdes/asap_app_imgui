.. Structure conventions
     # with overline, for parts
     * with overline, for chapters
     = for sections
     - for subsections
     ^ for subsubsections
     " for paragraphs

*****************
Project structure
*****************

Compared to other languages such Javscript, Java or Rust, C++ seriously lacks 
built-in capabilities for defining, building and using modules. Many competing
approaches exist such as `Conan <https://conan.io/>`_, `vcpkg <https://vcpkg.io/>`_,
`Hunter <https://github.com/cpp-pm/hunter>`_, etc. None of them completely solves
the problem and the worst part is that they often are mutually exclusive.

The choices made in the `asap` starter project with regard to the project layout
and to how modules (internal and 3rd party) are integrated and managed do not 
exclude the use of package managers or lock you down to one approach or another.

The emphasis is put on a clean and extensible project layout and a build system 
that stays as much as possible within the commonly used things in CMake.

Software Architecture: The Module Viewtype [#f1]_
=================================================

Decomposing a system into manageable units remains one of the most common ways 
to think about the structure of the system. Often, it also determines how a 
system's source code is partitioned into separate parts, what kind of interfaces
exist between these parts and how these parts are aggregated into larger 
ensembles.

In the context of `asap` we adopt a broad definition of the `module` concept as 
an implementation unit of software that provides a coherent unit of 
functionality. Because we are dealing specifically with C++ development, that 
implementation unit will be a set of C++ files, configuration files, static
data files, etc. In addition to that,we also deal with documentation files, and 
various build scripts, including CMake and other tools.

Modules can be related to each others in different ways:

- **A is part of B**: this type of relation can indicate simple aggregation, 
  such as when a module is included into one or many others, or could indicate 
  a more sophisticated decomposition relationship.
- **A depends on B**: this relation can take several forms such as uses, 
  allowed-to-use, sends-data-to, transfers-control-to, and so forth.
- **A is a B**: this one defines a generalization relationship and in general 
  would not impact the macro-level structure of the project. We do however use 
  it in this starter project to define library and executable modules for which 
  we provide generic reusable CMake build scripts that can be simply extended.

To the maximum possible extent we want to preserve and faithfully map the 
modules and their relations in the software architecture to the development 
infrastructure. Implementing a module always results in many separate files, 
such as those that contain the source code, files that have to be included and 
usually contain definitions, files that describe how to build an executable, 
and files that are the result of translating, or compiling, the module. Those 
files need to be organized so as not to lose control and integrity of the system. 
Configuration management techniques usually do this job: in the simplest case, 
a hierarchy of directories in a file system.

Assuming this starter project is used to build a simple application that reads 
and writes files on the local filesystem, a representation of the application 
architecture in a module decomposition view would look like this:

.. code-block:: text

    ┌─────────────────────────────────────────────────────────────────┐
    │ ASAP Application                                                │
    │  (System)                                                       │
    │  ┌─────────────────────┐                          ┌──────────┐  │
    │  │                     ├──────────────────────────►          │  │
    │  │                     │                          │          │  │
    │  │                     │  ┌───────────────────┐   │          │  │
    │  │                     │  │                   │   │          │  │
    │  │                     │  │ logging frameowrk │   │          │  │
    │  │                     │  │                   │   │          │  │
    │  │                     │  │               3PM │   │          │  │
    │  │                     │  └───────────▲───────┘   │          │  │
    │  │                     │              │           │          │  │
    │  │       Main          │              │           │          │  │
    │  │                     │  ┌───────────┴───────┐   │          │  │
    │  │    Application      │  │  common           │   │          │  │
    │  │                     │  │      ┌──────────┐ │   │          │  │
    │  │       Module        │  │      │Hedley    │ │   │          │  │
    │  │                     ├──►      │       PM │ ├───►          │  │
    │  │                     │  │      └──────────┘ │   │   unit   │  │
    │  │                     │  │                   │   │          │  │
    │  │                     │  └───▲───────────────┘   │  testing │  │
    │  │                     │      │                   │          │  │
    │  │                     │  ┌───┴───────────────┐   │ framework│  │
    │  │                     │  │                   │   │          │  │
    │  │                     ├──►  filesystem       ├───►          │  │
    │  │                     │  │                   │   │          │  │
    │  │                     │  │                   │   │      3PM │  │
    │  └─────────────────────┘  └───────────────────┘   └──────────┘  │
    │                                                                 │
    └─────────────────────────────────────────────────────────────────┘

The arrow in the diagram above mainly refer to a uses relationship. Notice how 
the modules can be 3rd party modules (indicated with a `3PM` sticker), provided
by `asap` as reusable components or specifically made for the application.

Also notice how the `common` module contains another 3rd party module (Hedley). 
Composition is allowed at any depth within the structure of an `asap` based 
project. In this particular case, this aggregation is implemented using a `git` 
submodule.

The `Unit Testing Frameowrk` is extensively used in the project but only during 
development. It won't be part of the final application package for deployment 
for instance. Such modules exist within the project and the CMake build system
can clear make the difference between the artifacts intended for use during 
evelopment only vs. the ones destined for deployment.

Project Layout
==============

Based on the module decomposition view above, we now can look at how this 
translates into the hierarchy of directories forming the `asap` project layout.

.. code-block:: text

    .
    ├── <.git>
    ├── <.sphinx>
    ├── <build>
    │   
    ├── <.husky>
    ├── <.vscode>
    ├── <doxygen>
    ├── .clang-format
    ├── .commitlintrc.json
    ├── .gitignore
    ├── .gitmodules
    ├── .runsettings
    ├── .versionrc.json
    ├── requirements.txt
    │   
    ├── <doc>
    │   
    ├── <deploy>
    │   
    ├── <data>
    │   
    ├── <templates>
    ├── <common>
    ├── <filesystem>
    ├── <third_party>
    │   ├── <catch2>
    │   ├── <spdlog>
    │   └── CMakeLists.txt
    │   
    ├── <cmake>
    ├── CMakeLists.txt
    ├── asap-config.cmake
    │   
    ├── AUTHORS
    ├── LICENSE
    ├── README.md
    └── CHANGELOG.md

.. rubric: Footnotes

.. [#f1] An excellent book on this topic is `Documenting Software Architectures:
   Views and Beyond <https://www.amazon.com/Documenting-Software-Architectures-Views-Beyond/dp/0201703726/ref=sr_1_1?_encoding=UTF-8&camp=212361&creative=380601&dchild=1&keywords=0201703726&link_code=wql&qid=1635883655&s=books&sr=1-1>`_
   (available on Amazon, or just Google it).
