![Tests](https://github.com/Craftarc/axel/actions/workflows/test.yml/badge.svg)

Documentation: https://craftarc.github.io/axel/index.html

# Axel Backend

This repository contains the source code for the companion application to the online ARPG, Path of Exile.

Axel is a replacement for Exilence, which has recently gone out of service. This project is in the early stages of
development.

# Table of Contents

- [Introduction](#introduction)
- [Overview](#overview)
    - [About Axel](#about-axel)
        - [Technology Stack](#technology-stack)
        - [Libraries](#libraries)
            - [Tools](#tools)
            - [Services](#services)
        - [About AWS Lambda](#about-aws-lambda)
            - [The Lambda Execution Environment](#the-lambda-execution-environment)
- [Setup](#setup)
    - [Cloning the repository](#1-cloning-the-repository)
    - [Pulling the Docker images](#2-pulling-the-docker-images)
    - [Building the project](#3-building-the-project)
- [Configuration](#configuration)
    - [Working with the RIE](#working-with-the-rie)
        - [Mounting the RIE into the container](#mounting-the-rie-into-the-container)
        - [Deploying into an official RIE image](#deploying-into-an-official-rie-image)
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

**The backend is an AWS Lambda function**, referred to as the _Lambda_.

A Lambda function is code deployed on a server that only activates when the
function's endpoints are hit. <br>
More simply put, when the desktop client calls the Lambda function, it activates, processes the data,
and outputs the response back to the desktop client. All code in this repository deals with is what happens between that
input and that output.

**The frontend client is a Neutralino.js GUI**, referred to as the _Axel client_, and is generally responsible for:

- Rendering graphs
- Displaying information about the user's characters, inventories, and net worth

---

To that end, there are **two** main events that require communication between these two components.

1. **On first login, initiate an Oauth process**. The Lambda function mediates the process - we follow the
   standard [Oauth Authorization Code Flow](https://datatracker.ietf.org/doc/html/rfc6749).
    - Axel is considered a [Confidential Client](https://www.pathofexile.com/developer/docs/authorization#clients)
      according to the PoE Developer Docs. The redirect URL is the Lambda function URL.
    - The backend manages access tokens and session tokens,
        - This minimises security implications as the access token is never exposed on the frontend client.
2. **When the frontend client requires new data to update its displayed
   information _(e.g. chart data, updated currency prices)_, it makes a request to the backend**. The Lambda function is
   responsible for making the appropriate requests to PoE's resource server and sending the data back to the Axel
   client.

## Technology Stack

The Axel backend depends on the following technologies. Explicit installation or building of these items _(except
Docker)_ is not required, as they are managed within a Docker image.

### Libraries

The Axel backend uses C++17.

1. [Boost](https://www.boost.org/)
    - Beast: Networking and HTTP requests
    - JSON: JSON parsing and formatting
2. [Botan](https://botan.randombit.net/)
    - Cryptographically secure random number generators
    - Hash functions
3. [Google Test](https://github.com/google/googletest)
    - Testing and mocking
4. [AWS C++ SDK](https://aws.amazon.com/sdk-for-cpp/)
    - Lambda and DynamoDB interface
5. [AWS C++ Runtime](https://github.com/awslabs/aws-lambda-cpp)
    - Lambda deployment compatibility layer. See [The Lambda Execution Environment](#the-lambda-execution-environment)
6. [spdlog](https://github.com/gabime/spdlog)
    - Logging

### Tools

1. [Docker](https://www.docker.com/)
2. C++17
3. CMake 3.22
4. gcc 11.4.0

### Services

1. AWS Lambda
2. AWS DynamoDB
3. PoE Ninja API
4. [Official PoE API](https://www.pathofexile.com/developer/docs/index)

## About AWS Lambda

### The Lambda Execution Environment

When a HTTP request from the Axel client reaches the Lambda's endpoint, the request data is stored. The AWS
Lambda service then activates our Lambda function, which consists of two layers: The _AWS C++ Runtime_, and our code.
The _Runtime_ polls the Lambda service, retrieves the stored request data, then passes it to our code for processing.
Finally, our code returns a result to the _Runtime_, which then passes the response back to the Lambda service to be
sent back out to the requester.

We use the AWS C++ Runtime implementation. See [Libraries](#libraries).

The consequence of this is that we have two options:

**1. Test our code by hosting our own "mock Lambda service".**

- `axel/main.cpp` loads the _Runtime_ layer as well as our code. The _Runtime_ will continuously poll a hardcoded
  endpoint, thinking that it is running inside a real Lambda function, and try to get event data from the Lambda
  service. Obviously, this will fail, because the code is actually running in a local container. There is no Lambda
  service.
- To remedy this, we download
  the [AWS Runtime Interface Emulator](https://github.com/aws/aws-lambda-runtime-interface-emulator) (RIE). When the
  emulator binary is run, it acts like the Lambda service, storing event data we pass to it and allowing the _Runtime_
  to poll it.
- To test Axel's backend while simulating the production environment with the AWS Runtime Interface Emulator,
  see [Working with the RIE](#working-with-the-rie)

**2. Test our code independently of the _Runtime_.**

- In many situations during development, there's no real need to host the RIE, start up the _Runtime_, and exercise
  paths through our code by _POST_ ing data to the RIE.
- A simpler solution would be to use `axel/sandbox.cpp` to define custom entrypoints into the libraries and functions of
  Axel's backend.
- To test Axel's backend through custom entrypoints,
  see [Working with custom entrypoints](#working-with-custom-entrypoints)

# Setup

This section guides you through a basic setup of the development environment, concluding with successfully running a
Google Test suite.

**You are strongly recommended to use an IDE like CLion or Visual Studio**.

## 1. Cloning the repository

To clone the repository, `cd` into the directory in which you want the **root folder** of the repository to reside.

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

**1. dependencies** `docker pull craftarc/axel:dependencies`

- Stores all installed libraries the project uses.

**2. staging** `docker pull craftarc/axel:staging`

- An environment that as closely mimics deployment to the real Lambda service as possible.

## 3. Building the project

Axel's top-level CMakeLists.txt builds two primary executables: `main.cpp` and `sandbox.cpp`. For more information on
why there are two
executables, see [The Lambda Execution Environment](#the-lambda-execution-environment).

Along with the two executables, it also builds a testing executable.
**To build the project**,

- mount the project root into the container
- start an interactive shell

```
docker run -it craftarc/axel:dependencies -v .:/axel
```

You should be dropped into `/axel`, where the root directory of the project should be mounted. Verify this with `pwd`.

**To build the tests**, run:

```
mkdir -p build    # Make the build directory
cmake -B build -S . -G Ninja
```

Wait for compilation to complete. The executables should be compiled into `/axel/build`. <br>
Finally, run the tests: `build/tests`

All tests should pass.

# Configuration

## Working with the RIE

The `main` executable starts the AWS C++ runtime, which starts polling the hardcoded Lambda
service endpoint to get events. We need to mount the RIE before main is run so there _is_ an endpoint to poll in the
first place.

Two methods to start the RIE for main to poll against are provided.

### Mounting the RIE into the container

1. [Download](https://github.com/awslabs/aws-lambda-cpp) the RIE for your platform.
2. Start a container running the _craftarc/axel:dependencies_ image. A standardised start script is provided
   via `docker compose`:

```
docker compose up dev
```

This mounts the RIE binary and the project root into the container, recompiles Axel's code if there are changes, then
executes the binary, passing in the handler 
here our _main_ executable) as an argument. It builds Axel with `CMAKE_BUILD_TYPE=Debug`.

You may _POST_ something to the container's exposed port to test for a response.

```
curl --location 'http://localhost:9000/2015-03-31/functions/function/invocations' \
--header 'Content-Type: application/json' \
--data '{
    "key1": "value1",
    "key2": "value2"
}'
```

This method is suitable for **testing changes**, such as verbose logging, breaking changes

### Deploying into an official RIE image

AWS provides an _official_ Lambda image, which already embeds the RIE binary, and uses the same operating system as the
actual deployment environment.

You may have noticed that the [build step](#3-build-the-project) produces an archive `main.zip`. Axel's code is deployed
onto Lambda by directly uploading this zip file. AWS automatically unpacks this archive and runs setup scripts.

The _craftarc/axel:staging_ image mirrors this process. Upon launching this container, `build/main.zip` is **copied**
into the container and unpacked in a similar fashion. **Make sure `main.zip` is freshly built, because this process does
not compile new executables**.

In a craftarc/axel:dependencies container, at `/axel`, run the following command:
```
scripts/build-release.sh test

```

This builds Axel with `CMAKE_BUILD_TYPE=Release`, targeting testing servers.
Note that running `scripts/build-release.sh prod` will target production servers,
but running this in staging will not work because the credentials in `.aws` are insufficient.

Then start the staging service:

```
docker compose up --build staging
```

which uses the `main.zip` output from the previous command.

This method is suitable for **staging** tests, as it most closely mimics the deployment environment.

Behaviour is verified here before pushing to production. Test behaviour by POSTing
to the exposed port on the container, 9000.

## Working with custom entrypoints

**To work with custom entrypoints, we want to execute the `sandbox` executable instead.** <br>

**1. Build updated binaries**.
In a container running the _craftarc/axel:dependencies_ image, build the updated
binaries with the desired cmake flags.

**3. Navigate to and run the `sandbox` executable.**

```
cd build
./sandbox
```

## Utililty scripts
In `/scripts`, there are a few utility scripts for common tasks.

- `build-release.sh`
    - arguments: `prod` or `test`
    - This script builds release binaries and bundles them up into a deployable
    main.zip. This means no debug information!
    - To test in `craftarc/axel:staging`, run `build-release.sh test`, then
    **rebuild** the image and run it. (see [Deploying into an official RIE image](#deploying-into-an-official-rie-image))

# Contributing

# License

GNU General Public License v3.0


