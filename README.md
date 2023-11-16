![Tests](https://github.com/Craftarc/axel/actions/workflows/test.yml/badge.svg)

Documentation: https://craftarc.github.io/axel/index.html

# Axel Backend

This repository contains the source code for the companion application to the online ARPG, Path of Exile.

Axel is a stash currency tracker, and trade companion. This project is in the early stages of
development.

# Table of Contents

- [Introduction](#introduction)
- [Overview](#overview)
    - [About Axel](#about-axel)
        - [Technology Stack](#technology-stack)
            - [Stack](#stack)
            - [C++ Dependencies](#c++-dependencies)
            - [Tools](#tools)
            - [Services](#services)
- [Setup](#setup)
    - [Cloning the repository](#1-cloning-the-repository)
    - [Pulling the Docker images](#2-pulling-the-docker-images)
    - [Building the project](#3-building-the-project)
- [Configuration](#configuration)
    - [Utility scripts](#utililty-scripts)
- [Contributing](#contributing)
- [License](#license)

# Introduction

This documentation details the architecture of Axel Backend, and guides the user in setting up a development
environment. <br>
To contribute to this project, see [Overview](#overview) for a general overview of the project. <br>
To start from scratch, see [Setup](#setup)

# Overview

## About Axel

The frontend client is a **Tauri client**, responsible for

- Rendering graphs
- Displaying information about the user's characters, inventories, and net worth

The backend is made up of 3 components:

1. Docker container
2. Nginx server
3. C++ app

- serves the website
- nginx server acts as a reverse proxy into the C++ app
    - the C++ app does the actual business logic such as calling apis and writing
    to the SQLite3 database
- deal with the Oauth process

---

To that end, there are **two** main events that require communication between these two components.

1. **On first login, initiate an Oauth process**. The Lambda function mediates the process - we follow the
   standard [Oauth Authorization Code Flow](https://datatracker.ietf.org/doc/html/rfc6749).
    - Axel is considered a [Confidential Client](https://www.pathofexile.com/developer/docs/authorization#clients)
      according to the PoE Developer Docs. The redirect URL is the Lambda function URL.
    - The backend manages access tokens and session tokens,
        - This minimises security implications as the access token is never
        exposed on the frontend client.

2. **When the frontend client requires new data to update its displayed
   information _(e.g. chart data, updated currency prices)_, it makes a request to the backend**.
   The C++ app in the 
   responsible for making the appropriate requests to PoE's resource server
   and sending the data back to the Axel client.

## Technology Stack

The Axel backend depends on the following technologies. You do not need to
install them manually as they are all done by scripts.

### Stack
1. Ubuntu 22.04 on cloud VM
2. SQLite3
3. nginx

### C++ Dependencies

The Axel backend uses C++17.
1. [Boost](https://www.boost.org/)
    - Beast: Networking and HTTP requests
    - JSON: JSON parsing and formatting
2. [Botan](https://botan.randombit.net/)
    - Cryptographically secure random number generators
    - Hash functions
3. [Google Test](https://github.com/google/googletest)
    - Testing and mocking
4. [spdlog](https://github.com/gabime/spdlog)
    - Logging

### Tools
1. [Docker](https://www.docker.com/)
2. C++17
3. CMake 3.22
4. gcc 12

### Services
1. PoE Ninja API
2. [Official PoE API](https://www.pathofexile.com/developer/docs/index)

# Setup
This section guides you through a basic setup of the development environment, concluding with successfully running a
Google Test suite.

**You are strongly recommended to use an IDE like CLion or Visual Studio**.

## 1. Cloning the repository

To clone the repository, `cd` into the directory in which you want the
**root folder** of the repository to reside.

The following commands clone the repository into `~/axel/<contents-of-root-repository>`.

```
cd ~
mkdir axel
cd axel
git clone git@github.com:Craftarc/axel.git
```

## 2. Pulling the Docker images

Axel's development environment is containerised.

There are two images in use.

**1. dependencies** `docker pull craftarc/axel:dev`

- Stores all installed libraries the project uses.
- For use as a local development environment.

**2. staging** `docker pull craftarc/axel:staging`

- An environment that mimics deployment to the real VM as closely as possible.

## 3. Building the project

Axel's top-level CMakeLists.txt builds two primary executables: `main.cpp` and `sandbox.cpp`.

Along with the two executables, it also builds a testing executable.
**To build the project**,

- mount the project root into the container
- start an interactive shell

```
docker run -it craftarc/axel:dev -v .:/axel
```

You should be dropped into `/axel`, where the root directory of the project should be
mounted. Verify this with `pwd`.

**To build the tests**, first make sure our CMake settings are correct.
```
scripts/build-dev.sh
```
This script sets some predefined CMake settings. Note the `AXEL_TEST` flag -
this is to be set unless building for production.

- controls whether testing 'services' are used
    - will create a 'test' SQLite3 database when running, instead of the real
    database called 'app'
    - will avoid spamming offical PoE endpoints
    - etc...

We can then start compiling.
```
cd build
ninja
```

Wait for compilation to complete. The executables should be compiled into `/axel/build`. <br>
Finally, run the tests: `build/tests`

All tests should pass.

# Configuration

```
docker compose up dev
```



## Working with custom entrypoints

**To work with custom entrypoints, we want to execute the `sandbox` executable instead.** <br>

**1. Build updated binaries**.
In a container running the _craftarc/axel:dev_ image, build the updated
binaries with the desired cmake flags.

**3. Navigate to and run the `sandbox` executable.**

```
cd build
./sandbox
```

## Utililty scripts
In `/scripts`, there are a few utility scripts for common tasks.

# Contributing

# License

GNU General Public License v3.0


