ARG VARIANT=debian-11
# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.205.2/containers/python-3/.devcontainer/base.Dockerfile
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
  && apt-get -y install lsb-release wget software-properties-common \
  && apt-get -y install build-essential ninja-build valgrind gdb lcov doxygen graphviz \
  && apt-get -y install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev

# Install latest cmake 3.22 so we have support for cmake presets and a better integration in IDEs
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.0/cmake-3.22.0-linux-x86_64.sh \
  -q -O /tmp/cmake-install.sh \
  && chmod u+x /tmp/cmake-install.sh \
  && /tmp/cmake-install.sh --skip-license --prefix=/usr \
  && rm /tmp/cmake-install.sh

# Install clang-14
RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN add-apt-repository 'deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye-14 main'
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
  && apt-get -y install clang-14 clangd-14 lldb-14 llvm-14 clang-format-14 clang-tidy-14

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-14 100
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-14 100
RUN update-alternatives --install /usr/bin/clang-apply-replacements clang-apply-replacements /usr/bin/clang-apply-replacements-14 100
RUN update-alternatives --install /usr/bin/clang-check clang-check /usr/bin/clang-check-14 100
RUN update-alternatives --install /usr/bin/clang-query clang-query /usr/bin/clang-query-14 100
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-14 100
RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-14 100
RUN update-alternatives --install /usr/bin/scan-build scan-build /usr/bin/scan-build-14 100
RUN update-alternatives --install /usr/bin/scan-view scan-view /usr/bin/scan-view-14 100
RUN update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-14 100
RUN update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-14 100

# Install ccache
RUN apt-get -y install ccache

# Clean-up
RUN apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
