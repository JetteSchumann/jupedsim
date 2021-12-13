#! /bin/bash
set -ex

googletest_version="1.11.0"
fmt_version="8.0.1"
spdlog_version="1.9.2"
catch2_version="2.13.7"
cli11_version="2.1.2"
boost_version="1.78.0"

install_path=/usr/local

POSITIONAL=()
while [[ $# -gt 0 ]]; do
  key="$1"

  case $key in
    --install-path)
      install_path="$2"
      shift # past argument
      shift # past value
      ;;
    --compile-for-windows)
      shift # past argument
      compile_for_windows=1
      ;;
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
      ;;
  esac
done

if [ ${install_path} ]; then
    if [ ! -d "${install_path}" ]; then
        mkdir -p ${install_path}
    fi
    install_path="$(cd ${install_path}; pwd)"
fi

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    CPUS=$(nproc)
elif [[ "$OSTYPE" == "darwin"* ]]; then
    CPUS=$(sysctl -n hw.logicalcpu)
fi

if [[ ${compile_for_windows} ]]; then
    cross_compile_cmake_settings="-DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-c++-posix"
fi

function setup_boost {
    root=$(pwd)
    temp_folder=$(mktemp -d)
    cd ${temp_folder}
    boost_version_string=${boost_version//[.]/_}
    wget https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${boost_version_string}.tar.gz
    tar xf boost_${boost_version_string}.tar.gz
    cd boost_${boost_version_string}
    ./bootstrap.sh --prefix=${install_path} --with-libraries="headers"
    ./b2 --prefix=${install_path} install

    cd ${root}
    rm -rf ${temp_folder}
}

function setup_googletest {
    root=$(pwd)
    temp_folder=$(mktemp -d)
    cd ${temp_folder}

    wget https://github.com/google/googletest/archive/release-${googletest_version}.tar.gz
    tar xf release-${googletest_version}.tar.gz
    cd googletest-release-${googletest_version}
    mkdir build
    cd build
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=${install_path} \
        -DCMAKE_BUILD_TYPE=Release \
        ${cross_compile_cmake_settings}
    cmake --build . -j ${CPUS}
    cmake --install .
    cd ${root}
    rm -rf ${temp_folder}
}

function setup_fmt {
    root=$(pwd)
    temp_folder=$(mktemp -d)
    cd ${temp_folder}

    wget https://github.com/fmtlib/fmt/archive/${fmt_version}.tar.gz
    tar xf ${fmt_version}.tar.gz
    cd fmt-${fmt_version}
    mkdir build
    cd build
    cmake .. \
        -DCMAKE_CXX_FLAGS="-fPIC" \
        -DFMT_DOC=OFF \
        -DFMT_TEST=OFF \
        -DCMAKE_INSTALL_PREFIX=${install_path} \
        -DCMAKE_BUILD_TYPE=Release \
        ${cross_compile_cmake_settings}
    cmake --build . -j ${CPUS}
    cmake --install .
    cd ${root}
    rm -rf ${temp_folder}
}

function setup_spdlog {
    root=$(pwd)
    temp_folder=$(mktemp -d)
    cd ${temp_folder}

    wget https://github.com/gabime/spdlog/archive/v${spdlog_version}.tar.gz
    tar xf v${spdlog_version}.tar.gz
    cd spdlog-${spdlog_version}
    mkdir build
    cd build
    cmake .. \
        -DSPDLOG_BUILD_BENCH=OFF \
        -DSPDLOG_BUILD_TESTS=OFF \
        -DSPDLOG_FMT_EXTERNAL=ON \
        -DCMAKE_PREFIX_PATH=${install_path} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=${install_path} \
        ${cross_compile_cmake_settings}
    cmake --build . --target install -- -j${CPUS}

    cd ${root}
    rm -rf ${temp_folder}
}

function setup_catch2 {
    root=$(pwd)
    temp_folder=$(mktemp -d)
    cd ${temp_folder}

    wget https://github.com/catchorg/Catch2/archive/v${catch2_version}.tar.gz
    tar xf v${catch2_version}.tar.gz
    cd Catch2-${catch2_version}
    mkdir build
    cd build
    cmake .. \
        -DBUILD_TESTING=OFF      \
        -DCMAKE_PREFIX_PATH=${install_path} \
        -DCMAKE_INSTALL_PREFIX=${install_path} \
        ${cross_compile_cmake_settings}
    cmake --build . --target install -- -j${CPUS}

    cd ${root}
    rm -rf ${temp_folder}
}

function setup_cli11 {
    root=$(pwd)
    temp_folder=$(mktemp -d)
    cd ${temp_folder}

    wget https://github.com/CLIUtils/CLI11/archive/v${cli11_version}.tar.gz
    tar xf v${cli11_version}.tar.gz
    cd CLI11-${cli11_version}
    mkdir build
    cd build
    cmake .. \
        -DCMAKE_PREFIX_PATH=${install_path} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCLI11_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=${install_path} \
        -DCLI11_BUILD_EXAMPLES=OFF \
        ${cross_compile_cmake_settings}
    cmake --build . --target install -- -j${CPUS}

    cd ${root}
    rm -rf ${temp_folder}
}

setup_boost
setup_googletest
setup_fmt
setup_spdlog
setup_catch2
setup_cli11

